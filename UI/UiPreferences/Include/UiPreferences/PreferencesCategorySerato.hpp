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

#include <UiModel/Preferences/PreferencesCategoryBase.hpp>
#include <Base/Base.hpp>

namespace Ui {

class PreferencesSerato;

}

namespace NxA { namespace RekordBuddy {

class PreferencesCategorySerato : public PreferencesCategoryBase
{
protected:
    // -- Protected Instance Variables
    Ui::PreferencesSerato* p_preferencesPanelContents = nullptr;

    // -- Protected Instance Variables
    void p_handleConvertCuesFlagChanged(int);
    void p_handleIgnorePlaylistsChanged(int);

public:
    // -- Class Methods
    static void registerDefaultPreferenceValuesIn(NotNull<NxA::UserPreferences*>);

    // -- Constructors & Destructors
    PreferencesCategorySerato (NotNull<NxA::UserPreferences*> userPreferences) : PreferencesCategoryBase{ std::move(userPreferences) } { }
    ~PreferencesCategorySerato() override;

    // -- Instance Methods
    NxA::String CategoryName() const override;
    void migratePreferenceValues() const override;
    boolean hasPreferencesToMigrate() const override;
    void populatePreferencesPanel(QWidget*) override;

    boolean appClashesWithAppNamed(const String&) override;
};

} }
