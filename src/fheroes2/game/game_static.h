/***************************************************************************
 *   Free Heroes of Might and Magic II: https://github.com/ihhub/fheroes2  *
 *   Copyright (C) 2019 - 2022                                             *
 *                                                                         *
 *   Free Heroes2 Engine: http://sourceforge.net/projects/fheroes2         *
 *   Copyright (C) 2011 by Andrey Afletdinov <fheroes2@gmail.com>          *
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

#ifndef H2GAMESTATIC_H
#define H2GAMESTATIC_H

#include "mp2.h"
#include "types.h"

class StreamBase;

namespace Skill
{
    struct stats_t;
    struct values_t;
    struct secondary_t;
}

namespace GameStatic
{
    u32 GetLostOnWhirlpoolPercent( void );
    u32 GetGameOverLostDays( void );
    u32 GetOverViewDistance( u32 );

    u32 GetKingdomMaxHeroes( void );

    u32 GetCastleGrownWell( void );
    u32 GetCastleGrownWel2( void );
    u32 GetCastleGrownWeekOf( void );
    u32 GetCastleGrownMonthOf( void );

    u32 GetHeroesRestoreSpellPointsPerDay( void );
    u32 GetMageGuildRestoreSpellPointsPercentDay( int level );

    s32 ObjectVisitedModifiers( const MP2::MapObjectType objectType );

    int GetBattleMoatReduceDefense( void );

    const Skill::stats_t * GetSkillStats( int race );
    const Skill::values_t * GetSkillValues( int skill );
    const Skill::secondary_t * GetSkillForWitchsHut( void );
}

#endif
