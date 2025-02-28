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

#ifndef H2HEROES_H
#define H2HEROES_H

#include <algorithm>
#include <list>
#include <string>
#include <vector>

#include "army.h"
#include "heroes_base.h"
#include "pairs.h"
#include "route.h"
#include "visit.h"

namespace Battle
{
    class Only;
}

namespace Interface
{
    class GameArea;
}

class StreamBuf;

struct HeroSeedsForLevelUp
{
    uint32_t seedPrimarySkill = 0;
    uint32_t seedSecondaySkill1 = 0;
    uint32_t seedSecondaySkill2 = 0;
    uint32_t seedSecondaySkillRandomChoose = 0;
};

class Heroes : public HeroBase, public ColorBase
{
public:
    enum
    {
        // knight
        LORDKILBURN,
        SIRGALLANTH,
        ECTOR,
        GVENNETH,
        TYRO,
        AMBROSE,
        RUBY,
        MAXIMUS,
        DIMITRY,
        // barbarian
        THUNDAX,
        FINEOUS,
        JOJOSH,
        CRAGHACK,
        JEZEBEL,
        JACLYN,
        ERGON,
        TSABU,
        ATLAS,
        // sorceress
        ASTRA,
        NATASHA,
        TROYAN,
        VATAWNA,
        REBECCA,
        GEM,
        ARIEL,
        CARLAWN,
        LUNA,
        // warlock
        ARIE,
        ALAMAR,
        VESPER,
        CRODO,
        BAROK,
        KASTORE,
        AGAR,
        FALAGAR,
        WRATHMONT,
        // wizard
        MYRA,
        FLINT,
        DAWN,
        HALON,
        MYRINI,
        WILFREY,
        SARAKIN,
        KALINDRA,
        MANDIGAL,
        // necromancer
        ZOM,
        DARLANA,
        ZAM,
        RANLOO,
        CHARITY,
        RIALDO,
        ROXANA,
        SANDRO,
        CELIA,
        // From The Succession Wars campaign.
        ROLAND,
        CORLAGON,
        ELIZA,
        ARCHIBALD,
        HALTON,
        BAX,
        // From The Price of Loyalty expansion.
        SOLMYR,
        DAINWIN,
        MOG,
        UNCLEIVAN,
        JOSEPH,
        GALLAVANT,
        ELDERIAN,
        CEALLACH,
        DRAKONIA,
        MARTINE,
        JARKONAS,
        // debugger
        DEBUG_HERO,
        UNKNOWN
    };

    static const fheroes2::Sprite & GetPortrait( int heroid, int type );
    static const char * GetName( int heroid );

    enum flags_t : uint32_t
    {
        SHIPMASTER = 0x00000001,
        // UNUSED = 0x00000002,
        SPELLCASTED = 0x00000004,
        ENABLEMOVE = 0x00000008,
        // UNUSED = 0x00000010,
        // UNUSED = 0x00000020,
        RECRUIT = 0x00000040, // Hero is available for recruitment in any kingdom
        JAIL = 0x00000080,
        ACTION = 0x00000100,
        SAVE_MP_POINTS = 0x00000200,
        SLEEPER = 0x00000400,
        GUARDIAN = 0x00000800,
        NOTDEFAULTS = 0x00001000,
        NOTDISMISS = 0x00002000,
        VISIONS = 0x00004000,
        PATROL = 0x00008000,
        CUSTOMARMY = 0x00010000,
        CUSTOMSKILLS = 0x00020000,
        SKIPPED_TURN = 0x00040000,
        WAITING = 0x00080000,
        MOVED = 0x00100000
    };

    // Types of heroes. Used only for AI as humans are smart enough to manage heroes by themselves.
    enum class Role : int
    {
        // The most ordinary hero's role with no any specialization. This type does eveything what a hero can do:
        // collecting resources, fighting (mostly weak) monsters, claiming towns and mines and expanding the visible territory.
        HUNTER,

        // The type of hero with a skew towards fights. His main priority is to kill monsters and enemies, capture castles and guarded mines.
        // This type still can capture valuable mines or dwellings if they're on the way to something better.
        FIGHTER,

        // The main goal for Scout is to discover new areas so he should run towards the fog of war to expand the visible territory.
        // These heroes usually appear when either no tasks exist on the map or when AI has too many heroes.
        SCOUT,

        // Courier's life is to deliver things from one place to another. Usually they bring an army for Fighters or Champions from
        // dwellings, castles or from one hero to another.
        COURIER,

        // The mightiest hero among others. The main purpose of this type is to run over the enemy's territory and defeat all heroes there while
        // capturing all castles and towns. This type of hero is set when one (or few) heroes are too strong in comparison to others.
        CHAMPION
    };

    struct RedrawIndex
    {
        int32_t topOnBottom = -1;
        int32_t topOnDirectionBottom = -1;
        int32_t topOnDirection = -1;
        int32_t objectsOnBottom = -1;
        int32_t objectsOnDirectionBottom = -1;
    };

    Heroes();
    Heroes( int heroid, int rc );
    Heroes( int heroID, int race, int initialLevel );

    bool isValid() const override;
    bool isFreeman( void ) const;
    void SetFreeman( int reason );

    bool isLosingGame() const;
    const Castle * inCastle() const override;
    Castle * inCastleMutable() const;

    void LoadFromMP2( s32 map_index, int cl, int rc, StreamBuf );
    void PostLoad( void );

    int GetRace() const override;
    const std::string & GetName() const override;
    int GetColor() const override;
    int GetType() const override;
    int GetControl() const override;

    int GetKillerColor( void ) const;
    void SetKillerColor( int );

    const Army & GetArmy() const override;
    Army & GetArmy() override;

    int GetID( void ) const;

    double getMeetingValue( const Heroes & otherHero ) const;
    double getRecruitValue() const;
    int getStatsValue() const;

    int GetAttack() const override;
    int GetDefense() const override;
    int GetPower() const override;
    int GetKnowledge() const override;

    int GetAttack( std::string * ) const;
    int GetDefense( std::string * ) const;
    int GetPower( std::string * ) const;
    int GetKnowledge( std::string * ) const;

    void IncreasePrimarySkill( int skill );

    int GetMorale() const override;
    int GetLuck() const override;
    int GetMoraleWithModificators( std::string * str = nullptr ) const;
    int GetLuckWithModificators( std::string * str = nullptr ) const;
    int GetLevel( void ) const;

    MP2::MapObjectType GetMapsObject() const;
    void SetMapsObject( const MP2::MapObjectType objectType );

    const fheroes2::Point & GetCenterPatrol( void ) const;
    void SetCenterPatrol( const fheroes2::Point & );
    int GetSquarePatrol( void ) const;

    u32 GetMaxSpellPoints() const override;
    u32 GetMaxMovePoints() const;

    u32 GetMovePoints( void ) const;
    void IncreaseMovePoints( u32 );
    bool MayStillMove( const bool ignorePath, const bool ignoreSleeper ) const;
    void ResetMovePoints( void );
    void MovePointsScaleFixed( void );

    bool HasSecondarySkill( int ) const;
    bool HasMaxSecondarySkill( void ) const;
    int GetLevelSkill( int ) const override;
    u32 GetSecondaryValues( int ) const override;
    void LearnSkill( const Skill::Secondary & );
    Skill::SecSkills & GetSecondarySkills( void );

    bool PickupArtifact( const Artifact & );
    bool HasUltimateArtifact( void ) const;
    u32 GetCountArtifacts( void ) const;
    bool IsFullBagArtifacts( void ) const;

    int GetMobilityIndexSprite( void ) const;

    // Returns the relative height of mana column near hero's portrait in heroes panel. Returned value will be in range [0; 25].
    int GetManaIndexSprite( void ) const;

    int OpenDialog( bool readonly = false, bool fade = false, bool disableDismiss = false, bool disableSwitch = false );
    void MeetingDialog( Heroes & );

    bool Recruit( const int col, const fheroes2::Point & pt );
    bool Recruit( const Castle & castle );

    void ActionNewDay( void );
    void ActionNewWeek( void );
    void ActionNewMonth( void );
    void ActionAfterBattle() override;
    void ActionPreBattle() override;

    bool BuySpellBook( const Castle *, int shrine = 0 );

    const Route::Path & GetPath() const;
    Route::Path & GetPath();
    int GetRangeRouteDays( s32 ) const;
    void ShowPath( bool );
    // Calculates the hero's path to the tile with the dstIdx index using the pathfinder from the World global object.
    // Recalculates the existing path if dstIdx is negative. Not applicable if you want to use a pathfinder other than
    // PlayerWorldPathfinder.
    void calculatePath( int32_t dstIdx );

    int GetDirection() const;
    void setDirection( int directionToSet );

    // set visited cell
    void SetVisited( s32, Visit::type_t = Visit::LOCAL );

    // Set global visited state for itself and for allies.
    void setVisitedForAllies( const int32_t tileIndex ) const;

    void SetVisitedWideTile( s32, const MP2::MapObjectType objectType, Visit::type_t = Visit::LOCAL );
    bool isObjectTypeVisited( const MP2::MapObjectType object, Visit::type_t = Visit::LOCAL ) const;
    bool isVisited( const Maps::Tiles &, Visit::type_t = Visit::LOCAL ) const;

    // These methods are used only for AI.
    bool hasMetWithHero( int heroID ) const;
    void markHeroMeeting( int heroID );
    void unmarkHeroMeeting();

    bool Move( bool fast = false );
    void Move2Dest( const int32_t destination );
    bool isMoveEnabled( void ) const;
    bool CanMove( void ) const;
    void SetMove( bool );
    bool isAction( void ) const;
    void ResetAction( void );
    void Action( int tileIndex, bool isDestination );
    void ActionNewPosition( const bool allowMonsterAttack );
    void ApplyPenaltyMovement( uint32_t penalty );
    bool ActionSpellCast( const Spell & );

    bool MayCastAdventureSpells() const;

    const RedrawIndex & GetRedrawIndex() const;
    void SetRedrawIndexes();
    void UpdateRedrawTop( const Maps::Tiles & tile );
    void UpdateRedrawBottom( const Maps::Tiles & tile );
    void RedrawTop( fheroes2::Image & dst, const fheroes2::Rect & visibleTileROI, const Interface::GameArea & area ) const;
    void RedrawBottom( fheroes2::Image & dst, const fheroes2::Rect & visibleTileROI, const Interface::GameArea & area, bool isPuzzleDraw ) const;
    void Redraw( fheroes2::Image & dst, const int32_t dx, int32_t dy, const fheroes2::Rect & visibleTileROI, const Interface::GameArea & area ) const;
    void RedrawShadow( fheroes2::Image & dst, const int32_t dx, int32_t dy, const fheroes2::Rect & visibleTileROI, const Interface::GameArea & area ) const;

    void PortraitRedraw( const int32_t px, const int32_t py, const PortraitType type, fheroes2::Image & dstsf ) const override;
    int GetSpriteIndex( void ) const;

    // These 2 methods must be used only for hero's animation. Please never use them anywhere else!
    void SetSpriteIndex( int index );
    void SetOffset( const fheroes2::Point & offset );

    void FadeOut( const fheroes2::Point & offset = fheroes2::Point() ) const;
    void FadeIn( const fheroes2::Point & offset = fheroes2::Point() ) const;
    void Scoute( const int tileIndex ) const;
    int GetScoute( void ) const;
    u32 GetVisionsDistance( void ) const;

    bool isShipMaster( void ) const;
    void SetShipMaster( bool );
    uint32_t lastGroundRegion() const;
    void setLastGroundRegion( uint32_t regionID );

    u32 GetExperience( void ) const;
    void IncreaseExperience( const uint32_t amount, const bool autoselect = false );

    std::string String( void ) const;
    const fheroes2::Sprite & GetPortrait( int type ) const;

    static int GetLevelFromExperience( u32 );
    static u32 GetExperienceFromLevel( int );

    static void ScholarAction( Heroes &, Heroes & );

    fheroes2::Point MovementDirection() const;

    int GetAttackedMonsterTileIndex() const;
    void SetAttackedMonsterTileIndex( int idx );

    void setAIRole( const Role role )
    {
        _aiRole = role;
    }

    Role getAIRole() const
    {
        return _aiRole;
    }

private:
    friend StreamBase & operator<<( StreamBase &, const Heroes & );
    friend StreamBase & operator>>( StreamBase &, Heroes & );

    friend class Recruits;
    friend class Battle::Only;

    HeroSeedsForLevelUp GetSeedsForLevelUp() const;
    void LevelUp( bool skipsecondary, bool autoselect = false );
    void LevelUpSecondarySkill( const HeroSeedsForLevelUp & seeds, int primary, bool autoselect = false );
    void AngleStep( int );
    bool MoveStep( bool fast = false );
    static void MoveStep( Heroes &, s32 to, bool newpos );
    static uint32_t GetStartingXp();
    bool isInVisibleMapArea() const;

    // This function is useful only in a situation when AI hero moves out of the fog
    // we don't update his direction during movement under the fog so there is a situation
    // when initial hero's sprite is set incorrectly. This function fixes it
    void SetValidDirectionSprite();

    uint32_t UpdateMovementPoints( const uint32_t movePoints, const int skill ) const;

    // Daily replenishment of spell points
    void ReplenishSpellPoints();

    bool isInDeepOcean() const;

    enum
    {
        SKILL_VALUE = 100
    };

    std::string name;
    ColorBase killer_color;
    u32 experience;
    s32 move_point_scale;

    Skill::SecSkills secondary_skills;

    Army army;

    int hid; /* hero id */
    int portrait; /* hero id */
    int _race;
    int save_maps_object;

    Route::Path path;

    int direction;
    int sprite_index;
    fheroes2::Point _offset; // used only during hero's movement

    fheroes2::Point patrol_center;
    int patrol_square;

    std::list<IndexObject> visit_object;
    uint32_t _lastGroundRegion = 0;

    RedrawIndex _redrawIndex;

    mutable int _alphaValue;

    int _attackedMonsterTileIndex; // used only when hero attacks a group of wandering monsters

    // This value should NOT be saved in save file as it's dynamically set during AI turn.
    Role _aiRole;

    enum
    {
        HERO_MOVE_STEP = 4 // in pixels
    };
};

struct VecHeroes : public std::vector<Heroes *>
{
    Heroes * Get( int /* hero id */ ) const;
    Heroes * Get( const fheroes2::Point & ) const;
};

struct AllHeroes : public VecHeroes
{
    AllHeroes();
    AllHeroes( const AllHeroes & ) = delete;

    ~AllHeroes();

    AllHeroes & operator=( const AllHeroes & ) = delete;

    void Init( void );
    void clear( void );

    void Scoute( int ) const;

    void ResetModes( const uint32_t modes ) const
    {
        std::for_each( begin(), end(), [modes]( Heroes * hero ) { hero->ResetModes( modes ); } );
    }

    void NewDay()
    {
        std::for_each( begin(), end(), []( Heroes * hero ) { hero->ActionNewDay(); } );
    }

    void NewWeek()
    {
        std::for_each( begin(), end(), []( Heroes * hero ) { hero->ActionNewWeek(); } );
    }

    void NewMonth()
    {
        std::for_each( begin(), end(), []( Heroes * hero ) { hero->ActionNewMonth(); } );
    }

    Heroes * GetGuest( const Castle & ) const;
    Heroes * GetGuard( const Castle & ) const;
    Heroes * GetFreeman( const int race, const int heroIDToIgnore ) const;
    Heroes * FromJail( s32 ) const;
};

StreamBase & operator<<( StreamBase &, const VecHeroes & );
StreamBase & operator>>( StreamBase &, VecHeroes & );

StreamBase & operator<<( StreamBase &, const Heroes & );
StreamBase & operator>>( StreamBase &, Heroes & );

StreamBase & operator<<( StreamBase &, const AllHeroes & );
StreamBase & operator>>( StreamBase &, AllHeroes & );

#endif
