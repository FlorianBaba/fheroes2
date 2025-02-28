/***************************************************************************
 *   Free Heroes of Might and Magic II: https://github.com/ihhub/fheroes2  *
 *   Copyright (C) 2019 - 2022                                             *
 *                                                                         *
 *   Free Heroes2 Engine: http://sourceforge.net/projects/fheroes2         *
 *   Copyright (C) 2010 by Andrey Afletdinov <fheroes2@gmail.com>          *
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

#ifndef H2BATTLE_ARENA_H
#define H2BATTLE_ARENA_H

#include <cstdint>
#include <list>
#include <utility>

#include "battle.h"
#include "battle_board.h"
#include "battle_grave.h"
#include "battle_pathfinding.h"
#include "rand.h"
#include "spell_storage.h"

class Castle;
class HeroBase;

namespace Battle
{
    class Bridge;
    class Catapult;
    class Force;
    class Units;
    class Unit;
    class Command;
    class Tower;
    class Interface;
    class Status;

    class Actions : public std::list<Command>
    {
    };

    class TroopsUidGenerator
    {
    public:
        TroopsUidGenerator() = default;
        TroopsUidGenerator( const TroopsUidGenerator & ) = delete;
        TroopsUidGenerator & operator=( const TroopsUidGenerator & ) = delete;

        uint32_t GetUnique()
        {
            return _id++;
        }

    private:
        uint32_t _id{ 1 };
    };

    class Arena
    {
    public:
        Arena( Army & army1, Army & army2, s32 index, bool local, Rand::DeterministicRandomGenerator & randomGenerator );
        ~Arena();

        void Turns( void );
        bool BattleValid( void ) const;

        bool AutoBattleInProgress() const;
        bool CanToggleAutoBattle() const;

        u32 GetCurrentTurn( void ) const;
        Result & GetResult( void );

        const HeroBase * getCommander( const int color ) const;
        const HeroBase * getEnemyCommander( const int color ) const;
        const HeroBase * GetCommander1( void ) const;
        const HeroBase * GetCommander2( void ) const;
        const HeroBase * GetCurrentCommander( void ) const;

        Force & GetForce1( void );
        Force & GetForce2( void );
        Force & getForce( const int color );
        Force & getEnemyForce( const int color );
        Force & GetCurrentForce( void );

        int GetArmyColor1( void ) const;
        int GetArmyColor2( void ) const;
        int GetCurrentColor( void ) const;
        int GetOppositeColor( int ) const;

        Unit * GetTroopBoard( s32 );
        const Unit * GetTroopBoard( s32 ) const;

        Unit * GetTroopUID( u32 );
        const Unit * GetTroopUID( u32 ) const;

        const SpellStorage & GetUsageSpells( void ) const;

        bool DialogBattleSummary( const Result & res, const std::vector<Artifact> & artifacts, bool allowToCancel ) const;
        int DialogBattleHero( const HeroBase & hero, const bool buttons, Status & status ) const;
        void DialogBattleNecromancy( const uint32_t raiseCount, const uint32_t raisedMonsterType ) const;

        void FadeArena( bool clearMessageLog ) const;

        // returns pair with move cell index and distance
        std::pair<int, uint32_t> CalculateMoveToUnit( const Unit & target ) const;

        uint32_t CalculateMoveDistance( int32_t indexTo ) const;
        bool hexIsPassable( int32_t indexTo ) const;
        Indexes getAllAvailableMoves( uint32_t moveRange ) const;
        Indexes CalculateTwoMoveOverlap( int32_t indexTo, uint32_t movementRange = 0 ) const;
        Indexes GetPath( const Unit &, const Position & ) const;

        // Returns the cell nearest to the end of the path to the cell with the given index (according to the AIBattlePathfinder)
        // and reachable for the current unit (to which the current board passability information relates) or -1 if the cell
        // with the given index is unreachable in principle
        int32_t GetNearestReachableCell( const Unit & currentUnit, const int32_t dst ) const;

        void ApplyAction( Command & );

        void TargetsApplyDamage( Unit &, const Unit &, TargetsInfo & ) const;
        TargetsInfo GetTargetsForSpells( const HeroBase * hero, const Spell & spell, int32_t dest, bool * playResistSound = nullptr );
        void TargetsApplySpell( const HeroBase *, const Spell &, TargetsInfo & ) const;

        bool isSpellcastDisabled() const;
        bool isDisableCastSpell( const Spell &, std::string * msg );

        bool GraveyardAllowResurrect( s32, const Spell & ) const;
        const Unit * GraveyardLastTroop( s32 ) const;
        std::vector<const Unit *> GetGraveyardTroops( const int32_t hexIndex ) const;
        Indexes GraveyardClosedCells( void ) const;

        bool CanSurrenderOpponent( int color ) const;
        bool CanRetreatOpponent( int color ) const;

        void ApplyActionSpellSummonElemental( const Command &, const Spell & );
        void ApplyActionSpellMirrorImage( Command & );
        void ApplyActionSpellTeleport( Command & );
        void ApplyActionSpellEarthQuake( const Command & );
        void ApplyActionSpellDefaults( Command &, const Spell & );

        bool IsShootingPenalty( const Unit &, const Unit & ) const;
        int GetICNCovr( void ) const;

        u32 GetCastleTargetValue( int ) const;

        int32_t GetFreePositionNearHero( const int heroColor ) const;

        const Rand::DeterministicRandomGenerator & GetRandomGenerator() const;

        static Board * GetBoard( void );
        static Tower * GetTower( int );
        static Bridge * GetBridge( void );
        static const Castle * GetCastle( void );
        static Interface * GetInterface( void );
        static Graveyard * GetGraveyard( void );

        static bool isAnyTowerPresent();

        enum
        {
            CATAPULT_POS = 77,
            CASTLE_GATE_POS = 50,
            CASTLE_FIRST_TOP_WALL_POS = 8,
            CASTLE_SECOND_TOP_WALL_POS = 29,
            CASTLE_THIRD_TOP_WALL_POS = 73,
            CASTLE_FOURTH_TOP_WALL_POS = 96,
            CASTLE_TOP_ARCHER_TOWER_POS = 19,
            CASTLE_BOTTOM_ARCHER_TOWER_POS = 85,
            CASTLE_TOP_GATE_TOWER_POS = 40,
            CASTLE_BOTTOM_GATE_TOWER_POS = 62
        };

    private:
        Arena( const Arena & ) = delete;
        Arena & operator=( const Arena & ) = delete;

        Arena( const Arena && ) = delete;
        Arena & operator=( const Arena && ) = delete;

        void RemoteTurn( const Unit &, Actions & );
        void HumanTurn( const Unit &, Actions & );

        void TurnTroop( Unit * troop, const Units & orderHistory );
        void TowerAction( const Tower & );

        void SetCastleTargetValue( int, u32 );
        void CatapultAction( void );

        static TargetsInfo GetTargetsForDamage( const Unit & attacker, Unit & defender, const int32_t dst, const int dir );

        std::vector<int> GetCastleTargets( void ) const;
        TargetsInfo TargetsForChainLightning( const HeroBase * hero, int32_t attackedTroopIndex );
        std::vector<Unit *> FindChainLightningTargetIndexes( const HeroBase * hero, Unit * firstUnit );

        void ApplyActionRetreat( const Command & );
        void ApplyActionSurrender( const Command & );
        void ApplyActionAttack( Command & );
        void ApplyActionMove( Command & );
        void ApplyActionEnd( Command & );
        void ApplyActionSkip( Command & );
        void ApplyActionMorale( Command & );
        void ApplyActionSpellCast( Command & );
        void ApplyActionTower( Command & );
        void ApplyActionCatapult( Command & );
        void ApplyActionAutoBattle( Command & );

        // Performs an actual attack of one unit (defender) by another unit (attacker), applying the attacker's
        // built-in magic, if necessary. If the given index of the target cell of the attack (dst) is negative,
        // then an attempt will be made to calculate it automatically based on the adjacency of the unit cells.
        // If the given direction of the attack (dir) is negative, then an attempt will be made to calculate it
        // automatically. When an attack is made by firing a shot, the dir should be UNKNOWN (zero).
        void BattleProcess( Unit & attacker, Unit & defender, int32_t dst = -1, int dir = -1 );

        Unit * CreateElemental( const Spell & );
        Unit * CreateMirrorImage( Unit &, s32 );

        Force * army1;
        Force * army2;
        Units * armies_order;

        int current_color;
        int preferredColor; // preferred color for the next unit in the battle queue

        const Castle * castle;
        const bool _isTown; // If the battle is in town (village or castle).

        Tower * towers[3];
        Catapult * catapult;
        Bridge * bridge;

        Interface * interface;
        Result result_game;

        Graveyard graveyard;
        SpellStorage usage_spells;

        Board board;
        AIBattlePathfinder _globalAIPathfinder;
        int icn_covr;

        u32 current_turn;
        int auto_battle;

        bool end_turn;

        Rand::DeterministicRandomGenerator & _randomGenerator;

        TroopsUidGenerator _uidGenerator;

        enum
        {
            CHAIN_LIGHTNING_CREATURE_COUNT = 4
        };
    };

    Arena * GetArena( void );
}

#endif
