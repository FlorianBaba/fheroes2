/***************************************************************************
 *   Free Heroes of Might and Magic II: https://github.com/ihhub/fheroes2  *
 *   Copyright (C) 2019 - 2022                                             *
 *                                                                         *
 *   Free Heroes2 Engine: http://sourceforge.net/projects/fheroes2         *
 *   Copyright (C) 2013 by Andrey Afletdinov <fheroes2@gmail.com>          *
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
#ifndef H2MAPS_OBJECTS_H
#define H2MAPS_OBJECTS_H

#include <string>

#include "artifact.h"
#include "position.h"
#include "resource.h"

class StreamBuf;

class MapObjectSimple : public MapPosition
{
public:
    explicit MapObjectSimple( int v = 0 )
        : uid( 0 )
        , type( v )
    {}
    ~MapObjectSimple() override = default;

    int GetType( void ) const
    {
        return type;
    }
    u32 GetUID( void ) const
    {
        return uid;
    }
    void SetUID( u32 v )
    {
        uid = v;
    }

protected:
    friend StreamBase & operator<<( StreamBase &, const MapObjectSimple & );
    friend StreamBase & operator>>( StreamBase &, MapObjectSimple & );

    u32 uid;
    int type;
};

StreamBase & operator<<( StreamBase &, const MapObjectSimple & );
StreamBase & operator>>( StreamBase &, MapObjectSimple & );

struct MapEvent : public MapObjectSimple
{
    MapEvent();

    void LoadFromMP2( s32 index, StreamBuf );

    bool isAllow( int color ) const;
    void SetVisited( int color );

    Funds resources;
    Artifact artifact;
    bool computer;
    bool cancel;
    int colors;
    std::string message;
};

StreamBase & operator<<( StreamBase &, const MapEvent & );
StreamBase & operator>>( StreamBase &, MapEvent & );

using RiddleAnswers = std::list<std::string>;

struct MapSphinx : public MapObjectSimple
{
    MapSphinx();

    void LoadFromMP2( s32 index, StreamBuf );

    bool AnswerCorrect( const std::string & answer );
    void SetQuiet( void );

    Funds resources;
    Artifact artifact;
    RiddleAnswers answers;
    std::string message;
    bool valid;
};

StreamBase & operator<<( StreamBase &, const MapSphinx & );
StreamBase & operator>>( StreamBase &, MapSphinx & );

struct MapSign : public MapObjectSimple
{
    MapSign();

    void LoadFromMP2( s32 index, StreamBuf );

    std::string message;
};

StreamBase & operator<<( StreamBase &, const MapSign & );
StreamBase & operator>>( StreamBase &, MapSign & );

#endif
