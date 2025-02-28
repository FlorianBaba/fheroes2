/***************************************************************************
 *   Free Heroes of Might and Magic II: https://github.com/ihhub/fheroes2  *
 *   Copyright (C) 2019 - 2022                                             *
 *                                                                         *
 *   Free Heroes2 Engine: http://sourceforge.net/projects/fheroes2         *
 *   Copyright (C) 2009 by Josh Matthews  <josh@joshmatthews.net>          *
 *   Copyright (C) 2008 by Andrey Afletdinov <fheroes2@gmail.com>          *
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

#ifndef H2HEROESBASE_H
#define H2HEROESBASE_H

#include "artifact.h"
#include "bitmodes.h"
#include "players.h"
#include "position.h"
#include "skill.h"
#include "spell_book.h"

class Army;
class Castle;

enum
{
    MDF_NONE,
    MDF_ATTACK,
    MDF_DEFENSE,
    MDF_POWER,
    MDF_KNOWLEDGE,
    MDF_MORALE,
    MDF_LUCK
};

enum PortraitType
{
    PORT_BIG = 1,
    PORT_MEDIUM = 2,
    PORT_SMALL = 3
};

class HeroBase : public Skill::Primary, public MapPosition, public BitModes, public Control
{
public:
    HeroBase( const int type, const int race );
    HeroBase();

    enum
    {
        UNDEFINED,
        CAPTAIN,
        HEROES
    };

    virtual const std::string & GetName() const = 0;
    virtual int GetColor() const = 0;
    int GetControl() const override = 0;
    virtual bool isValid() const = 0;

    virtual const Army & GetArmy() const = 0;
    virtual Army & GetArmy() = 0;

    virtual u32 GetMaxSpellPoints() const = 0;

    virtual int GetLevelSkill( int skill ) const = 0;
    virtual u32 GetSecondaryValues( int skill ) const = 0;

    virtual void ActionAfterBattle() = 0;
    virtual void ActionPreBattle() = 0;

    virtual const Castle * inCastle() const = 0;
    virtual void PortraitRedraw( const int32_t px, const int32_t py, const PortraitType type, fheroes2::Image & dstsf ) const = 0;

    virtual int GetType() const = 0;

    virtual bool isCaptain() const;
    virtual bool isHeroes() const;

    int GetAttackModificator( std::string * = nullptr ) const;
    int GetDefenseModificator( std::string * = nullptr ) const;
    int GetPowerModificator( std::string * = nullptr ) const;
    int GetKnowledgeModificator( std::string * = nullptr ) const;
    int GetMoraleModificator( std::string * = nullptr ) const;
    int GetLuckModificator( std::string * = nullptr ) const;
    double GetSpellcastStrength( const double armyLimit ) const;

    uint32_t GetSpellPoints() const;
    bool HaveSpellPoints( const Spell & spell ) const;
    bool CanCastSpell( const Spell & spell, std::string * = nullptr ) const;
    bool CanTeachSpell( const Spell & spell ) const;
    bool CanLearnSpell( const Spell & spell ) const;
    bool CanTranscribeScroll( const Artifact & art ) const;
    void TranscribeScroll( const Artifact & art );
    void SpellCasted( const Spell & spell );
    void SetSpellPoints( const uint32_t points );

    std::vector<Spell> GetSpells( const int lvl = -1 ) const;
    void EditSpellBook();
    Spell OpenSpellBook( const SpellBook::Filter filter, const bool canSelect, const std::function<void( const std::string & )> * statusCallback ) const;
    bool HaveSpellBook() const;
    bool HaveSpell( const Spell &, const bool skip_bag = false ) const;
    void AppendSpellToBook( const Spell &, const bool without_wisdom = false );
    void AppendSpellsToBook( const SpellStorage &, const bool without_wisdom = false );
    bool SpellBookActivate();

    BagArtifacts & GetBagArtifacts();
    const BagArtifacts & GetBagArtifacts() const;
    uint32_t artifactCount( const Artifact & art ) const;
    bool hasArtifact( const Artifact & art ) const;

    void LoadDefaults( const int type, const int race );

protected:
    friend StreamBase & operator<<( StreamBase &, const HeroBase & );
    friend StreamBase & operator>>( StreamBase &, HeroBase & );

    uint32_t magic_point;
    uint32_t move_point;

    SpellBook spell_book;
    BagArtifacts bag_artifacts;
};

StreamBase & operator<<( StreamBase &, const HeroBase & );
StreamBase & operator>>( StreamBase &, HeroBase & );

#endif
