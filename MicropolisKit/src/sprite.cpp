/* sprite.cpp
 *
 * Micropolis, Unix Version.  This game was released for the Unix platform
 * in or about 1990 and has been modified for inclusion in the One Laptop
 * Per Child program.  Copyright (C) 1989 - 2007 Electronic Arts Inc.  If
 * you need assistance with this program, you may contact:
 *   http://wiki.laptop.org/go/Micropolis  or email  micropolis@laptop.org.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  You should have received a
 * copy of the GNU General Public License along with this program.  If
 * not, see <http://www.gnu.org/licenses/>.
 *
 *             ADDITIONAL TERMS per GNU GPL Section 7
 *
 * No trademark or publicity rights are granted.  This license does NOT
 * give you any right, title or interest in the trademark SimCity or any
 * other Electronic Arts trademark.  You may not distribute any
 * modification of this program using the trademark SimCity or claim any
 * affliation or association with Electronic Arts Inc. or its employees.
 *
 * Any propagation or conveyance of this program must include this
 * copyright notice and these terms.
 *
 * If you convey this program (or any modifications of it) and assume
 * contractual liability for the program to recipients of it, you agree
 * to indemnify Electronic Arts for any liability that those contractual
 * assumptions impose on Electronic Arts.
 *
 * You may not misrepresent the origins of this program; modified
 * versions of the program must be marked as such and not identified as
 * the original program.
 *
 * This disclaimer supplements the one included in the General Public
 * License.  TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, THIS
 * PROGRAM IS PROVIDED TO YOU "AS IS," WITH ALL FAULTS, WITHOUT WARRANTY
 * OF ANY KIND, AND YOUR USE IS AT YOUR SOLE RISK.  THE ENTIRE RISK OF
 * SATISFACTORY QUALITY AND PERFORMANCE RESIDES WITH YOU.  ELECTRONIC ARTS
 * DISCLAIMS ANY AND ALL EXPRESS, IMPLIED OR STATUTORY WARRANTIES,
 * INCLUDING IMPLIED WARRANTIES OF MERCHANTABILITY, SATISFACTORY QUALITY,
 * FITNESS FOR A PARTICULAR PURPOSE, NONINFRINGEMENT OF THIRD PARTY
 * RIGHTS, AND WARRANTIES (IF ANY) ARISING FROM A COURSE OF DEALING,
 * USAGE, OR TRADE PRACTICE.  ELECTRONIC ARTS DOES NOT WARRANT AGAINST
 * INTERFERENCE WITH YOUR ENJOYMENT OF THE PROGRAM; THAT THE PROGRAM WILL
 * MEET YOUR REQUIREMENTS; THAT OPERATION OF THE PROGRAM WILL BE
 * UNINTERRUPTED OR ERROR-FREE, OR THAT THE PROGRAM WILL BE COMPATIBLE
 * WITH THIRD PARTY SOFTWARE OR THAT ANY ERRORS IN THE PROGRAM WILL BE
 * CORRECTED.  NO ORAL OR WRITTEN ADVICE PROVIDED BY ELECTRONIC ARTS OR
 * ANY AUTHORIZED REPRESENTATIVE SHALL CREATE A WARRANTY.  SOME
 * JURISDICTIONS DO NOT ALLOW THE EXCLUSION OF OR LIMITATIONS ON IMPLIED
 * WARRANTIES OR THE LIMITATIONS ON THE APPLICABLE STATUTORY RIGHTS OF A
 * CONSUMER, SO SOME OR ALL OF THE ABOVE EXCLUSIONS AND LIMITATIONS MAY
 * NOT APPLY TO YOU.
 */

/** @file sprite.cpp */

////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "micropolis.h"
#include "text.h"
#include "SimSprite.hpp"

#include <algorithm>
#include <vector>

////////////////////////////////////////////////////////////////////////


#define TRA_GROOVE_X -39
#define TRA_GROOVE_Y 6
#define BUS_GROOVE_X -39
#define BUS_GROOVE_Y 6


////////////////////////////////////////////////////////////////////////


/**
 * Destroy all sprites by de-activating them all (setting their
 * SimSprite::frame to 0).
 */
void Micropolis::destroyAllSprites()
{
    // TODO: does this makes sense?
    doForEachSprite([this](SimSprite* s) { delete s; });
}


void Micropolis::didCreateSprite(SimSprite* s) {
    allSprites.push_back(s);
    delegate->didCreateSprite(this, s->spriteID);
}

/**
 * Destroy the sprite by taking it out of the active list.
 * @param sprite Sprite to destroy.
 * @todo Break the connection between any views that are following this sprite.
 */
void Micropolis::willDestroySprite(SimSprite *sprite)
{
    delegate->willDestroySprite(this, sprite->spriteID);
    std::erase_if(allSprites, [sprite](SimSprite* s){ return s->spriteID == sprite->spriteID; });
}

void Micropolis::didUpdateSprite(SimSprite *s) {
    delegate->didUpdateSprite(this, s->spriteID);
}


/**
 * Get character from the map.
 * @param x X coordinate in pixels.
 * @param y Y coordinate in pixels.
 * @return Map character if on-map, or \c -1 if off-map.
 */
short Micropolis::getChar(int x, int y)
{
    // Convert sprite coordinates to tile coordinates.
    x >>= 4;
    y >>= 4;
    
    if (!testBounds(x, y)) {
        return -1;
    } else {
        return map[x][y] & LOMASK;
    }
}


/**
 * Turn.
 * @param p Present direction (1..8).
 * @param d Destination direction (1..8).
 * @return New direction.
 * @todo Remove local magic constants and document the code.
 */
short Micropolis::turnTo(int p, int d)
{
    if (p == d) {
        return p;
    }
    
    if (p < d) {
        if (d - p < 4) {
            p++;
        } else {
            p--;
        }
    } else {
        if (p - d < 4) {
            p--;
        } else {
            p++;
        }
    }
    
    if (p > 8) {
        p = 1;
    }
    
    if (p < 1) {
        p = 8;
    }
    
    return p;
}

/** ???
 * @todo Figure out what this function is doing.
 * @todo Remove local magic constants and document the code.
 */
bool Micropolis::tryOther(int Tpoo, int Told, int Tnew)
{
    short z;
    
    z = Told + 4;
    
    if (z > 8) {
        z -= 8;
    }
    
    if (Tnew != z) {
        return false;
    }
    
    if (Tpoo == POWERBASE || Tpoo == POWERBASE + 1
        || Tpoo == RAILBASE || Tpoo == RAILBASE + 1) {
        return true;
    }
    
    return false;
}


/**
 * Check whether a sprite is still entirely on-map.
 * @param sprite Sprite to check.
 * @return Sprite is at least partly off-map.
 */
bool Micropolis::spriteNotInBounds(SimSprite *sprite)
{
    int x = sprite->x + sprite->xHot;
    int y = sprite->y + sprite->yHot;
    
    return x < 0 || y < 0 || x >= (WORLD_W <<4) || y >= (WORLD_H <<4);
}

/**
 * Get direction (0..8?) to get from starting point to destination point.
 * @param orgX X coordinate starting point.
 * @param orgY Y coordinate starting point.
 * @param desX X coordinate destination point.
 * @param desY Y coordinate destination point.
 * @return Direction to go in.
 * @todo Remove local magic constants and document the code.
 * @bug Has a condition that never holds.
 */
short Micropolis::getDir(int orgX, int orgY, int desX, int desY)
{
    static const short Gdtab[13] = { 0, 3, 2, 1, 3, 4, 5, 7, 6, 5, 7, 8, 1 };
    int dispX, dispY, z;
    
    dispX = desX - orgX;
    dispY = desY - orgY;
    
    if (dispX < 0) {
        if (dispY < 0) {
            z = 11;
        } else {
            z = 8;
        }
    } else {
        if (dispY < 0) {
            z = 2;
        } else {
            z = 5;
        }
    }
    
    dispX = absoluteValue(dispX);
    dispY = absoluteValue(dispY);
    absDist = dispX + dispY;
    
    if (dispX * 2 < dispY) {
        z++;
    } else if (dispY * 2 < dispY) {  // XXX This never holds!!
        z--;
    }
    
    if (z < 0 || z > 12) {
        z = 0;
    }
    
    return Gdtab[z];
}


/**
 * Compute Manhattan distance between two points.
 * @param x1 X coordinate first point.
 * @param y1 Y coordinate first point.
 * @param x2 X coordinate second point.
 * @param y2 Y coordinate second point.
 * @return Manhattan distance between both points.
 */
int Micropolis::getDistance(int x1, int y1, int x2, int y2)
{
    return SimSprite::getDistance(x1, y1, x2, y2);
}


/**
 * Move all sprites.
 *
 * Sprites with SimSprite::frame == 0 are removed.
 */
void Micropolis::moveObjects() {
    
    if (!simSpeed) {
        return;
    }
    
    spriteCycle++;

    doForEachSprite([this](SimSprite* s) mutable {
        if (s->frame <= 0) {
            delete s;
        } else {
            s->doMove();
        }
    });
}


/**
 * Can one drive at the specified tile?
 * @param x X coordinate at map.
 * @param y Y coordinate at map.
 * @return 0 if not, 1 if you can, -1 otherwise
 */
int Micropolis::canDriveOn(int x, int y)
{
    int tile;
    
    if (!testBounds(x, y)) {
        return 0;
    }
    
    tile = map[x][y] & LOMASK;
    
    if ((tile >= ROADBASE && tile <= LASTROAD && tile != BRWH && tile != BRWV)
        || tile == HRAILROAD || tile == VRAILROAD) {
        return 1;
    }
    
    if (tile == DIRT || tally(tile)) {
        return -1;
    }
    
    return 0;
}


bool Micropolis::checkWet(int x)
{
    if (x == HPOWER || x == VPOWER || x == HRAIL || x == VRAIL
        || x == BRWH || x == BRWV) {
        return true;
    } else {
        return false;
    }
}


/**
 * Destroy a map tile.
 * @param ox X coordinate in pixels.
 * @param oy Y coordinate in pixels.
 */
void Micropolis::destroyMapTile(int ox, int oy)
{
    short t, z, x, y;
    
    x = ox >>4;
    y = oy >>4;
    
    if (!testBounds(x, y)) {
        return;
    }
    
    z = map[x][y];
    t = z & LOMASK;
    
    if (t >= TREEBASE) {
        if (!(z & BURNBIT)) {
            
            if (t >= ROADBASE && t <= LASTROAD) {
                map[x][y] = RIVER;
            }
            
            return;
        }
        
        if (z & ZONEBIT) {
            
            startFireInZone(x, y, z);
            
            if (t > RZB) {
                makeExplosionAt(ox, oy);
            }
            
        }
        
        if (checkWet(t)) {
            map[x][y] = RIVER;
        } else {
            map[x][y] = (doAnimation ? TINYEXP : (LASTTINYEXP - 3))
            | BULLBIT | ANIMBIT;
        }
    }
}


/**
 * Start a fire in a zone.
 * @param Xloc X coordinate in map coordinate.
 * @param Yloc Y coordinate in map coordinate.
 * @param ch   Map character at (\a Xloc, \a Yloc).
 */
void Micropolis::startFireInZone(int Xloc, int Yloc, int ch)
{
    short Xtem, Ytem;
    short x, y, XYmax;
    
    int value = rateOfGrowthMap.worldGet(Xloc, Yloc);
    value = clamp(value - 20, -200, 200);
    rateOfGrowthMap.worldSet(Xloc, Yloc, value);
    
    ch &= LOMASK;
    
    if (ch < PORTBASE) {
        XYmax = 2;
    } else {
        if (ch == AIRPORT) {
            XYmax = 5;
        } else {
            XYmax = 4;
        }
    }
    
    for (x = -1; x < XYmax; x++) {
        for (y = -1; y < XYmax; y++) {
            
            Xtem = Xloc + x;
            Ytem = Yloc + y;
            
            if (testBounds(Xtem, Ytem) && (map[Xtem][Ytem] & LOMASK) >= ROADBASE) {
                map[Xtem][Ytem] |= BULLBIT;
            }
            
        }
    }
}


/**
 * Start a fire at a single tile.
 * @param x X coordinate in map coordinate.
 * @param y Y coordinate in map coordinate.
 */
void Micropolis::startFire(int x, int y)
{
    int t, z;
    
    x >>= 4;
    y >>= 4;
    
    if (!testBounds(x, y)) {
        return;
    }
    
    z = map[x][y];
    t = z & LOMASK;
    
    if (!(z & BURNBIT) && t != DIRT) {
        return;
    }
    
    if (z & ZONEBIT) {
        return;
    }
    
    map[x][y] = randomFire();
}


/**
 * Try to start a new train sprite at the given map tile.
 * @param x X coordinate in map coordinate.
 * @param y Y coordinate in map coordinate.
 */
void Micropolis::generateTrain(int x, int y)
{
    if (totalPop > 20 && this->trainSprite == NULL && getRandom(25) == 0) {
        this->trainSprite = new TrainSprite(this, (x <<4) + TRA_GROOVE_X, (y <<4) + TRA_GROOVE_Y);
    }
}


/**
 * Try to start a new bus sprite at the given map tile.
 * @param x X coordinate in map coordinate.
 * @param y Y coordinate in map coordinate.
 */
void Micropolis::generateBus(int x, int y)
{
    if (this->busSprite == NULL && getRandom(25) == 0) {
        this->busSprite = new BusSprite(this, (x <<4) + BUS_GROOVE_X, (y <<4) + BUS_GROOVE_Y);
    }
}


/** Try to construct a new ship sprite */
void Micropolis::generateShip()
{
    short x, y;
    
    if (!(getRandom16() & 3)) {
        for (x = 4; x < WORLD_W - 2; x++) {
            if (map[x][0] == CHANNEL)  {
                makeShipHere(x, 0);
                return;
            }
        }
    }
    
    if (!(getRandom16() & 3)) {
        for (y = 1; y < WORLD_H - 2; y++) {
            if (map[0][y] == CHANNEL)  {
                makeShipHere(0, y);
                return;
            }
        }
    }
    
    if (!(getRandom16() & 3)) {
        for (x = 4; x < WORLD_W - 2; x++) {
            if (map[x][WORLD_H - 1] == CHANNEL)  {
                makeShipHere(x, WORLD_H - 1);
                return;
            }
        }
    }
    
    if (!(getRandom16() & 3)) {
        for (y = 1; y < WORLD_H - 2; y++) {
            if (map[WORLD_W - 1][y] == CHANNEL)  {
                makeShipHere(WORLD_W - 1, y);
                return;
            }
        }
    }
}


/**
 * Start a new ship sprite at the given map tile.
 * @param x X coordinate in map coordinate.
 * @param y Y coordinate in map coordinate.
 */
void Micropolis::makeShipHere(int x, int y)
{
    this->shipSprite = new ShipSprite(this, (x <<4) - (48 - 1), (y <<4));
}


/**
 * Start a new monster sprite.
 * @todo Make monster over land, because it disappears if it's made over water.
 *       Better yet make monster not disappear for a while after it's created,
 *       over land or water. Should never disappear prematurely.
 */
MonsterSprite* Micropolis::makeMonster()
{
    int x, y, z, done = 0;
    MonsterSprite* sprite;
    
    sprite = this->monsterSprite;
    if (sprite != NULL) {
        sprite->soundCount = 1;
        sprite->count = 1000;
        sprite->destX = pollutionMaxX <<4;
        sprite->destY = pollutionMaxY <<4;
        return sprite;
    }
    
    for (z = 0; z < 300; z++)  {
        
        x = getRandom(WORLD_W - 20) + 10;
        y = getRandom(WORLD_H - 10) + 5;
        
        if (map[x][y] == RIVER || map[x][y] == RIVER + BULLBIT) {
            this->monsterSprite = makeMonsterAt(x, y);
            done = 1;
            break;
        }
        
    }
    
    if (!done) {
        this->monsterSprite = makeMonsterAt(60, 50);
    }
    return this->monsterSprite;
}


/**
 * Start a new monster sprite at the given map tile.
 * @param x X coordinate in map coordinate.
 * @param y Y coordinate in map coordinate.
 */
MonsterSprite* Micropolis::makeMonsterAt(int x, int y)
{
    MonsterSprite* ms = new MonsterSprite(this, (x << 4) + 48, (y << 4));
    sendMessage(MESSAGE_MONSTER_SIGHTED, x + 5, y, true, true);
    return ms;
}


/**
 * Ensure a helicopter sprite exists.
 *
 * If it does not exist, create one at the given coordinates.
 * @param pos Start position in map coordinates.
 */
void Micropolis::generateCopter(const Position &pos)
{
    if (this->helicopterSprite != NULL) {
        return;
    }
    
    this->helicopterSprite = new HelicopterSprite(this, (pos.posX << 4), (pos.posY << 4) + 30);
}


/**
 * Ensure an airplane sprite exists.
 *
 * If it does not exist, create one at the given coordinates.
 * @param pos Start position in map coordinates.
 */
void Micropolis::generatePlane(const Position &pos)
{
    if (this->airplaneSprite != NULL) {
        return;
    }
    
    this->airplaneSprite = new AirplaneSprite(this, (pos.posX <<4) + 48, (pos.posY <<4) + 12);
}


/** Ensure a tornado sprite exists. */
void Micropolis::makeTornado()
{
    short x, y;
    SimSprite *sprite;
    
    sprite = this->tornadoSprite;
    if (sprite != NULL) {
        sprite->count = 200;
        return;
    }
    
    x = getRandom((WORLD_W <<4) - 800) + 400;
    y = getRandom((WORLD_H <<4) - 200) + 100;
    
    this->tornadoSprite = new TornadoSprite(this, x, y);
    sendMessage(MESSAGE_TORNADO_SIGHTED, (x >>4) + 3, (y >>4) + 2, true, true);
}


/**
 * Construct an explosion sprite.
 * @param x X coordinate of the explosion (in map coordinates).
 * @param y Y coordinate of the explosion (in map coordinates).
 */
void Micropolis::makeExplosion(int x, int y)
{
    if (testBounds(x, y)) {
        makeExplosionAt((x << 4) + 8, (y << 4) + 8);
    }
}


/**
 * Construct an explosion sprite.
 * @param x X coordinate of the explosion (in pixels).
 * @param y Y coordinate of the explosion (in pixels).
 */
void Micropolis::makeExplosionAt( int x, int y)
{
    new ExplosionSprite(this, x - 40, y - 16);
}


////////////////////////////////////////////////////////////////////////
