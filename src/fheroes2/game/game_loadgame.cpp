/***************************************************************************
 *   Free Heroes of Might and Magic II: https://github.com/ihhub/fheroes2  *
 *   Copyright (C) 2019 - 2022                                             *
 *                                                                         *
 *   Free Heroes2 Engine: http://sourceforge.net/projects/fheroes2         *
 *   Copyright (C) 2009 by Andrey Afletdinov <fheroes2@gmail.com>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "game.h"
#include "agg.h"
#include "agg_image.h"
#include "audio.h"
#include "cursor.h"
#include "dialog.h"
#include "game_io.h"
#include "game_mainmenu_ui.h"
#include "icn.h"
#include "localevent.h"
#include "mus.h"
#include "screen.h"
#include "settings.h"
#include "text.h"
#include "translations.h"
#include "ui_button.h"

fheroes2::GameMode Game::LoadCampaign()
{
    Settings::Get().SetGameType( Game::TYPE_CAMPAIGN );
    return DisplayLoadGameDialog();
}

fheroes2::GameMode Game::LoadHotseat()
{
    Settings::Get().SetGameType( Game::TYPE_HOTSEAT );
    return DisplayLoadGameDialog();
}

fheroes2::GameMode Game::LoadMulti()
{
    fheroes2::Display & display = fheroes2::Display::instance();

    // setup cursor
    const CursorRestorer cursorRestorer( true, Cursor::POINTER );

    // image background
    const fheroes2::Sprite & back = fheroes2::AGG::GetICN( ICN::HEROES, 0 );
    fheroes2::drawMainMenuScreen();

    const fheroes2::Sprite & panel = fheroes2::AGG::GetICN( ICN::REDBACK, 0 );
    const int32_t panelOffset = fheroes2::Display::DEFAULT_HEIGHT - panel.height();
    const int32_t panelXPos = back.width() - ( panel.width() + panelOffset );
    fheroes2::Blit( panel, display, panelXPos, panelOffset );

    const int32_t buttonMiddlePos = panelXPos + SHADOWWIDTH + ( panel.width() - SHADOWWIDTH ) / 2;
    const fheroes2::Sprite & buttonSample = fheroes2::AGG::GetICN( ICN::BTNNEWGM, 0 );
    const int32_t buttonWidth = buttonSample.width();
    const int32_t buttonXPos = buttonMiddlePos - buttonWidth / 2 - 3; // 3 is button shadow
    const int32_t buttonYPos = 46;
    const int32_t buttonYStep = 66;

    fheroes2::Button buttonHotSeat( buttonXPos, buttonYPos, ICN::BTNMP, 0, 1 );
    fheroes2::Button buttonNetwork( buttonXPos, buttonYPos + buttonYStep * 1, ICN::BTNMP, 2, 3 );
    fheroes2::Button buttonCancelGame( buttonXPos, buttonYPos + buttonYStep * 5, ICN::BTNMP, 8, 9 );

    buttonHotSeat.draw();
    buttonCancelGame.draw();
    buttonNetwork.disable();

    display.render();

    LocalEvent & le = LocalEvent::Get();
    while ( le.HandleEvents() ) {
        le.MousePressLeft( buttonHotSeat.area() ) ? buttonHotSeat.drawOnPress() : buttonHotSeat.drawOnRelease();
        le.MousePressLeft( buttonCancelGame.area() ) ? buttonCancelGame.drawOnPress() : buttonCancelGame.drawOnRelease();

        if ( le.MouseClickLeft( buttonHotSeat.area() ) || HotKeyPressEvent( EVENT_BUTTON_HOTSEAT ) ) {
            if ( ListFiles::IsEmpty( GetSaveDir(), GetSaveFileExtension( Game::TYPE_HOTSEAT ), false ) ) {
                Dialog::Message( _( "Load Game" ), _( "No save files to load." ), Font::BIG, Dialog::OK );
            }
            else {
                return fheroes2::GameMode::LOAD_HOT_SEAT;
            }
        }
        else if ( HotKeyPressEvent( EVENT_DEFAULT_EXIT ) || le.MouseClickLeft( buttonCancelGame.area() ) ) {
            return fheroes2::GameMode::LOAD_GAME;
        }

        // right info
        else if ( le.MousePressRight( buttonHotSeat.area() ) ) {
            Dialog::Message( _( "Hot Seat" ),
                             _( "Play a Hot Seat game, where 2 to 4 players play around the same computer, switching into the 'Hot Seat' when it is their turn." ),
                             Font::BIG );
        }
        else if ( le.MousePressRight( buttonCancelGame.area() ) ) {
            Dialog::Message( _( "Cancel" ), _( "Cancel back to the main menu." ), Font::BIG );
        }
    }

    return fheroes2::GameMode::LOAD_GAME;
}

fheroes2::GameMode Game::LoadGame()
{
    // Stop all sounds, but not the music
    Mixer::Stop();

    AGG::PlayMusic( MUS::MAINMENU, true, true );

    fheroes2::Display & display = fheroes2::Display::instance();

    // setup cursor
    const CursorRestorer cursorRestorer( true, Cursor::POINTER );

    const fheroes2::Sprite & back = fheroes2::AGG::GetICN( ICN::HEROES, 0 );
    fheroes2::drawMainMenuScreen();

    const fheroes2::Sprite & panel = fheroes2::AGG::GetICN( ICN::REDBACK, 0 );
    const int32_t panelOffset = fheroes2::Display::DEFAULT_HEIGHT - panel.height();
    const int32_t panelXPos = back.width() - ( panel.width() + panelOffset );
    fheroes2::Blit( panel, display, panelXPos, panelOffset );

    const int32_t buttonMiddlePos = panelXPos + SHADOWWIDTH + ( panel.width() - SHADOWWIDTH ) / 2;
    const fheroes2::Sprite & buttonSample = fheroes2::AGG::GetICN( ICN::BTNNEWGM, 0 );
    const int32_t buttonWidth = buttonSample.width();

    std::vector<fheroes2::Button> buttons( 4 );
    const size_t buttonCount = buttons.size();

    buttons[0].setICNInfo( ICN::BTNNEWGM, 0, 1 );
    buttons[1].setICNInfo( ICN::BTNNEWGM, 2, 3 );
    buttons[2].setICNInfo( ICN::BTNNEWGM, 4, 5 );
    buttons[3].setICNInfo( ICN::BTNNEWGM, 6, 7 );

    const int32_t buttonXPos = buttonMiddlePos - buttonWidth / 2 - 3; // 3 is button shadow
    const int32_t buttonYPos = 46;
    const int32_t buttonYStep = 66;

    for ( size_t i = 0; i < buttonCount - 1; ++i ) {
        buttons[i].setPosition( buttonXPos, buttonYPos + buttonYStep * static_cast<int32_t>( i ) );
        buttons[i].draw();
    }

    // following the cancel button in newgame
    buttons.back().setPosition( buttonXPos, buttonYPos + buttonYStep * 5 );
    buttons.back().draw();

    display.render();

    LocalEvent & le = LocalEvent::Get();

    while ( le.HandleEvents() ) {
        for ( size_t i = 0; i < buttonCount; ++i ) {
            le.MousePressLeft( buttons[i].area() ) ? buttons[i].drawOnPress() : buttons[i].drawOnRelease();
        }

        if ( le.MouseClickLeft( buttons[0].area() ) || HotKeyPressEvent( EVENT_BUTTON_STANDARD ) ) {
            if ( ListFiles::IsEmpty( GetSaveDir(), GetSaveFileExtension( Game::TYPE_STANDARD ), false ) ) {
                Dialog::Message( _( "Load Game" ), _( "No save files to load." ), Font::BIG, Dialog::OK );
            }
            else {
                return fheroes2::GameMode::LOAD_STANDARD;
            }
        }
        else if ( le.MouseClickLeft( buttons[1].area() ) || HotKeyPressEvent( EVENT_BUTTON_CAMPAIGN ) ) {
            if ( ListFiles::IsEmpty( GetSaveDir(), GetSaveFileExtension( Game::TYPE_CAMPAIGN ), false ) ) {
                Dialog::Message( _( "Load Game" ), _( "No save files to load." ), Font::BIG, Dialog::OK );
            }
            else {
                return fheroes2::GameMode::LOAD_CAMPAIN;
            }
        }
        else if ( le.MouseClickLeft( buttons[2].area() ) || HotKeyPressEvent( EVENT_BUTTON_MULTI ) ) {
            return fheroes2::GameMode::LOAD_MULTI;
        }
        else if ( le.MouseClickLeft( buttons[3].area() ) || HotKeyPressEvent( EVENT_DEFAULT_EXIT ) ) {
            return fheroes2::GameMode::MAIN_MENU;
        }
        else if ( le.MousePressRight( buttons[0].area() ) ) {
            Dialog::Message( _( "Standard Game" ), _( "A single player game playing out a single map." ), Font::BIG );
        }
        else if ( le.MousePressRight( buttons[1].area() ) ) {
            Dialog::Message( _( "Campaign Game" ), _( "A single player game playing through a series of maps." ), Font::BIG );
        }
        else if ( le.MousePressRight( buttons[2].area() ) ) {
            Dialog::Message( _( "Multi-Player Game" ), _( "A multi-player game, with several human players completing against each other on a single map." ), Font::BIG );
        }
        else if ( le.MousePressRight( buttons[3].area() ) ) {
            Dialog::Message( _( "Cancel" ), _( "Cancel back to the main menu." ), Font::BIG );
        }
    }

    return fheroes2::GameMode::MAIN_MENU;
}

fheroes2::GameMode Game::LoadStandard()
{
    Settings::Get().SetGameType( Game::TYPE_STANDARD );
    return DisplayLoadGameDialog();
}

fheroes2::GameMode Game::DisplayLoadGameDialog()
{
    // Stop all sounds, but not the music
    Mixer::Stop();

    AGG::PlayMusic( MUS::MAINMENU, true, true );

    // setup cursor
    const CursorRestorer cursorRestorer( true, Cursor::POINTER );

    // image background
    fheroes2::drawMainMenuScreen();

    fheroes2::Display::instance().render();

    const std::string file = Dialog::SelectFileLoad();
    if ( file.empty() ) {
        return fheroes2::GameMode::LOAD_GAME;
    }

    const fheroes2::GameMode returnValue = Game::Load( file );
    if ( returnValue == fheroes2::GameMode::CANCEL ) {
        return fheroes2::GameMode::LOAD_GAME;
    }

    return returnValue;
}
