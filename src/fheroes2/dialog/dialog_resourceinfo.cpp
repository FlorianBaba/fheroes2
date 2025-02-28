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

#include "cursor.h"
#include "dialog.h"
#include "localevent.h"
#include "resource.h"
#include "text.h"
#include "ui_button.h"

int Dialog::ResourceInfo( const std::string & header, const std::string & message, const Funds & rs, int buttons )
{
    fheroes2::Display & display = fheroes2::Display::instance();

    // setup cursor
    const CursorRestorer cursorRestorer( true, Cursor::POINTER );

    TextBox box1( header, Font::YELLOW_BIG, BOXAREA_WIDTH );
    TextBox box2( message, Font::BIG, BOXAREA_WIDTH );
    Resource::BoxSprite rbs( rs, BOXAREA_WIDTH );

    const int spacer = 10;

    FrameBox box( box1.h() + spacer + box2.h() + spacer + rbs.GetArea().height, buttons != 0 );
    fheroes2::Point pos( box.GetArea().x, box.GetArea().y );

    if ( !header.empty() )
        box1.Blit( pos.x, pos.y );
    pos.y += box1.h() + spacer;

    if ( !message.empty() )
        box2.Blit( pos.x, pos.y );
    pos.y += box2.h() + spacer;

    rbs.SetPos( pos.x, pos.y );
    rbs.Redraw();

    LocalEvent & le = LocalEvent::Get();

    fheroes2::ButtonGroup btnGroups( box.GetArea(), buttons );
    btnGroups.draw();

    display.render();

    int result = Dialog::ZERO;

    while ( result == Dialog::ZERO && le.HandleEvents() ) {
        if ( !buttons && !le.MousePressRight() )
            break;
        result = btnGroups.processEvents();
    }

    return result;
}
