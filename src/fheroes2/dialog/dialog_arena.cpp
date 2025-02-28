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
#include "localevent.h"
#include "settings.h"
#include "skill.h"
#include "text.h"
#include "translations.h"
#include "ui_button.h"

void InfoSkillClear( const fheroes2::Rect &, const fheroes2::Rect &, const fheroes2::Rect &, const fheroes2::Rect & );
void InfoSkillSelect( int, const fheroes2::Rect &, const fheroes2::Rect &, const fheroes2::Rect &, const fheroes2::Rect & );
int InfoSkillNext( int );
int InfoSkillPrev( int );

int Dialog::SelectSkillFromArena( void )
{
    fheroes2::Display & display = fheroes2::Display::instance();
    const int system = Settings::Get().ExtGameEvilInterface() ? ICN::SYSTEME : ICN::SYSTEM;
    const bool allSkills = Settings::Get().ExtHeroArenaCanChoiseAnySkills();

    // setup cursor
    const CursorRestorer cursorRestorer( true, Cursor::POINTER );

    TextBox title( _( "Arena" ), Font::YELLOW_BIG, BOXAREA_WIDTH );

    TextBox textbox(
        _( "You enter the arena and face a pack of vicious lions. You handily defeat them, to the wild cheers of the crowd.  Impressed by your skill, the aged trainer of gladiators agrees to train you in a skill of your choice." ),
        Font::BIG, BOXAREA_WIDTH );
    const fheroes2::Sprite & sprite = fheroes2::AGG::GetICN( ICN::XPRIMARY, 0 );
    const int spacer = 10;

    Dialog::FrameBox box( title.h() + textbox.h() + 2 * spacer + sprite.height() + 15, true );

    const fheroes2::Rect & box_rt = box.GetArea();
    fheroes2::Point dst_pt( box_rt.x, box_rt.y );

    title.Blit( dst_pt.x, dst_pt.y );
    dst_pt.y += title.h() + spacer;

    textbox.Blit( dst_pt.x, dst_pt.y );
    dst_pt.y += textbox.h() + spacer;

    int res = Skill::Primary::ATTACK;

    const int spacingX = allSkills ? ( box_rt.width - sprite.width() * 4 ) / 5 : ( box_rt.width - sprite.width() * 3 ) / 4;

    fheroes2::Rect rect1( dst_pt.x + spacingX, dst_pt.y, sprite.width(), sprite.height() );
    fheroes2::Rect rect2( rect1.x + sprite.width() + spacingX, dst_pt.y, sprite.width(), sprite.height() );
    fheroes2::Rect rect3( rect2.x + sprite.width() + spacingX, dst_pt.y, sprite.width(), sprite.height() );
    fheroes2::Rect rect4( rect3.x + sprite.width() + spacingX, dst_pt.y, sprite.width(), sprite.height() );

    InfoSkillClear( rect1, rect2, rect3, rect4 );
    InfoSkillSelect( res, rect1, rect2, rect3, rect4 );

    // info texts
    TextBox text( Skill::Primary::String( Skill::Primary::ATTACK ), Font::SMALL, 60 );
    dst_pt.x = rect1.x + ( rect1.width - text.w() ) / 2;
    dst_pt.y = rect1.y + rect1.height + 5;
    text.Blit( dst_pt.x, dst_pt.y );

    text.Set( Skill::Primary::String( Skill::Primary::DEFENSE ), Font::SMALL, 60 );
    dst_pt.x = rect2.x + ( rect2.width - text.w() ) / 2;
    dst_pt.y = rect2.y + rect2.height + 5;
    text.Blit( dst_pt.x, dst_pt.y );

    text.Set( Skill::Primary::String( Skill::Primary::POWER ), Font::SMALL, 60 );
    dst_pt.x = rect3.x + ( rect3.width - text.w() ) / 2;
    dst_pt.y = rect3.y + rect3.height + 5;
    text.Blit( dst_pt.x, dst_pt.y );

    if ( allSkills ) {
        text.Set( Skill::Primary::String( Skill::Primary::KNOWLEDGE ), Font::SMALL, 66 );
        dst_pt.x = rect4.x + ( rect4.width - text.w() ) / 2;
        dst_pt.y = rect4.y + rect4.height + 5;
        text.Blit( dst_pt.x, dst_pt.y );
    }

    // buttons
    dst_pt.x = box_rt.x + ( box_rt.width - fheroes2::AGG::GetICN( system, 1 ).width() ) / 2;
    dst_pt.y = box_rt.y + box_rt.height - fheroes2::AGG::GetICN( system, 1 ).height();
    fheroes2::Button buttonOk( dst_pt.x, dst_pt.y, system, 1, 2 );

    LocalEvent & le = LocalEvent::Get();

    buttonOk.draw();
    display.render();

    // message loop
    while ( le.HandleEvents() ) {
        bool redraw = false;

        le.MousePressLeft( buttonOk.area() ) ? buttonOk.drawOnPress() : buttonOk.drawOnRelease();

        if ( Game::HotKeyPressEvent( Game::EVENT_DEFAULT_LEFT ) && Skill::Primary::UNKNOWN != InfoSkillPrev( res ) ) {
            res = InfoSkillPrev( res );
            redraw = true;
        }
        else if ( Game::HotKeyPressEvent( Game::EVENT_DEFAULT_RIGHT ) && Skill::Primary::UNKNOWN != InfoSkillNext( res ) ) {
            res = InfoSkillNext( res );
            redraw = true;
        }
        else if ( le.MouseClickLeft( rect1 ) ) {
            res = Skill::Primary::ATTACK;
            redraw = true;
        }
        else if ( le.MouseClickLeft( rect2 ) ) {
            res = Skill::Primary::DEFENSE;
            redraw = true;
        }
        else if ( le.MouseClickLeft( rect3 ) ) {
            res = Skill::Primary::POWER;
            redraw = true;
        }
        else if ( allSkills && le.MouseClickLeft( rect4 ) ) {
            res = Skill::Primary::KNOWLEDGE;
            redraw = true;
        }

        if ( redraw ) {
            InfoSkillClear( rect1, rect2, rect3, rect4 );
            InfoSkillSelect( res, rect1, rect2, rect3, rect4 );
            display.render();
        }

        if ( Game::HotKeyPressEvent( Game::EVENT_DEFAULT_READY ) || le.MouseClickLeft( buttonOk.area() ) )
            break;
    }

    return res;
}

void InfoSkillClear( const fheroes2::Rect & rect1, const fheroes2::Rect & rect2, const fheroes2::Rect & rect3, const fheroes2::Rect & rect4 )
{
    fheroes2::Display & display = fheroes2::Display::instance();

    fheroes2::Blit( fheroes2::AGG::GetICN( ICN::XPRIMARY, 0 ), display, rect1.x, rect1.y );
    fheroes2::Blit( fheroes2::AGG::GetICN( ICN::XPRIMARY, 1 ), display, rect2.x, rect2.y );
    fheroes2::Blit( fheroes2::AGG::GetICN( ICN::XPRIMARY, 2 ), display, rect3.x, rect3.y );

    if ( Settings::Get().ExtHeroArenaCanChoiseAnySkills() ) {
        const int32_t borderWidth = 2;
        const fheroes2::Sprite & knowledgeICN = fheroes2::AGG::GetICN( ICN::XPRIMARY, 3 );

        fheroes2::Blit( knowledgeICN, display, rect4.x, rect4.y );
        fheroes2::Blit( knowledgeICN, borderWidth + 1, borderWidth, display, rect4.x + borderWidth, rect4.y + borderWidth, rect4.width - 2 * borderWidth - 1,
                        rect4.height - 2 * borderWidth );
        fheroes2::Blit( knowledgeICN, borderWidth, 0, display, rect4.x + rect4.width - borderWidth - 1, rect4.y, 1, rect4.height );
    }
}

void InfoSkillSelect( int skill, const fheroes2::Rect & rect1, const fheroes2::Rect & rect2, const fheroes2::Rect & rect3, const fheroes2::Rect & rect4 )
{
    fheroes2::Display & display = fheroes2::Display::instance();

    switch ( skill ) {
    case Skill::Primary::ATTACK:
        fheroes2::Blit( fheroes2::AGG::GetICN( ICN::XPRIMARY, 4 ), display, rect1.x, rect1.y );
        break;
    case Skill::Primary::DEFENSE:
        fheroes2::Blit( fheroes2::AGG::GetICN( ICN::XPRIMARY, 5 ), display, rect2.x, rect2.y );
        break;
    case Skill::Primary::POWER:
        fheroes2::Blit( fheroes2::AGG::GetICN( ICN::XPRIMARY, 6 ), display, rect3.x, rect3.y );
        break;
    case Skill::Primary::KNOWLEDGE:
        if ( Settings::Get().ExtHeroArenaCanChoiseAnySkills() ) {
            const int32_t borderWidth = 2;
            const fheroes2::Sprite & knowledgeICN = fheroes2::AGG::GetICN( ICN::XPRIMARY, 7 );

            fheroes2::Blit( knowledgeICN, display, rect4.x, rect4.y );
            fheroes2::Blit( knowledgeICN, borderWidth + 1, borderWidth, display, rect4.x + borderWidth, rect4.y + borderWidth, rect4.width - 2 * borderWidth - 1,
                            rect4.height - 2 * borderWidth );
            fheroes2::Blit( knowledgeICN, borderWidth, 0, display, rect4.x + rect4.width - borderWidth - 1, rect4.y, 1, rect4.height );
        }

        break;
    default:
        break;
    }
}

int InfoSkillNext( int skill )
{
    switch ( skill ) {
    case Skill::Primary::ATTACK:
        return Skill::Primary::DEFENSE;
    case Skill::Primary::DEFENSE:
        return Skill::Primary::POWER;
    case Skill::Primary::POWER:
        if ( Settings::Get().ExtHeroArenaCanChoiseAnySkills() )
            return Skill::Primary::KNOWLEDGE;
        break;
    default:
        break;
    }

    return Skill::Primary::UNKNOWN;
}

int InfoSkillPrev( int skill )
{
    switch ( skill ) {
    case Skill::Primary::DEFENSE:
        return Skill::Primary::ATTACK;
    case Skill::Primary::POWER:
        return Skill::Primary::DEFENSE;
    case Skill::Primary::KNOWLEDGE:
        return Skill::Primary::POWER;
    default:
        break;
    }

    return Skill::Primary::UNKNOWN;
}
