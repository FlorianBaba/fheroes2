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

#include "captain.h"
#include "agg_image.h"
#include "castle.h"
#include "icn.h"
#include "interface_icons.h"
#include "luck.h"
#include "morale.h"
#include "race.h"

#include <cassert>

namespace
{
    int GetPortraitIcnId( int race )
    {
        switch ( race ) {
        case Race::KNGT:
            return ICN::PORT0090;
        case Race::BARB:
            return ICN::PORT0091;
        case Race::SORC:
            return ICN::PORT0092;
        case Race::WRLK:
            return ICN::PORT0093;
        case Race::WZRD:
            return ICN::PORT0094;
        case Race::NECR:
            return ICN::PORT0095;
        default:
            return -1;
        }
    }

    fheroes2::Point GetFlagOffset( int race )
    {
        switch ( race ) {
        case Race::KNGT:
            return fheroes2::Point( 43, 9 );
        case Race::BARB:
            return fheroes2::Point( 42, 8 );
        case Race::SORC:
            return fheroes2::Point( 43, 9 );
        case Race::WRLK:
            return fheroes2::Point( 41, 9 );
        case Race::WZRD:
            return fheroes2::Point( 42, 10 );
        case Race::NECR:
            return fheroes2::Point( 42, 9 );
        default:
            return fheroes2::Point();
        }
    }
}

Captain::Captain( Castle & cstl )
    : HeroBase( HeroBase::CAPTAIN, cstl.GetRace() )
    , home( cstl )
{
    SetCenter( home.GetCenter() );
}

bool Captain::isValid( void ) const
{
    return home.isBuild( BUILD_CAPTAIN );
}

int Captain::GetAttack( void ) const
{
    return attack + GetAttackModificator( nullptr );
}

int Captain::GetDefense( void ) const
{
    return defense + GetDefenseModificator( nullptr );
}

int Captain::GetPower( void ) const
{
    const int finalPower = power + GetPowerModificator( nullptr );
    return finalPower < 1 ? 1 : ( finalPower > 255 ? 255 : finalPower );
}

int Captain::GetKnowledge( void ) const
{
    return knowledge + GetKnowledgeModificator( nullptr );
}

int Captain::GetMorale( void ) const
{
    int result = Morale::NORMAL;

    // global modificator
    result += GetMoraleModificator( nullptr );

    // result
    return Morale::Normalize( result );
}

int Captain::GetLuck( void ) const
{
    int result = Luck::NORMAL;

    // global modificator
    result += GetLuckModificator( nullptr );

    // result
    return Luck::Normalize( result );
}

int Captain::GetRace( void ) const
{
    return home.GetRace();
}

int Captain::GetColor( void ) const
{
    return home.GetColor();
}

const std::string & Captain::GetName( void ) const
{
    return home.GetName();
}

int Captain::GetType( void ) const
{
    return HeroBase::CAPTAIN;
}

const Army & Captain::GetArmy( void ) const
{
    return home.GetArmy();
}

Army & Captain::GetArmy( void )
{
    return home.GetArmy();
}

int Captain::GetControl( void ) const
{
    return home.GetControl();
}

void Captain::ActionAfterBattle( void )
{
    SetSpellPoints( GetMaxSpellPoints() );
}

void Captain::ActionPreBattle( void )
{
    SetSpellPoints( GetMaxSpellPoints() );
}

const Castle * Captain::inCastle( void ) const
{
    return &home;
}

fheroes2::Sprite Captain::GetPortrait( const PortraitType type ) const
{
    switch ( type ) {
    case PORT_BIG: {
        const int portraitIcnId = GetPortraitIcnId( GetRace() );
        if ( portraitIcnId < 0 )
            return fheroes2::Image();

        fheroes2::Sprite portait = fheroes2::AGG::GetICN( portraitIcnId, 0 );
        const fheroes2::Image & flag = fheroes2::AGG::GetICN( ICN::GetFlagIcnId( GetColor() ), 0 );

        const fheroes2::Point & offset = GetFlagOffset( GetRace() );
        fheroes2::Blit( flag, portait, offset.x, offset.y );
        return portait;
    }
    case PORT_MEDIUM:
    case PORT_SMALL:
        switch ( GetRace() ) {
        case Race::KNGT:
            return fheroes2::AGG::GetICN( ICN::MINICAPT, 0 );
        case Race::BARB:
            return fheroes2::AGG::GetICN( ICN::MINICAPT, 1 );
        case Race::SORC:
            return fheroes2::AGG::GetICN( ICN::MINICAPT, 2 );
        case Race::WRLK:
            return fheroes2::AGG::GetICN( ICN::MINICAPT, 3 );
        case Race::WZRD:
            return fheroes2::AGG::GetICN( ICN::MINICAPT, 4 );
        case Race::NECR:
            return fheroes2::AGG::GetICN( ICN::MINICAPT, 5 );
        default:
            break;
        }
        break;
    }

    // We shouldn't even reach this code!
    assert( 0 );
    return fheroes2::AGG::GetICN( -1, 0 );
}

void Captain::PortraitRedraw( const int32_t px, const int32_t py, const PortraitType type, fheroes2::Image & dstsf ) const
{
    if ( !isValid() )
        return;

    const fheroes2::Sprite & port = GetPortrait( type );
    if ( PORT_SMALL != type ) { // a normal portrait in a castle or in battle
        fheroes2::Blit( port, dstsf, px, py );
        return;
    }

    const fheroes2::Sprite & mana = fheroes2::AGG::GetICN( ICN::MANA, GetManaIndexSprite() );

    const int iconWidth = Interface::IconsBar::GetItemWidth();
    const int iconHeight = Interface::IconsBar::GetItemHeight();
    const int barWidth = 7;

    // background
    fheroes2::Fill( dstsf, px, py, iconWidth, iconHeight, 0 );

    // mobility is always 0
    const uint8_t blueColor = fheroes2::GetColorId( 15, 30, 120 );
    fheroes2::Fill( dstsf, px, py, barWidth, iconHeight, blueColor );

    // portrait
    fheroes2::Blit( port, dstsf, px + barWidth + 1, py );

    // spell points
    fheroes2::Fill( dstsf, px + barWidth + port.width() + 2, py, barWidth, iconHeight, blueColor );
    fheroes2::Blit( mana, dstsf, px + barWidth + port.width() + 2, py + mana.y() );
}

int Captain::GetManaIndexSprite() const
{
    // valid range (0 - 25)
    const int r = GetMaxSpellPoints() / 5;

    return 25 >= r ? r : 25;
}
