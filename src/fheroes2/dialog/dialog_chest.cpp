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
#include "settings.h"
#include "text.h"
#include "translations.h"
#include "world.h"

bool Dialog::SelectGoldOrExp( const std::string & header, const std::string & message, u32 gold, u32 expr, const Heroes & hero )
{
    fheroes2::Display & display = fheroes2::Display::instance();
    const int system = Settings::Get().ExtGameEvilInterface() ? ICN::SYSTEME : ICN::SYSTEM;

    // setup cursor
    const CursorRestorer cursorRestorer( true, Cursor::POINTER );

    const fheroes2::Sprite & sprite_gold = fheroes2::AGG::GetICN( ICN::RESOURCE, 6 );
    const fheroes2::Sprite & sprite_expr = fheroes2::AGG::GetICN( ICN::EXPMRL, 4 );

    TextBox box1( header, Font::YELLOW_BIG, BOXAREA_WIDTH );
    TextBox box2( message, Font::BIG, BOXAREA_WIDTH );

    Text text;
    text.Set( std::to_string( gold ) + " (" + _( "Total: " ) + std::to_string( world.GetKingdom( hero.GetColor() ).GetFunds().Get( Resource::GOLD ) ) + ")",
              Font::SMALL );

    const int spacer = 10;
    FrameBox box( box1.h() + spacer + box2.h() + spacer + sprite_expr.height() + 2 + text.h(), true );

    fheroes2::Point pt;
    pt.x = box.GetArea().x + box.GetArea().width / 2 - fheroes2::AGG::GetICN( system, 9 ).width() - 20;
    pt.y = box.GetArea().y + box.GetArea().height - fheroes2::AGG::GetICN( system, 5 ).height();
    fheroes2::Button button_yes( pt.x, pt.y, system, 5, 6 );

    pt.x = box.GetArea().x + box.GetArea().width / 2 + 20;
    pt.y = box.GetArea().y + box.GetArea().height - fheroes2::AGG::GetICN( system, 7 ).height();
    fheroes2::Button button_no( pt.x, pt.y, system, 7, 8 );

    fheroes2::Rect pos = box.GetArea();

    if ( !header.empty() )
        box1.Blit( pos.x, pos.y );
    pos.y += box1.h() + spacer;

    if ( !message.empty() )
        box2.Blit( pos.x, pos.y );
    pos.y += box2.h() + spacer;

    pos.y += sprite_expr.height();
    // sprite1
    pos.x = box.GetArea().x + box.GetArea().width / 2 - sprite_gold.width() - 30;
    fheroes2::Blit( sprite_gold, display, pos.x, pos.y - sprite_gold.height() );
    // text
    text.Blit( pos.x + sprite_gold.width() / 2 - text.w() / 2, pos.y + 2 );

    // sprite2
    pos.x = box.GetArea().x + box.GetArea().width / 2 + 30;
    fheroes2::Blit( sprite_expr, display, pos.x, pos.y - sprite_expr.height() );
    // text
    text.Set( std::to_string( expr ) + " (" + _( "Need: " ) + std::to_string( Heroes::GetExperienceFromLevel( hero.GetLevel() ) - hero.GetExperience() ) + ")",
              Font::SMALL );
    text.Blit( pos.x + sprite_expr.width() / 2 - text.w() / 2, pos.y + 2 );

    button_yes.draw();
    button_no.draw();

    display.render();
    LocalEvent & le = LocalEvent::Get();
    bool result = false;

    // message loop
    while ( le.HandleEvents() ) {
        le.MousePressLeft( button_yes.area() ) ? button_yes.drawOnPress() : button_yes.drawOnRelease();
        le.MousePressLeft( button_no.area() ) ? button_no.drawOnPress() : button_no.drawOnRelease();

        if ( Game::HotKeyPressEvent( Game::EVENT_DEFAULT_READY ) || le.MouseClickLeft( button_yes.area() ) ) {
            result = true;
            break;
        }
        if ( Game::HotKeyPressEvent( Game::EVENT_DEFAULT_EXIT ) || le.MouseClickLeft( button_no.area() ) ) {
            result = false;
            break;
        }
    }

    return result;
}
