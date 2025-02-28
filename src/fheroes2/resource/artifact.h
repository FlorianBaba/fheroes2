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
#ifndef H2ARTIFACT_H
#define H2ARTIFACT_H

#include <set>
#include <vector>

#include "interface_itemsbar.h"
#include "mp2.h"
#include "ui_tool.h"

class Spell;
class Heroes;
class StatusBar;
class StreamBase;

class Artifact
{
public:
    enum level_t
    {
        ART_NONE = 0,
        ART_LEVEL1 = 0x01,
        ART_LEVEL2 = 0x02,
        ART_LEVEL3 = 0x04,
        ART_LEVEL123 = ART_LEVEL1 | ART_LEVEL2 | ART_LEVEL3,
        ART_ULTIMATE = 0x08,
        ART_LOYALTY = 0x10,
        ART_NORANDOM = 0x20
    };

    enum type_t
    {
        ULTIMATE_BOOK,
        ULTIMATE_SWORD,
        ULTIMATE_CLOAK,
        ULTIMATE_WAND,
        ULTIMATE_SHIELD,
        ULTIMATE_STAFF,
        ULTIMATE_CROWN,
        GOLDEN_GOOSE,
        ARCANE_NECKLACE,
        CASTER_BRACELET,
        MAGE_RING,
        WITCHES_BROACH,
        MEDAL_VALOR,
        MEDAL_COURAGE,
        MEDAL_HONOR,
        MEDAL_DISTINCTION,
        FIZBIN_MISFORTUNE,
        THUNDER_MACE,
        ARMORED_GAUNTLETS,
        DEFENDER_HELM,
        GIANT_FLAIL,
        BALLISTA,
        STEALTH_SHIELD,
        DRAGON_SWORD,
        POWER_AXE,
        DIVINE_BREASTPLATE,
        MINOR_SCROLL,
        MAJOR_SCROLL,
        SUPERIOR_SCROLL,
        FOREMOST_SCROLL,
        ENDLESS_SACK_GOLD,
        ENDLESS_BAG_GOLD,
        ENDLESS_PURSE_GOLD,
        NOMAD_BOOTS_MOBILITY,
        TRAVELER_BOOTS_MOBILITY,
        RABBIT_FOOT,
        GOLDEN_HORSESHOE,
        GAMBLER_LUCKY_COIN,
        FOUR_LEAF_CLOVER,
        TRUE_COMPASS_MOBILITY,
        SAILORS_ASTROLABE_MOBILITY,
        EVIL_EYE,
        ENCHANTED_HOURGLASS,
        GOLD_WATCH,
        SKULLCAP,
        ICE_CLOAK,
        FIRE_CLOAK,
        LIGHTNING_HELM,
        EVERCOLD_ICICLE,
        EVERHOT_LAVA_ROCK,
        LIGHTNING_ROD,
        SNAKE_RING,
        ANKH,
        BOOK_ELEMENTS,
        ELEMENTAL_RING,
        HOLY_PENDANT,
        PENDANT_FREE_WILL,
        PENDANT_LIFE,
        SERENITY_PENDANT,
        SEEING_EYE_PENDANT,
        KINETIC_PENDANT,
        PENDANT_DEATH,
        WAND_NEGATION,
        GOLDEN_BOW,
        TELESCOPE,
        STATESMAN_QUILL,
        WIZARD_HAT,
        POWER_RING,
        AMMO_CART,
        TAX_LIEN,
        HIDEOUS_MASK,
        ENDLESS_POUCH_SULFUR,
        ENDLESS_VIAL_MERCURY,
        ENDLESS_POUCH_GEMS,
        ENDLESS_CORD_WOOD,
        ENDLESS_CART_ORE,
        ENDLESS_POUCH_CRYSTAL,
        SPIKED_HELM,
        SPIKED_SHIELD,
        WHITE_PEARL,
        BLACK_PEARL,

        MAGIC_BOOK,

        DUMMY1,
        DUMMY2,
        DUMMY3,
        DUMMY4,

        SPELL_SCROLL,
        ARM_MARTYR,
        BREASTPLATE_ANDURAN,
        BROACH_SHIELDING,
        BATTLE_GARB,
        CRYSTAL_BALL,
        HEART_FIRE,
        HEART_ICE,
        HELMET_ANDURAN,
        HOLY_HAMMER,
        LEGENDARY_SCEPTER,
        MASTHEAD,
        SPHERE_NEGATION,
        STAFF_WIZARDRY,
        SWORD_BREAKER,
        SWORD_ANDURAN,
        SPADE_NECROMANCY,

        UNKNOWN
    };

    Artifact( int = UNKNOWN );

    bool operator==( const Spell & ) const;
    bool operator==( const Artifact & ) const;
    bool operator!=( const Artifact & ) const;
    int GetID( void ) const;

    bool isUltimate( void ) const;
    bool isAlchemistRemove( void ) const;
    bool isValid( void ) const;

    void Reset( void );

    u32 ExtraValue( void ) const;
    int Level( void ) const;
    int LoyaltyLevel( void ) const;
    int Type( void ) const;
    int getArtifactValue() const;

    /* objnarti.icn */
    u32 IndexSprite( void ) const;
    /* artfx.icn */
    u32 IndexSprite32( void ) const;
    /* artifact.icn */
    u32 IndexSprite64( void ) const;

    void SetSpell( int );
    int GetSpell( void ) const;

    const char * GetName( void ) const;
    std::string GetDescription( void ) const;

    static int Rand( level_t );
    static Artifact FromMP2IndexSprite( u32 );
    static const char * GetScenario( const Artifact & );

private:
    friend StreamBase & operator<<( StreamBase &, const Artifact & );
    friend StreamBase & operator>>( StreamBase &, Artifact & );

    int id;
    int ext;
};

StreamBase & operator<<( StreamBase &, const Artifact & );
StreamBase & operator>>( StreamBase &, Artifact & );

u32 GoldInsteadArtifact( const MP2::MapObjectType objectType );

namespace fheroes2
{
    void ResetArtifactStats();
    void ExcludeArtifactFromRandom( const int artifactID );
}

struct ArtifactSetData
{
    ArtifactSetData( const uint32_t artifactID, const std::string & assembleMessage );

    uint32_t _assembledArtifactID = Artifact::UNKNOWN;
    std::string _assembleMessage;

    bool operator<( const ArtifactSetData & other ) const;
};

class BagArtifacts : public std::vector<Artifact>
{
public:
    BagArtifacts();

    bool ContainSpell( const Spell & ) const;
    bool isPresentArtifact( const Artifact & ) const;
    bool PushArtifact( const Artifact & );

    void RemoveArtifact( const Artifact & art );

    bool isFull( void ) const;
    bool ContainUltimateArtifact( void ) const;

    void RemoveScroll( const Artifact & );
    void exchangeArtifacts( BagArtifacts & giftBag );

    int getArtifactValue() const;
    u32 CountArtifacts( void ) const;
    u32 Count( const Artifact & ) const;

    std::set<ArtifactSetData> assembleArtifactSetIfPossible();

    std::string String( void ) const;
};

class ArtifactsBar : public Interface::ItemsActionBar<Artifact>
{
public:
    ArtifactsBar( const Heroes * hero, const bool mini, const bool ro, const bool change, const bool allowOpeningMagicBook, StatusBar * bar );

    void RedrawBackground( const fheroes2::Rect &, fheroes2::Image & ) override;
    void RedrawItem( Artifact &, const fheroes2::Rect &, bool, fheroes2::Image & ) override;

    void ResetSelected( void );
    void Redraw( fheroes2::Image & dstsf = fheroes2::Display::instance() );

    bool ActionBarLeftMouseSingleClick( Artifact & artifact ) override;
    bool ActionBarLeftMouseSingleClick( Artifact & artifact1, Artifact & artifact2 ) override;
    bool ActionBarLeftMouseDoubleClick( Artifact & artifact ) override;
    bool ActionBarRightMouseHold( Artifact & artifact ) override;

    bool QueueEventProcessing( std::string * = nullptr );
    bool QueueEventProcessing( ArtifactsBar &, std::string * = nullptr );

    bool ActionBarCursor( Artifact & ) override;
    bool ActionBarCursor( Artifact &, Artifact & ) override;

protected:
    fheroes2::MovableSprite spcursor;

private:
    const Heroes * _hero;
    fheroes2::Image backsf;
    const bool use_mini_sprite;
    const bool read_only;
    const bool can_change;
    const bool _allowOpeningMagicBook;
    StatusBar * _statusBar;
    std::string msg;

    static bool isMagicBook( const Artifact & artifact );

    void messageMagicBookAbortTrading() const;
};

#endif
