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

#ifndef H2OBJLAVA_H
#define H2OBJLAVA_H

#include "types.h"

namespace ObjLav2
{
    bool isAction( u32 index );
    bool isShadow( const uint8_t index );
    int GetPassable( const uint8_t index );
    int GetActionObject( u32 index );
}

namespace ObjLav3
{
    bool isAction( u32 index );
    bool isShadow( const uint8_t index );
    int GetPassable( const uint8_t index );
    int GetActionObject( u32 index );
}

namespace ObjLava
{
    bool isAction( u32 index );
    bool isShadow( const uint8_t index );
    int GetPassable( const uint8_t index );
    int GetActionObject( u32 index );
}

#endif
