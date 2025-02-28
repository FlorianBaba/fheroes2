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

#include "spell.h"
#include "artifact.h"
#include "heroes_base.h"
#include "race.h"
#include "rand.h"
#include "resource.h"
#include "serialize.h"
#include "translations.h"

struct spellstats_t
{
    const char * name;
    uint8_t spellPoints;
    uint16_t movePoints;
    uint32_t imageId;
    uint8_t extraValue;
    const char * description;
};

// The original resources don't have most of sprites for Mass Spells
// so we made some tricks in AGG source file. All modified sprite IDs start from 60

spellstats_t spells[] = {
    //  name | sp | mp | spr | value| bits | description
    { "Unknown", 0, 0, 0, 0, "Unknown spell." },
    { gettext_noop( "Fireball" ), 9, 0, 8, 10, gettext_noop( "Causes a giant fireball to strike the selected area, damaging all nearby creatures." ) },
    { gettext_noop( "Fireblast" ), 15, 0, 9, 10,
      gettext_noop( "An improved version of fireball, fireblast affects two hexes around the center point of the spell, rather than one." ) },
    { gettext_noop( "Lightning Bolt" ), 7, 0, 4, 25, gettext_noop( "Causes a bolt of electrical energy to strike the selected creature." ) },
    { gettext_noop( "Chain Lightning" ), 15, 0, 5, 40,
      gettext_noop(
          "Causes a bolt of electrical energy to strike a selected creature, then strike the nearest creature with half damage, then strike the NEXT nearest creature with half again damage, and so on, until it becomes too weak to be harmful.  Warning:  This spell can hit your own creatures!" ) },
    { gettext_noop( "Teleport" ), 9, 0, 10, 0, gettext_noop( "Teleports the creature you select to any open position on the battlefield." ) },
    { gettext_noop( "Cure" ), 6, 0, 6, 5,
      gettext_noop( "Removes all negative spells cast upon one of your units, and restores up to %{count} HP per level of spell power." ) },
    { gettext_noop( "Mass Cure" ), 15, 0, 60, 5,
      gettext_noop( "Removes all negative spells cast upon your forces, and restores up to %{count} HP per level of spell power, per creature." ) },
    { gettext_noop( "Resurrect" ), 12, 0, 13, 50, gettext_noop( "Resurrects creatures from a damaged or dead unit until end of combat." ) },
    { gettext_noop( "Resurrect True" ), 15, 0, 12, 50, gettext_noop( "Resurrects creatures from a damaged or dead unit permanently." ) },
    { gettext_noop( "Haste" ), 3, 0, 14, 0, gettext_noop( "Increases the speed of any creature by %{count}." ) },
    { gettext_noop( "Mass Haste" ), 10, 0, 61, 0, gettext_noop( "Increases the speed of all of your creatures by %{count}." ) },
    { gettext_noop( "spell|Slow" ), 3, 0, 1, 0, gettext_noop( "Slows target to half movement rate." ) },
    { gettext_noop( "Mass Slow" ), 15, 0, 62, 0, gettext_noop( "Slows all enemies to half movement rate." ) },
    { gettext_noop( "spell|Blind" ), 6, 0, 21, 0, gettext_noop( "Clouds the affected creatures' eyes, preventing them from moving." ) },
    { gettext_noop( "Bless" ), 3, 0, 7, 0, gettext_noop( "Causes the selected creatures to inflict maximum damage." ) },
    { gettext_noop( "Mass Bless" ), 12, 0, 63, 0, gettext_noop( "Causes all of your units to inflict maximum damage." ) },
    { gettext_noop( "Stoneskin" ), 3, 0, 31, 3, gettext_noop( "Magically increases the defense skill of the selected creatures." ) },
    { gettext_noop( "Steelskin" ), 6, 0, 30, 5, gettext_noop( "Increases the defense skill of the targeted creatures.  This is an improved version of Stoneskin." ) },
    { gettext_noop( "Curse" ), 3, 0, 3, 0, gettext_noop( "Causes the selected creatures to inflict minimum damage." ) },
    { gettext_noop( "Mass Curse" ), 12, 0, 64, 0, gettext_noop( "Causes all enemy troops to inflict minimum damage." ) },
    { gettext_noop( "Holy Word" ), 9, 0, 22, 10, gettext_noop( "Damages all undead in the battle." ) },
    { gettext_noop( "Holy Shout" ), 12, 0, 23, 20, gettext_noop( "Damages all undead in the battle.  This is an improved version of Holy Word." ) },
    { gettext_noop( "Anti-Magic" ), 7, 0, 17, 0, gettext_noop( "Prevents harmful magic against the selected creatures." ) },
    { gettext_noop( "Dispel Magic" ), 5, 0, 18, 0, gettext_noop( "Removes all magic spells from a single target." ) },
    { gettext_noop( "Mass Dispel" ), 12, 0, 18, 0, gettext_noop( "Removes all magic spells from all creatures." ) },
    { gettext_noop( "Magic Arrow" ), 3, 0, 38, 10, gettext_noop( "Causes a magic arrow to strike the selected target." ) },
    { gettext_noop( "Berserker" ), 12, 0, 19, 0, gettext_noop( "Causes a creature to attack its nearest neighbor." ) },
    { gettext_noop( "Armageddon" ), 20, 0, 16, 50, gettext_noop( "Holy terror strikes the battlefield, causing severe damage to all creatures." ) },
    { gettext_noop( "Elemental Storm" ), 15, 0, 11, 25, gettext_noop( "Magical elements pour down on the battlefield, damaging all creatures." ) },
    { gettext_noop( "Meteor Shower" ), 15, 0, 24, 25, gettext_noop( "A rain of rocks strikes an area of the battlefield, damaging all nearby creatures." ) },
    { gettext_noop( "Paralyze" ), 9, 0, 20, 0, gettext_noop( "The targeted creatures are paralyzed, unable to move or retaliate." ) },
    { gettext_noop( "Hypnotize" ), 15, 0, 37, 25,
      gettext_noop( "Brings a single enemy unit under your control if its hits are less than %{count} times the caster's spell power." ) },
    { gettext_noop( "Cold Ray" ), 6, 0, 36, 20, gettext_noop( "Drains body heat from a single enemy unit." ) },
    { gettext_noop( "Cold Ring" ), 9, 0, 35, 10, gettext_noop( "Drains body heat from all units surrounding the center point, but not including the center point." ) },
    { gettext_noop( "Disrupting Ray" ), 7, 0, 34, 3, gettext_noop( "Reduces the defense rating of an enemy unit by three." ) },
    { gettext_noop( "Death Ripple" ), 6, 0, 28, 5, gettext_noop( "Damages all living (non-undead) units in the battle." ) },
    { gettext_noop( "Death Wave" ), 10, 0, 29, 10,
      gettext_noop( "Damages all living (non-undead) units in the battle.  This spell is an improved version of Death Ripple." ) },
    { gettext_noop( "Dragon Slayer" ), 6, 0, 32, 5, gettext_noop( "Greatly increases a unit's attack skill vs. Dragons." ) },
    { gettext_noop( "Blood Lust" ), 3, 0, 27, 3, gettext_noop( "Increases a unit's attack skill." ) },
    { gettext_noop( "Animate Dead" ), 10, 0, 25, 50, gettext_noop( "Resurrects creatures from a damaged or dead undead unit permanently." ) },
    { gettext_noop( "Mirror Image" ), 25, 0, 26, 0,
      gettext_noop(
          "Creates an illusionary unit that duplicates one of your existing units.  This illusionary unit does the same damages as the original, but will vanish if it takes any damage." ) },
    { gettext_noop( "Shield" ), 3, 0, 15, 2, gettext_noop( "Halves damage received from ranged attacks for a single unit." ) },
    { gettext_noop( "Mass Shield" ), 7, 0, 65, 0, gettext_noop( "Halves damage received from ranged attacks for all of your units." ) },
    { gettext_noop( "Summon Earth Elemental" ), 30, 0, 56, 3, gettext_noop( "Summons Earth Elementals to fight for your army." ) },
    { gettext_noop( "Summon Air Elemental" ), 30, 0, 57, 3, gettext_noop( "Summons Air Elementals to fight for your army." ) },
    { gettext_noop( "Summon Fire Elemental" ), 30, 0, 58, 3, gettext_noop( "Summons Fire Elementals to fight for your army." ) },
    { gettext_noop( "Summon Water Elemental" ), 30, 0, 59, 3, gettext_noop( "Summons Water Elementals to fight for your army." ) },
    { gettext_noop( "Earthquake" ), 15, 0, 33, 0, gettext_noop( "Damages castle walls." ) },
    { gettext_noop( "View Mines" ), 1, 0, 39, 0, gettext_noop( "Causes all mines across the land to become visible." ) },
    { gettext_noop( "View Resources" ), 1, 0, 40, 0, gettext_noop( "Causes all resources across the land to become visible." ) },
    { gettext_noop( "View Artifacts" ), 2, 0, 41, 0, gettext_noop( "Causes all artifacts across the land to become visible." ) },
    { gettext_noop( "View Towns" ), 2, 0, 42, 0, gettext_noop( "Causes all towns and castles across the land to become visible." ) },
    { gettext_noop( "View Heroes" ), 2, 0, 43, 0, gettext_noop( "Causes all Heroes across the land to become visible." ) },
    { gettext_noop( "View All" ), 3, 0, 44, 0, gettext_noop( "Causes the entire land to become visible." ) },
    { gettext_noop( "Identify Hero" ), 3, 0, 45, 0, gettext_noop( "Allows the caster to view detailed information on enemy Heroes." ) },
    { gettext_noop( "Summon Boat" ), 5, 0, 46, 0,
      gettext_noop(
          "Summons the nearest unoccupied, friendly boat to an adjacent shore location.  A friendly boat is one which you just built or were the most recent player to occupy." ) },
    { gettext_noop( "Dimension Door" ), 10, 0, 47, 0, gettext_noop( "Allows the caster to magically transport to a nearby location." ) },
    { gettext_noop( "Town Gate" ), 10, 0, 48, 0, gettext_noop( "Returns the caster to any town or castle currently owned." ) },
    { gettext_noop( "Town Portal" ), 20, 0, 49, 0, gettext_noop( "Returns the hero to the town or castle of choice, provided it is controlled by you." ) },
    { gettext_noop( "Visions" ), 6, 0, 50, 3, gettext_noop( "Visions predicts the likely outcome of an encounter with a neutral army camp." ) },
    { gettext_noop( "Haunt" ), 8, 0, 51, 4,
      gettext_noop( "Haunts a mine you control with Ghosts.  This mine stops producing resources.  (If I can't keep it, nobody will!)" ) },
    { gettext_noop( "Set Earth Guardian" ), 15, 0, 52, 4, gettext_noop( "Sets Earth Elementals to guard a mine against enemy armies." ) },
    { gettext_noop( "Set Air Guardian" ), 15, 0, 53, 4, gettext_noop( "Sets Air Elementals to guard a mine against enemy armies." ) },
    { gettext_noop( "Set Fire Guardian" ), 15, 0, 54, 4, gettext_noop( "Sets Fire Elementals to guard a mine against enemy armies." ) },
    { gettext_noop( "Set Water Guardian" ), 15, 0, 55, 4, gettext_noop( "Sets Water Elementals to guard a mine against enemy armies." ) },
    { "Random", 1, 0, 0, 0, "Random" },
    { "Random 1", 1, 0, 0, 0, "Random 1" },
    { "Random 2", 1, 0, 0, 0, "Random 2" },
    { "Random 3", 1, 0, 0, 0, "Random 3" },
    { "Random 4", 1, 0, 0, 0, "Random 4" },
    { "Random 5", 1, 0, 0, 0, "Random 5" },
    { gettext_noop( "Petrification" ), 1, 0, 0, 0,
      gettext_noop( "Turns the affected creature into stone.  A petrified creature receives half damage from a direct attack." ) },
};

Spell::Spell( int s )
    : id( s > STONE ? NONE : s )
{}

bool Spell::operator<( const Spell & s ) const
{
    return id < s.id;
}

bool Spell::operator==( const Spell & s ) const
{
    return s.id == id;
}

bool Spell::operator!=( const Spell & s ) const
{
    return s.id != id;
}

bool Spell::isValid( void ) const
{
    return id != Spell::NONE;
}

int Spell::GetID( void ) const
{
    return id;
}

const char * Spell::GetName( void ) const
{
    return _( spells[id].name );
}

const char * Spell::GetDescription( void ) const
{
    return _( spells[id].description );
}

u32 Spell::MovePoint( void ) const
{
    return spells[id].movePoints;
}

u32 Spell::SpellPoint( const HeroBase * hero ) const
{
    u32 res = spells[id].spellPoints;
    u32 acount = 0;

    if ( hero ) {
        switch ( id ) {
        case BLESS:
        case MASSBLESS:
            acount = hero->artifactCount( Artifact::SNAKE_RING );
            if ( acount )
                res = spells[id].spellPoints / ( acount * 2 );
            break;

        case SUMMONEELEMENT:
        case SUMMONAELEMENT:
        case SUMMONFELEMENT:
        case SUMMONWELEMENT:
            acount = hero->artifactCount( Artifact::ELEMENTAL_RING );
            if ( acount )
                res = spells[id].spellPoints / ( acount * 2 );
            break;

        case CURSE:
        case MASSCURSE:
            acount = hero->artifactCount( Artifact::EVIL_EYE );
            if ( acount )
                res = spells[id].spellPoints / ( acount * 2 );
            break;

        default:
            break;
        }

        if ( isMindInfluence() ) {
            acount = hero->artifactCount( Artifact::SKULLCAP );
            if ( acount )
                res = spells[id].spellPoints / ( acount * 2 );
        }
    }

    return res ? res : 1;
}

bool Spell::isLevel( int lvl ) const
{
    return Level() == lvl;
}

int Spell::Level( void ) const
{
    switch ( id ) {
    case BLESS:
    case BLOODLUST:
    case CURE:
    case CURSE:
    case DISPEL:
    case HASTE:
    case ARROW:
    case SHIELD:
    case SLOW:
    case STONESKIN:

    case VIEWMINES:
    case VIEWRESOURCES:
        return 1;

    case BLIND:
    case COLDRAY:
    case DEATHRIPPLE:
    case DISRUPTINGRAY:
    case DRAGONSLAYER:
    case LIGHTNINGBOLT:
    case STEELSKIN:

    case HAUNT:
    case SUMMONBOAT:
    case VIEWARTIFACTS:
    case VISIONS:
        return 2;

    case ANIMATEDEAD:
    case ANTIMAGIC:
    case COLDRING:
    case DEATHWAVE:
    case EARTHQUAKE:
    case FIREBALL:
    case HOLYWORD:
    case MASSBLESS:
    case MASSCURSE:
    case MASSDISPEL:
    case MASSHASTE:
    case PARALYZE:
    case TELEPORT:

    case IDENTIFYHERO:
    case VIEWHEROES:
    case VIEWTOWNS:
        return 3;

    case BERSERKER:
    case CHAINLIGHTNING:
    case ELEMENTALSTORM:
    case FIREBLAST:
    case HOLYSHOUT:
    case MASSCURE:
    case MASSSHIELD:
    case MASSSLOW:
    case METEORSHOWER:
    case RESURRECT:

    case SETEGUARDIAN:
    case SETAGUARDIAN:
    case SETFGUARDIAN:
    case SETWGUARDIAN:
    case TOWNGATE:
    case VIEWALL:
        return 4;

    case ARMAGEDDON:
    case HYPNOTIZE:
    case MIRRORIMAGE:
    case RESURRECTTRUE:
    case SUMMONEELEMENT:
    case SUMMONAELEMENT:
    case SUMMONFELEMENT:
    case SUMMONWELEMENT:

    case DIMENSIONDOOR:
    case TOWNPORTAL:
        return 5;

    default:
        break;
    }

    return 0;
}

bool Spell::isCombat( void ) const
{
    switch ( id ) {
    case NONE:
    case VIEWMINES:
    case VIEWRESOURCES:
    case VIEWARTIFACTS:
    case VIEWTOWNS:
    case VIEWHEROES:
    case VIEWALL:
    case IDENTIFYHERO:
    case SUMMONBOAT:
    case DIMENSIONDOOR:
    case TOWNGATE:
    case TOWNPORTAL:
    case VISIONS:
    case HAUNT:
    case SETEGUARDIAN:
    case SETAGUARDIAN:
    case SETFGUARDIAN:
    case SETWGUARDIAN:
        return false;
    default:
        break;
    }
    return true;
}

bool Spell::isFire() const
{
    return id == FIREBALL || id == FIREBLAST;
}

bool Spell::isCold() const
{
    return id == COLDRAY || id == COLDRING;
}

bool Spell::isGuardianType() const
{
    switch ( id ) {
    case HAUNT:
    case SETEGUARDIAN:
    case SETAGUARDIAN:
    case SETFGUARDIAN:
    case SETWGUARDIAN:
        return true;
    default:
        break;
    }

    return false;
}

bool Spell::isAdventure( void ) const
{
    return !isCombat();
}

bool Spell::isDamage( void ) const
{
    return Damage() != 0;
}

u32 Spell::Damage( void ) const
{
    switch ( id ) {
    case ARROW:
    case FIREBALL:
    case FIREBLAST:
    case LIGHTNINGBOLT:
    case COLDRING:
    case DEATHWAVE:
    case HOLYWORD:
    case CHAINLIGHTNING:
    case ARMAGEDDON:
    case ELEMENTALSTORM:
    case METEORSHOWER:
    case COLDRAY:
    case HOLYSHOUT:
    case DEATHRIPPLE:
        return spells[id].extraValue;

    default:
        break;
    }

    return 0;
}

bool Spell::isMindInfluence( void ) const
{
    switch ( id ) {
    case BLIND:
    case PARALYZE:
    case BERSERKER:
    case HYPNOTIZE:
        return true;

    default:
        break;
    }

    return false;
}

u32 Spell::IndexSprite( void ) const
{
    return spells[id].imageId;
}

u32 Spell::Restore( void ) const
{
    switch ( id ) {
    case Spell::CURE:
    case Spell::MASSCURE:
        return spells[id].extraValue;

    default:
        break;
    }

    return Resurrect();
}

u32 Spell::Resurrect( void ) const
{
    switch ( id ) {
    case Spell::ANIMATEDEAD:
    case Spell::RESURRECT:
    case Spell::RESURRECTTRUE:
        return spells[id].extraValue;

    default:
        break;
    }

    return 0;
}

bool Spell::isRestore( void ) const
{
    return Restore() != 0;
}

bool Spell::isResurrect( void ) const
{
    return Resurrect() != 0;
}

u32 Spell::ExtraValue( void ) const
{
    return spells[id].extraValue;
}

Spell Spell::Rand( int lvl, bool adv )
{
    std::vector<Spell> v;
    v.reserve( 15 );

    for ( u32 sp = NONE; sp < STONE; ++sp ) {
        const Spell spell( sp );
        if ( ( ( adv && !spell.isCombat() ) || ( !adv && spell.isCombat() ) ) && lvl == spell.Level() )
            v.push_back( spell );
    }
    return !v.empty() ? Rand::Get( v ) : Spell( Spell::NONE );
}

Spell Spell::RandCombat( int lvl )
{
    return Rand( lvl, false );
}

Spell Spell::RandAdventure( int lvl )
{
    Spell res = Rand( lvl, true );
    return res.isValid() ? res : RandCombat( lvl );
}

bool Spell::isUndeadOnly( void ) const
{
    switch ( id ) {
    case ANIMATEDEAD:
    case HOLYWORD:
    case HOLYSHOUT:
        return true;

    default:
        break;
    }

    return false;
}

bool Spell::isALiveOnly( void ) const
{
    switch ( id ) {
    case BLESS:
    case MASSBLESS:
    case CURSE:
    case MASSCURSE:
    case DEATHRIPPLE:
    case DEATHWAVE:
    case RESURRECT:
    case RESURRECTTRUE:
        return true;

    default:
        break;
    }

    return false;
}

bool Spell::isSingleTarget() const
{
    switch ( id ) {
    case LIGHTNINGBOLT:
    case TELEPORT:
    case CURE:
    case RESURRECT:
    case RESURRECTTRUE:
    case HASTE:
    case SLOW:
    case BLIND:
    case BLESS:
    case STONESKIN:
    case STEELSKIN:
    case CURSE:
    case ANTIMAGIC:
    case DISPEL:
    case ARROW:
    case BERSERKER:
    case PARALYZE:
    case HYPNOTIZE:
    case COLDRAY:
    case DISRUPTINGRAY:
    case DRAGONSLAYER:
    case BLOODLUST:
    case ANIMATEDEAD:
    case MIRRORIMAGE:
    case SHIELD:
        return true;
    }

    return false;
}

bool Spell::isApplyWithoutFocusObject( void ) const
{
    if ( isMassActions() || isSummon() )
        return true;
    else
        switch ( id ) {
        case DEATHRIPPLE:
        case DEATHWAVE:
        case EARTHQUAKE:
        case HOLYWORD:
        case HOLYSHOUT:
        case ARMAGEDDON:
        case ELEMENTALSTORM:
            return true;

        default:
            break;
        }

    return false;
}

bool Spell::isSummon( void ) const
{
    switch ( id ) {
    case SUMMONEELEMENT:
    case SUMMONAELEMENT:
    case SUMMONFELEMENT:
    case SUMMONWELEMENT:
        return true;

    default:
        break;
    }

    return false;
}

bool Spell::isEffectDispel() const
{
    switch ( id ) {
    case CURE:
    case MASSCURE:
    case DISPEL:
    case MASSDISPEL:
        return true;

    default:
        break;
    }

    return false;
}

bool Spell::isApplyToAnyTroops( void ) const
{
    switch ( id ) {
    case DISPEL:
    case MASSDISPEL:
        return true;

    default:
        break;
    }

    return false;
}

bool Spell::isApplyToFriends( void ) const
{
    switch ( id ) {
    case BLESS:
    case BLOODLUST:
    case CURE:
    case HASTE:
    case SHIELD:
    case STONESKIN:
    case DRAGONSLAYER:
    case STEELSKIN:
    case ANIMATEDEAD:
    case ANTIMAGIC:
    case TELEPORT:
    case RESURRECT:
    case MIRRORIMAGE:
    case RESURRECTTRUE:

    case MASSBLESS:
    case MASSCURE:
    case MASSHASTE:
    case MASSSHIELD:
        return true;

    default:
        break;
    }

    return false;
}

bool Spell::isMassActions( void ) const
{
    switch ( id ) {
    case MASSCURE:
    case MASSHASTE:
    case MASSSLOW:
    case MASSBLESS:
    case MASSCURSE:
    case MASSDISPEL:
    case MASSSHIELD:
        return true;

    default:
        break;
    }

    return false;
}

bool Spell::isApplyToEnemies( void ) const
{
    switch ( id ) {
    case MASSSLOW:
    case MASSCURSE:

    case CURSE:
    case ARROW:
    case SLOW:
    case BLIND:
    case COLDRAY:
    case DISRUPTINGRAY:
    case LIGHTNINGBOLT:
    case CHAINLIGHTNING:
    case PARALYZE:
    case BERSERKER:
    case HYPNOTIZE:
        return true;

    default:
        break;
    }

    return false;
}

bool Spell::isRaceCompatible( int race ) const
{
    switch ( id ) {
    case MASSCURE:
    case MASSBLESS:
    case HOLYSHOUT:
    case HOLYWORD:
    case BLESS:
    case CURE:
        if ( Race::NECR == race )
            return false;
        break;

    case DEATHWAVE:
    case DEATHRIPPLE:
    case ANIMATEDEAD:
        if ( Race::NECR != race )
            return false;
        break;

    default:
        break;
    }

    return true;
}

int32_t Spell::CalculateDimensionDoorDistance()
{
    // original h2 variant
    return 14;
}

StreamBase & operator<<( StreamBase & msg, const Spell & spell )
{
    return msg << spell.id;
}

StreamBase & operator>>( StreamBase & msg, Spell & spell )
{
    return msg >> spell.id;
}
