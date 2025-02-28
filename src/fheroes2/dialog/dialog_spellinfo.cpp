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

#include "agg_image.h"
#include "cursor.h"
#include "dialog.h"
#include "game.h"
#include "icn.h"
#include "localevent.h"
#include "settings.h"
#include "spell.h"
#include "spell_info.h"
#include "text.h"
#include "tools.h"
#include "translations.h"
#include "ui_button.h"

void Dialog::SpellInfo( const Spell & spell, const HeroBase * hero, const bool showOkayButton )
{
    std::string msg = fheroes2::getSpellDescription( spell, hero );
    u32 extra = spell.ExtraValue();

    switch ( spell.GetID() ) {
    case Spell::HASTE:
    case Spell::MASSHASTE:
        if ( 0 == extra )
            extra = 2;
        break;

    default:
        break;
    }

    if ( 1 == extra )
        StringReplace( msg, "%{count}", _( "one" ) );
    else if ( 2 == extra )
        StringReplace( msg, "%{count}", _( "two" ) );
    else
        StringReplace( msg, "%{count}", extra );

    Dialog::SpellInfo( spell.GetName(), msg, spell, showOkayButton );
}

void Dialog::SpellInfo( const std::string & header, const std::string & message, const Spell & spell, bool ok_button )
{
    fheroes2::Display & display = fheroes2::Display::instance();

    // setup cursor
    const CursorRestorer cursorRestorer( ok_button, Cursor::POINTER );

    TextBox box1( header, Font::YELLOW_BIG, BOXAREA_WIDTH );
    TextBox box2( message, Font::BIG, BOXAREA_WIDTH );
    Text text( std::string( spell.GetName() ) + " [" + std::to_string( spell.SpellPoint( nullptr ) ) + "]", Font::SMALL );

    const fheroes2::Sprite & sprite = fheroes2::AGG::GetICN( ICN::SPELLS, spell.IndexSprite() );
    const int spacer = 10;

    FrameBox box( box1.h() + spacer + box2.h() + spacer + sprite.height() + 2 + text.h(), ok_button );
    fheroes2::Rect pos = box.GetArea();

    if ( !header.empty() )
        box1.Blit( pos.x, pos.y );
    pos.y += box1.h() + spacer;

    if ( !message.empty() )
        box2.Blit( pos.x, pos.y );
    pos.y += box2.h() + spacer;

    // blit sprite
    pos.x = box.GetArea().x + ( pos.width - sprite.width() ) / 2;
    fheroes2::Blit( sprite, display, pos.x, pos.y );

    // small text
    pos.x = box.GetArea().x + ( pos.width - text.w() ) / 2;
    pos.y = pos.y + sprite.height() + 2;
    text.Blit( pos.x, pos.y );

    LocalEvent & le = LocalEvent::Get();

    const int system = Settings::Get().ExtGameEvilInterface() ? ICN::SYSTEME : ICN::SYSTEM;
    const fheroes2::Sprite & buttonSprite = fheroes2::AGG::GetICN( system, 1 );

    fheroes2::Button button( box.GetArea().x + ( box.GetArea().width - buttonSprite.width() ) / 2, box.GetArea().y + box.GetArea().height - buttonSprite.height(), system,
                             1, 2 );

    if ( !ok_button ) {
        button.disable();
    }

    if ( button.isEnabled() ) {
        button.draw();
    }

    display.render();

    // message loop
    while ( le.HandleEvents() ) {
        if ( !ok_button && !le.MousePressRight() )
            break;
        if ( button.isEnabled() )
            le.MousePressLeft( button.area() ) ? button.drawOnPress() : button.drawOnRelease();
        if ( button.isEnabled() && le.MouseClickLeft( button.area() ) ) {
            break;
        }
        if ( HotKeyCloseWindow ) {
            break;
        }
    }
}
