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

#include <RekordBuddyCollection/Implementation/Crates/Folder.hpp>
#include <RekordBuddyCollection/Implementation/Crates/Internal/CrateInternal.hpp>

namespace NxA {

// -- Operator Specialization
inline constexpr bool operator==(const RekordBuddy::CollectionImplementation::PersistentFolder& first,
                                 const RekordBuddy::CollectionImplementation::PersistentFolder& second) noexcept
{
    return first.objectID() == second.objectID();
}

inline constexpr bool operator<(const RekordBuddy::CollectionImplementation::PersistentFolder& first,
                                const RekordBuddy::CollectionImplementation::PersistentFolder& second) noexcept
{
    return first.objectID() < second.objectID();
}

namespace RekordBuddy { namespace CollectionImplementation {

// -- Private Functions
inline void p_deleteFolderFromCollection(PersistentFolder& folder)
{
    // -- This is a collection object deletion method. This comment can help find them all.
    // -- Collection objects must also delete any orphan children they have but only after deleting
    // -- themselves to prevent infinite recursions. It is assumed that Persistence
    // -- will remove them from any parents (unless the parent is already deleted).
    p_deleteCrateFromCollection(folder);
}

} } }
