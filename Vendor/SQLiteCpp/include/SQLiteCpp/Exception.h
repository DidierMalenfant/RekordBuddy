/**
 * @file    Exception.h
 * @ingroup SQLiteCpp
 * @brief   Encapsulation of the error message from SQLite3 on a std::runtime_error.
 *
 * Copyright (c) 2012-2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include <stdexcept>
#include <string>


namespace SQLite
{


/**
 * @brief Encapsulation of the error message from SQLite3, based on std::runtime_error.
 */
class Exception : public std::runtime_error
{
public:
    /**
     * @brief Encapsulation of the error message from SQLite3, based on std::runtime_error.
     *
     * @param[in] aErrorMessage The string message describing the SQLite error
     */
    explicit Exception(const std::string& aErrorMessage) : std::runtime_error(aErrorMessage)
    {
    }
};

class DatabaseLocked : public std::runtime_error
{
public:
    /**
     * @brief Thrown if the write has failed because this same process has a lock.
     * The SQLITE_LOCKED result code indicates that a write operation could not continue because of a conflict within the same database connection or a conflict with a different database connection that uses a shared cache.
     *
     * @param[in] aErrorMessage The string message describing the SQLite error
     */
    explicit DatabaseLocked(const std::string& aErrorMessage) : std::runtime_error(aErrorMessage)
    {
    }
};
class DatabaseBusy : public std::runtime_error
{
public:
    /**
     * @brief Thrown if the write has failed because the database is busy (SQLITE_BUSY).
     * The SQLITE_BUSY result code indicates that the database file could not be written (or in some cases read) because of concurrent activity by some other database connection, usually a database connection in a separate process.
     *
     * @param[in] aErrorMessage The string message describing the SQLite error
     */
    explicit DatabaseBusy(const std::string& aErrorMessage) : std::runtime_error(aErrorMessage)
    {
    }
};


}  // namespace SQLite


/// Compatibility with non-clang compilers.
#ifndef __has_feature
    #define __has_feature(x) 0
#endif

// Detect whether the compiler supports C++11 noexcept exception specifications.
#if (  defined(__GNUC__) && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 7) || (__GNUC__ > 4)) \
    && defined(__GXX_EXPERIMENTAL_CXX0X__))
// GCC 4.7 and following have noexcept
#elif defined(__clang__) && __has_feature(cxx_noexcept)
// Clang 3.0 and above have noexcept
#elif defined(_MSC_VER) && _MSC_VER > 1800
// Visual Studio 2015 and above have noexcept
#else
    // Visual Studio 2013 does not support noexcept, and "throw()" is deprecated by C++11
    #define noexcept
#endif
