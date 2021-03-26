//
//  Rekord Buddy - The future proof music collection tool made by DJs for DJs.
//  Copyright (C) 2020-2021 Didier Malenfant (didier@rekordbuddy.org)
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#pragma once

#include <RekordBuddyCollection/Implementation/Crates/OtherTracksPlaylist.hpp>
#include <RekordBuddyCollection/Implementation/Crates/Internal/PlaylistInternal.hpp>
#include <RekordBuddyCollection/Implementation/Collection.hpp>

namespace NxA {

// -- Operator Specialization
inline constexpr bool operator==(const RekordBuddy::CollectionImplementation::PersistentOtherTracksPlaylist& first,
                                 const RekordBuddy::CollectionImplementation::PersistentOtherTracksPlaylist& second) noexcept
{
    return first.objectID() == second.objectID();
}

inline constexpr bool operator<(const RekordBuddy::CollectionImplementation::PersistentOtherTracksPlaylist& first,
                                const RekordBuddy::CollectionImplementation::PersistentOtherTracksPlaylist& second) noexcept
{
    return first.objectID() < second.objectID();
}

namespace RekordBuddy { namespace CollectionImplementation {

// -- Private Functions
inline void p_deleteOtherTracksPlaylistFromCollection(PersistentOtherTracksPlaylist& playlist)
{
    // -- This is a collection object deletion method. This comment can help find them all.
    // -- Collection objects must also delete any orphan children they have but only after deleting
    // -- themselves to prevent infinite recursions. It is assumed that Persistence
    // -- will remove them from any parents (unless the parent is already deleted).
    p_deletePlaylistFromCollection(playlist);
}

Set<ObjectID> p_trackObjectIDsNotFoundInChildrenSortingCratesOf(const Playlist&);
void p_updateOtherTracksPlaylistSmartContent(OtherTracksPlaylist&, PersistentOtherTracksPlaylist&);
Optional<OtherTracksPlaylist> p_findOtherTrackPlaylistIn(const Playlist&);

} } }
