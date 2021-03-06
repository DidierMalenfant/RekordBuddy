#!/usr/bin/env bash
#
#   Given a directory, uploads any *.dSYM bundles with the directory to
#   Bugsnag. As a pre-upload step, bitcode symbol maps can be combined with
#   dSYM files to ensure symbols are sent for bitcode-enabled binaries.
#
#   Depends on:
#   * curl
#   * dwarfdump
#   * dsymutil (for --symbol-maps, optional)
#   * unzip (for uploading a .zip file, optional)

function print_usage() {
    echo "Usage: $0 [--symbol-maps DIR] dSYMS_PATH"
    echo
    echo "-h, --help          Displays this message"
    echo "-v, --verbose       Print verbose logging output during execution"
    echo "--api-key API_KEY   The API key of the project the dSYM should be applied to"
    echo "--symbol-maps DIR   Path to a directory of bitcode symbol maps. The"
    echo "                    dSYM files will be restored with symbols prior to"
    echo "                    upload. Requires dsymutil."
    echo "--upload-server URL The server receiving dSYM files. Set this value if"
    echo "                    using an on-premise Bugsnag installation"
    echo "--project-root DIR  The root directory of the project. This will help to"
    echo "                    group error reports by project"
    echo "dSYMS_PATH          A directory or .zip file containing *.dSYM bundles to"
    echo "                    upload"
}

function exit_with_usage() {
    echo $1
    echo
    print_usage
    exit 1
}

function log() {
    if [[ $silent != 1 ]]; then
        echo $@
    fi
}

function log_verbose() {
    if [[ $verbose == 1 ]]; then
        log $@
    fi
}

upload_server=https://upload.bugsnag.com
unset symbol_maps
unset dsym_dir
unset verbose
unset silent
unset project_root
unset api_key

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            print_usage
            exit 0;;
        -s|--silent)
            silent=1
            shift;;
        -v|--verbose)
            verbose=1
            shift;;
        --symbol-maps)
            symbol_maps=$2
            shift
            shift;;
        --upload-server)
            upload_server=$2
            shift
            shift;;
        --api-key)
            api_key=$2
            shift
            shift;;
        --project-root)
            project_root=$2
            shift
            shift;;
        -*)
            exit_with_usage "Invalid parameter provided: $1";;
        *)
            dsym_dir=$1
            break;;
    esac
done

# Set IFS to ensure that file paths with spaces in get processed correctly
IFS=$'\n'

if [[ ! -z $symbol_maps ]]; then
    if [[ ! -d $symbol_maps ]]; then
        exit_with_usage "Bitcode symbol map parameter is not a directory"
    elif [[ ! -x "$(command -v dsymutil 2>/dev/null)" ]]; then
        exit_with_usage "dsymutil command not found."
    fi
fi
if [[ -z $dsym_dir ]]; then
    exit_with_usage "No dSYM directory provided"
fi
if [[ ! -d $dsym_dir ]]; then
    if [[ $dsym_dir == *".zip" ]]; then
        if [[ ! -x "$(command -v unzip 2>/dev/null)" ]]; then
            exit_with_usage "unzip command not found."
        fi
        temp_dir=$(mktemp -dt "bugsnag-dsym-upload.XXX")
        unzip -qq $dsym_dir -d $temp_dir
        dsym_dir=$temp_dir
    else
        exit_with_usage "'$dsym_dir' is not a directory or a zip file"
    fi
fi

log_verbose "Uploading files to $upload_server"
success_count=0
fail_count=0

for dsym in $dsym_dir/*.dSYM; do
    log_verbose "Preparing to upload $dsym"

    if [[ -d $symbol_maps ]]; then
        log_verbose "Updating file with bitcode symbol maps in $symbol_maps"
        dsymutil "$dsym" --symbol-map "$symbol_maps"
    fi

    dwarf_data=$dsym/Contents/Resources/DWARF
    if [[ ! -d $dwarf_data ]]; then
        log_verbose "Skipping file missing DWARF data: $dsym"
        fail_count=$((fail_count+1))
        continue
    fi
    for file in $dwarf_data/*; do
        uuid=$(dwarfdump -u $file 2>/dev/null)
        if [[ $uuid == UUID* ]]; then
            log Uploading $uuid

            # Attach the api key and project root parameters if they have been provided
            args=""
            if [[ ! -z $project_root ]]; then
                args="-F projectRoot=\"$project_root\" "
            fi

            if [[ ! -z $api_key ]]; then
                args="$args-F apiKey=$api_key"
            fi

            # We need to shell out to perform the curl as there seems to be some indirect
            # wrapping of this script which causes the command to fail if called directly.
            curl_cmd="curl --silent --show-error $upload_server -F dsym=@\"$file\" $args"
            output=$(sh -c "$curl_cmd")

            if [ $? -eq 0 ] && [ "$output" != "invalid apiKey" ]; then
                success_count=$((success_count+1))
            else
                fail_count=$((fail_count+1))
                log_verbose "Failed to upload file: $file"
            fi
            echo $output | grep -v '^OK$'
            log
        else
            log_verbose "Skipping file without UUID: $file"
            fail_count=$((fail_count+1))
        fi
    done
done

if [ $success_count -gt 0 ]; then
    log "$success_count files uploaded successfully"
fi

if [ $fail_count -gt 0 ]; then
    log "$fail_count files failed to upload"
    exit 1
fi
