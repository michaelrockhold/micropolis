//
//  SimSprite.cpp
//  MicropolisKit
//
//  Created by Michael Rockhold on 5/16/23.
//

#include "micropolis.h"
#include "SimSprite.hpp"

#include <algorithm>
#include <vector>

std::vector<SimSprite*> SimSprite::globalSprites;


void SimSprite::doForEach(std::function<void(SimSprite*)> f) {
    
    std::for_each(SimSprite::globalSprites.begin(), SimSprite::globalSprites.end(), f);
}


TrainSprite::TrainSprite(Micropolis* context, int x, int y) : SimSprite(context, x, y) {
    this->width = 32;
    this->height = 32;
    this->xOffset = 32;
    this->yOffset = -16;
    this->xHot = 40;
    this->yHot = -8;
    this->frame = 1;
    this->dir = 4;
}

/**
 * Move train sprite.
 */
void TrainSprite::doMove() {
    
    /* Offset in pixels of sprite x and y to map tile */
    static const short Cx[4] = {   0,  16,   0, -16 };
    static const short Cy[4] = { -16,   0,  16,   0 };
    /* X and Y movement of the sprite in pixels */
    static const short Dx[5] = {   0,   4,   0,  -4,   0 };
    static const short Dy[5] = {  -4,   0,   4,   0,   0 };
    
    static const short TrainPic2[5] = { 1, 2, 1, 2, 5 };
    short z, dir, dir2;
    short c;
    
    if (this->frame == 3 || this->frame == 4) {
        this->frame = TrainPic2[this->dir];
    }
    
    this->x += Dx[this->dir];
    this->y += Dy[this->dir];
    
    if ((context->spriteCycle & 3) == 0) {
        
        dir = context->getRandom16() & 3;
        for (z = dir; z < dir + 4; z++) {
            dir2 = z & 3;
            
            if (this->dir != 4) {
                if (dir2 == ((this->dir + 2) & 3)) {
                    continue;
                }
            }
            
            c = context->getChar(this->x + Cx[dir2] + 48, this->y + Cy[dir2]);
            
            if ((c >= RAILBASE && c <= LASTRAIL) /* track? */
                || c == RAILVPOWERH || c == RAILHPOWERV) {
                
                if (this->dir != dir2 && this->dir != 4) {
                    
                    if (this->dir + dir2 == 3) {
                        this->frame = 3;
                    } else {
                        this->frame = 4;
                    }
                    
                } else {
                    this->frame = TrainPic2[dir2];
                }
                
                if (c == HRAIL || c == VRAIL) {
                    this->frame = 5;
                }
                
                this->dir = dir2;
                return;
            }
        }
        
        if (this->dir == 4) {
            this->frame = 0;
            return;
        }
        
        this->dir = 4;
    }
    
    context->didUpdateSprite(this->spriteID);
}

enum MessageNumber TrainSprite::crashMsgNum() { return MESSAGE_TRAIN_CRASHED; }


ShipSprite::ShipSprite(Micropolis* context, int x, int y) : SimSprite(context, x, y) {
    this->width = 48;
    this->height = 48;
    this->xOffset = 32;
    this->yOffset = -16;
    this->xHot = 48;
    this->yHot = 0;
    
    if (x < (4 <<4)) {
        this->frame = 3;
    } else if (x >= ((WORLD_W - 4) <<4)) {
        this->frame = 7;
    } else if (y < (4 <<4)) {
        this->frame = 5;
    } else if (y >= ((WORLD_H - 4) <<4)) {
        this->frame = 1;
    } else {
        this->frame = 3;
    }
    
    this->newDir = this->frame;
    this->dir = 10;
    this->count = 1;
}

void ShipSprite::doMove() {
    static const short BDx[9] = { 0,  0,  1,  1,  1,  0, -1, -1, -1 };
    static const short BDy[9] = { 0, -1, -1,  0,  1,  1,  1,  0, -1 };
    static const short BPx[9] = { 0,  0,  2,  2,  2,  0, -2, -2, -2 };
    static const short BPy[9] = { 0, -2, -2,  0,  2,  2,  2,  0, -2 };
    static const short BtClrTab[8] = { RIVER, CHANNEL, POWERBASE, POWERBASE + 1,
        RAILBASE, RAILBASE + 1, BRWH, BRWV };
    short x, y, z, t = RIVER;
    short tem, pem;
    
    if (this->soundCount > 0) {
        this->soundCount--;
    }
    
    if (!this->soundCount) {
        
        if ((context->getRandom16() & 3) == 1) {
            
            // Convert this coordinates to tile coordinates.
            int shipX = this->x >>4;
            int shipY = this->y >>4;
            
            if (context->scenario.hasFoghorns && context->getRandom(10) < 5) {
                context->makeSound("city", "FogHornLow", shipX, shipY);
            } else {
                context->makeSound("city", "HonkHonkLow", shipX, shipY);
            }
            
        }
        
        this->soundCount = 200;
    }
    
    if (this->count > 0) {
        this->count--;
    }
    
    if (this->count == 0) {
        
        this->count = 9;
        
        if (this->frame != this->newDir) {
            this->frame = context->turnTo(this->frame, this->newDir);
            return;
        }
        
        tem = context->getRandom16() & 7;
        
        for (pem = tem; pem < (tem + 8); pem++) {
            
            z = (pem & 7) + 1;
            
            if (z == this->dir) {
                continue;
            }
            
            x = ((this->x + (48 - 1)) >>4) + BDx[z];
            y = (this->y >>4) + BDy[z];
            
            if (context->testBounds(x, y)) {
                
                t = context->map[x][y] & LOMASK;
                
                if (t == CHANNEL || t == BRWH || t == BRWV
                    || context->tryOther(t, this->dir, z)) {
                    
                    this->newDir = z;
                    this->frame = context->turnTo(this->frame, this->newDir);
                    this->dir = z + 4;
                    
                    if (this->dir > 8) {
                        this->dir -= 8;
                    }
                    
                    break;
                }
            }
        }
        
        if (pem == (tem + 8)) {
            this->dir = 10;
            this->newDir = (context->getRandom16() & 7) + 1;
        }
        
    } else {
        
        z = this->frame;
        
        if (z == this->newDir)  {
            this->x += BPx[z];
            this->y += BPy[z];
        }
    }
    
    if (this->notInBounds()) {
        this->frame = 0;
        return;
    }
    
    for (z = 0; z < 8; z++) {
        
        if (t == BtClrTab[z]) {
            break;
        }
        
        if (z == 7) {
            this->explode();
            context->destroyMapTile(this->x + 48, this->y);
        }
        
    }
}

enum MessageNumber ShipSprite::crashMsgNum() { return MESSAGE_SHIP_CRASHED; }
bool ShipSprite::isShip() const { return this->frame != 0; }


MonsterSprite::MonsterSprite(Micropolis* context, int x, int y) : SimSprite(context, x, y) {
    this->width = 48;
    this->height = 48;
    this->xOffset = 24;
    this->yOffset = 0;
    this->xHot = 40;
    this->yHot = 16;
    
    if (x > ((WORLD_W <<4) / 2)) {
        if (y > ((WORLD_H <<4) / 2)) {
            this->frame = 10;
        } else {
            this->frame = 7;
        }
    } else if (y > ((WORLD_H <<4) / 2)) {
        this->frame = 1;
    } else {
        this->frame = 4;
    }
    
    this->count = 1000;
    this->destX = context->pollutionMaxX <<4;
    this->destY = context->pollutionMaxY <<4;
    this->origX = this->x;
    this->origY = this->y;
}

/**
 * Move monster sprite.
 *
 * There are 16 monster sprite frames:
 *
 * Frame 0: NorthEast Left Foot
 * Frame 1: NorthEast Both Feet
 * Frame 2: NorthEast Right Foot
 * Frame 3: SouthEast Right Foot
 * Frame 4: SouthEast Both Feet
 * Frame 5: SouthEast Left Foot
 * Frame 6: SouthWest Right Foot
 * Frame 7: SouthWest Both Feet
 * Frame 8: SouthWest Left Foot
 * Frame 9: NorthWest Left Foot
 * Frame 10: NorthWest Both Feet
 * Frame 11: NorthWest Right Foot
 * Frame 12: North Left Foot
 * Frame 13: East Left Foot
 * Frame 14: South Right Foot
 * Frame 15: West Right Foot
 *
 * @todo Remove local magic constants and document the code.
 */
void MonsterSprite::doMove() {
    
    static const short Gx[5] = {  2,  2, -2, -2,  0 };
    static const short Gy[5] = { -2,  2,  2, -2,  0 };
    static const short ND1[4] = {  0,  1,  2,  3 };
    static const short ND2[4] = {  1,  2,  3,  0 };
    static const short nn1[4] = {  2,  5,  8, 11 };
    static const short nn2[4] = { 11,  2,  5,  8 };
    short d, z, c;
    
    if (this->soundCount > 0) {
        this->soundCount--;
    }
    
    if (this->control < 0) {
        /* business as usual */
        
        if (this->control == -2) {
            
            d = (this->frame - 1) / 3;
            z = (this->frame - 1) % 3;
            
            if (z == 2) {
                this->step = 0;
            }
            
            if (z == 0) {
                this->step = 1;
            }
            
            if (this->step) {
                z++;
            } else {
                z--;
            }
            
            c = context->getDir(this->x, this->y, this->destX, this->destY);
            
            if (context->absDist < 18) {
                
                this->control = -1;
                this->count = 1000;
                this->flag = 1;
                this->destX = this->origX;
                this->destY = this->origY;
                
            } else {
                
                c = (c - 1) / 2;
                
                if ((c != d && context->getRandom(5) == 0) || context->getRandom(20) == 0) {
                    
                    int diff = (c - d) & 3;
                    
                    if (diff == 1 || diff == 3) {
                        d = c;
                    } else {
                        
                        if (context->getRandom16() & 1) {
                            d++;
                        } else {
                            d--;
                        }
                        
                        d &= 3;
                    }
                } else {
                    
                    if (context->getRandom(20) == 0) {
                        
                        if (context->getRandom16() & 1) {
                            d++;
                        } else {
                            d--;
                        }
                        
                        d &= 3;
                    }
                }
            }
        } else {
            
            d = (this->frame - 1) / 3;
            
            if (d < 4) { /* turn n s e w */
                
                z = (this->frame - 1) % 3;
                
                if (z == 2) {
                    this->step = 0;
                }
                
                if (z == 0) {
                    this->step = 1;
                }
                
                if (this->step) {
                    z++;
                } else {
                    z--;
                }
                
                context->getDir(this->x, this->y, this->destX, this->destY);
                
                if (context->absDist < 60) {
                    
                    if (this->flag == 0) {
                        
                        this->flag = 1;
                        this->destX = this->origX;
                        this->destY = this->origY;
                        
                    } else {
                        
                        this->frame = 0;
                        return;
                        
                    }
                    
                }
                
                c = context->getDir(this->x, this->y, this->destX, this->destY);
                c = (c - 1) / 2;
                
                if ((c != d) && (!context->getRandom(10))) {
                    
                    if (context->getRandom16() & 1) {
                        z = ND1[d];
                    } else {
                        z = ND2[d];
                    }
                    
                    d = 4;
                    
                    if (!this->soundCount) {
                        // Convert sprite coordinates to tile coordinates.
                        int monsterX = this->x >>4;
                        int monsterY = this->y >>4;
                        context->makeSound("city", "Monster", monsterX, monsterY); /* monster */
                        this->soundCount = 50 + context->getRandom(100);
                    }
                    
                }
                
            } else {
                
                d = 4;
                c = this->frame;
                z = (c - 13) & 3;
                
                if (!(context->getRandom16() & 3)) {
                    
                    if (context->getRandom16() & 1) {
                        z = nn1[z];
                    } else {
                        z = nn2[z];
                    }
                    
                    d = (z - 1) / 3;
                    z = (z - 1) % 3;
                    
                }
                
            }
            
        }
        
    } else {
        
        /* somebody's taken control of the monster */
        
        d = this->control;
        z = (this->frame - 1) % 3;
        
        if (z == 2) {
            this->step = 0;
        }
        
        if (z == 0) {
            this->step = 1;
        }
        
        if (this->step) {
            z++;
        } else {
            z--;
        }
        
    }
    
    z = d * 3 + z + 1;
    
    if (z > 16) {
        z = 16;
    }
    
    this->frame = z;
    
    this->x += Gx[d];
    this->y += Gy[d];
    
    if (this->count > 0) {
        this->count--;
    }
    
    c = context->getChar(this->x + this->xHot, this->y + this->yHot);
    
    if (c == -1
        || (c == RIVER && this->count != 0 && this->control == -1)) {
        this->frame = 0; /* kill scary monster */
    }
    
    SimSprite::doForEach([this](SimSprite* s) mutable {
        if (s->checkSpriteCollision(s)) {
            s->explode();
        }
    });
    
    context->destroyMapTile(this->x + 48, this->y + 16);
}

bool MonsterSprite::canCrash(SimSprite* other) {
    return dynamic_cast<AirplaneSprite*>(other)
    || dynamic_cast<HelicopterSprite*>(other)
    || dynamic_cast<ShipSprite*>(other)
    || dynamic_cast<TrainSprite*>(other);
}


AirplaneSprite::AirplaneSprite(Micropolis* context, int x, int y) : SimSprite(context, x, y) {
    this->width = 48;
    this->height = 48;
    this->xOffset = 24;
    this->yOffset = 0;
    this->xHot = 48;
    this->yHot = 16;
    if (x > ((WORLD_W - 20) <<4)) {
        this->x -= 100 + 48;
        this->destX = this->x - 200;
        this->frame = 7;
    } else {
        this->destX = this->x + 200;
        this->frame = 11;
    }
    this->destY = this->y;
}

/**
 * Move airplane sprite.
 * @todo Remove local magic constants and document the code.
 * @todo absDist gets updated by Micropolis::getDir(), which is not always
 *       called before reading it (or worse, we just turned towards the old
 *       destination).
 */
void AirplaneSprite::doMove() {
    static const short CDx[12] = { 0,  0,  6, 8, 6, 0, -6, -8, -6, 8, 8, 8 };
    static const short CDy[12] = { 0, -8, -6, 0, 6, 8,  6,  0, -6, 0, 0, 0 };
    
    short z = this->frame;
    
    if ((context->spriteCycle % 5) == 0) {
        
        if (z > 8) { /* TakeOff  */
            z--;
            if (z < 9) {
                z = 3;
            }
            this->frame = z;
        } else { /* goto destination */
            short d = context->getDir(this->x, this->y, this->destX, this->destY);
            z = context->turnTo(z, d);
            this->frame = z;
        }
        
    }
    
    if (context->absDist < 50) { /* at destination  */
        this->destX = context->getRandom((WORLD_W * 16) + 100) - 50;
        this->destY = context->getRandom((WORLD_H * 16) + 100) - 50;
    }
    
    /* deh added test for enableDisasters */
    if (context->enableDisasters) {
        bool explode = false;
        
        /* Check whether another sprite is near enough to collide with */
        SimSprite::doForEach([this, explode](SimSprite* s) mutable {
            if (s && s->frame == 0 && s == this) {
                if (this->checkSpriteCollision(s)) {
                    this->explode();
                    explode = true;
                }
            }
        });
        
        if (explode) {
            this->explode();
        }
    }
    
    this->x += CDx[z];
    this->y += CDy[z];
    
    if (this->notInBounds()) {
        this->frame = 0;
    }
}

enum MessageNumber AirplaneSprite::crashMsgNum() { return MESSAGE_PLANE_CRASHED; }

bool AirplaneSprite::canCrash(SimSprite* other) {
    return dynamic_cast<AirplaneSprite*>(other) || dynamic_cast<HelicopterSprite*>(other);
}

TornadoSprite::TornadoSprite(Micropolis* context, int x, int y) : SimSprite(context, x, y) {
    this->width = 48;
    this->height = 48;
    this->xOffset = 24;
    this->yOffset = 0;
    this->xHot = 40;
    this->yHot = 36;
    this->frame = 1;
    this->count = 200;
}

void TornadoSprite::doMove() {
    static const short CDx[9] = {  2,  3,  2,  0, -2, -3 };
    static const short CDy[9] = { -2,  0,  2,  3,  2,  0 };
    short z;
    
    z = this->frame;
    
    if (z == 2) {
        
        /* cycle animation... post Rel */
        
        if (this->flag) {
            z = 3;
        } else {
            z = 1;
        }
        
    } else {
        
        if (z == 1) {
            this->flag = 1;
        } else {
            this->flag = 0;
        }
        
        z = 2;
    }
    
    if (this->count > 0) {
        this->count--;
    }
    
    this->frame = z;
    
    SimSprite::doForEach([this](SimSprite* s) mutable {
        if (this->checkSpriteCollision(s)) {
            this->explode();
        }
    });
    
    z = context->getRandom(5);
    this->x += CDx[z];
    this->y += CDy[z];
    
    if (this->notInBounds()) {
        this->frame = 0;
    }
    
    if (this->count != 0 && context->getRandom(500) == 0) {
        this->frame = 0;
    }
    
    context->destroyMapTile(this->x + 48, this->y + 40);
}

bool TornadoSprite::canCrash(SimSprite* other) {
    return dynamic_cast<AirplaneSprite*>(other)
    || dynamic_cast<HelicopterSprite*>(other)
    || dynamic_cast<ShipSprite*>(other)
    || dynamic_cast<TrainSprite*>(other);
}

ExplosionSprite::ExplosionSprite(Micropolis* context, int x, int y) : SimSprite(context, x, y) {
    this->width = 48;
    this->height = 48;
    this->xOffset = 24;
    this->yOffset = 0;
    this->xHot = 40;
    this->yHot = 16;
    this->frame = 1;
}

ExplosionSprite::~ExplosionSprite() {
    printf("deleting ExplosionSprite\n");
}

void ExplosionSprite::doMove() {
    short x, y;
    
    if ((context->spriteCycle & 1) == 0) {
        
        if (this->frame == 1) {
            // Convert sprite coordinates to tile coordinates.
            int explosionX = this->x >>4;
            int explosionY = this->y >>4;
            context->makeSound("city", "ExplosionHigh", explosionX, explosionY); /* explosion */
            x = (this->x >>4) + 3;
            y = (this->y >>4);
            context->sendMessage(MESSAGE_EXPLOSION_REPORTED, x, y);
        }
        
        this->frame++;
    }
    
    if (this->frame > 6) {
        this->frame = 0;
        
        context->startFire(this->x + 48 - 8, this->y + 16);
        context->startFire(this->x + 48 - 24, this->y);
        context->startFire(this->x + 48 + 8, this->y);
        context->startFire(this->x + 48 - 24, this->y + 32);
        context->startFire(this->x + 48 + 8, this->y + 32);
    }
}

BusSprite::BusSprite(Micropolis* context, int x, int y) : SimSprite(context, x, y) {
    this->width = 32;
    this->height = 32;
    this->xOffset = 30;
    this->yOffset = -18;
    this->xHot = 40;
    this->yHot = -8;
    this->frame = 1;
    this->dir = 1;
}

void BusSprite::doMove() {
    static const short Dx[5] = {   0,   1,   0,  -1,   0 };
    static const short Dy[5] = {  -1,   0,   1,   0,   0 };
    static const short Dir2Frame[4] = { 1, 2, 1, 2 };
    int dx, dy, tx, ty, otx, oty;
    int turned = 0;
    int speed = 0;
    int z;
    
#ifdef DEBUGBUS
    printf("Bus dir %d turn %d frame %d\n",
           this->dir, this->turn, this->frame);
#endif
    
    if (this->turn) {
        
        if (this->turn < 0) { /* ccw */
            
            if (this->dir & 1) { /* up or down */
                this->frame = 4;
            } else { /* left or right */
                this->frame = 3;
            }
            
            this->turn++;
            this->dir = (this->dir - 1) & 3;
            
        } else { /* cw */
            
            if (this->dir & 1) { /* up or down */
                this->frame = 3;
            } else { /* left or right */
                this->frame = 4;
            }
            
            this->turn--;
            this->dir = (this->dir + 1) & 3;
            
        }
        
        turned = 1;
        
    } else {
        
        /* finish turn */
        if ((this->frame == 3) ||
            (this->frame == 4)) {
            turned = 1;
            this->frame = Dir2Frame[this->dir];
        }
    }
    
    if (this->speed == 0) {
        
        /* brake */
        dx = 0; dy = 0;
        
    } else { /* cruise at traffic speed */
        
        tx = (this->x + this->xHot) >>5;
        ty = (this->y + this->yHot) >>5;
        
        if (tx >= 0 && tx < WORLD_W_2 && ty >= 0 && ty < WORLD_H_2) {
            
            z = context->trafficDensityMap.worldGet(tx << 1, ty << 1) >>6;
            
            if (z > 1) {
                z--;
            }
            
        } else {
            
            z = 0;
            
        }
        
        switch (z) {
                
            case 0:
                speed = 8;
                break;
                
            case 1:
                speed = 4;
                break;
                
            case 2:
                speed = 1;
                break;
                
        }
        
        /* govern speed */
        if (speed > this->speed) {
            speed = this->speed;
        }
        
        if (turned) {
            
#ifdef DEBUGBUS
            printf("turned\n");
#endif
            
            if (speed > 1) {
                speed = 1;
            }
            
            dx = Dx[this->dir] * speed;
            dy = Dy[this->dir] * speed;
            
        } else {
            
            dx = Dx[this->dir] * speed;
            dy = Dy[this->dir] * speed;
            
            tx = (this->x + this->xHot) >>4;
            ty = (this->y + this->yHot) >>4;
            
            /* drift into the right lane */
            switch (this->dir) {
                    
                case 0: /* up */
                    
                    z = ((tx <<4) + 4) - (this->x + this->xHot);
                    
                    if (z < 0) {
                        dx = -1;
                    } else if (z > 0) {
                        dx = 1;
                    }
                    
#ifdef DEBUGBUS
                    printf("moving up x %x z %d dx %d\n", this->x + this->xHot, z, dx);
#endif
                    
                    break;
                    
                case 1: /* right */
                    
                    z = ((ty <<4) + 4) - (this->y + this->yHot);
                    
                    if (z < 0) {
                        dy = -1;
                    } else if (z > 0) {
                        dy = 1;
                    }
                    
#ifdef DEBUGBUS
                    printf("moving right y %x z %d dy %d\n", this->y + this->yHot, z, dy);
#endif
                    
                    break;
                    
                case 2: /* down */
                    
                    z = (tx <<4) - (this->x + this->xHot);
                    
                    if (z < 0) {
                        dx = -1;
                    } else if (z > 0) {
                        dx = 1;
                    }
                    
#ifdef DEBUGBUS
                    printf("moving down x %x z %d dx %d\n", this->x + this->xHot, z, dx);
#endif
                    
                    break;
                    
                case 3: /* left */
                    
                    z = (ty <<4) - (this->y + this->yHot);
                    
                    if (z < 0) {
                        dy = -1;
                    } else if (z > 0) {
                        dy = 1;
                    }
                    
#ifdef DEBUGBUS
                    printf("moving left y %x z %d dy %d\n", vy + this->yHot, z, dy);
#endif
                    
                    break;
            }
        }
    }
    
#ifdef DEBUGBUS
    printf("speed dx %d dy %d\n", dx, dy);
#endif
    
#define AHEAD 8
    
    otx = (this->x + this->xHot + (Dx[this->dir] * AHEAD)) >>4;
    oty = (this->y + this->yHot + (Dy[this->dir] * AHEAD)) >>4;
    
    otx = clamp(otx, 0, WORLD_W - 1);
    oty = clamp(oty, 0, WORLD_H - 1);
    
    tx = (this->x + this->xHot + dx + (Dx[this->dir] * AHEAD)) >>4;
    ty = (this->y + this->yHot + dy + (Dy[this->dir] * AHEAD)) >>4;
    
    tx = clamp(tx, 0, WORLD_W - 1);
    ty = clamp(ty, 0, WORLD_H - 1);
    
    if (tx != otx || ty != oty) {
        
#ifdef DEBUGBUS
        printf("drive from tile %d %d to %d %d\n",
               otx, oty, tx, ty);
#endif
        
        z = context->canDriveOn(tx, ty);
        
        if (z == 0) {
            
            /* can't drive forward into a new tile */
            if (speed == 8) {
                context->bulldozerTool(tx, ty);
            } else {
            }
            
        } else {
            
            /* drive forward into a new tile */
            if (z > 0) {
                /* smooth */
            } else {
                /* bumpy */
                dx /= 2;
                dy /= 2;
            }
            
        }
    }
    
    tx = (this->x + this->xHot + dx) >>4;
    ty = (this->y + this->yHot + dy) >>4;
    
    z = context->canDriveOn(tx, ty);
    
    if (z > 0) {
        /* cool, cruise along */
    } else {
        if (z < 0) {
            /* bumpy */
        } else {
            /* something in the way */
        }
    }
    
    this->x += dx;
    this->y += dy;
    
    if (context->enableDisasters) {
        int explode = 0;
        
        SimSprite::doForEach([this, explode](SimSprite* s) mutable {
            if (this != s && this->checkSpriteCollision(s)) {
                this->explode();
                explode = 1;
            }
        });
        
        if (explode) {
            this->explode();
        }
        
    }
}

enum MessageNumber BusSprite::crashMsgNum() { return MESSAGE_TRAIN_CRASHED; } // there is no msg for bus crashes?

bool BusSprite::canCrash(SimSprite* other) {
    return dynamic_cast<BusSprite*>(other) || (dynamic_cast<TrainSprite*>(other) && other->frame != 5);
}

HelicopterSprite::HelicopterSprite(Micropolis* context, int x, int y) : SimSprite(context, x, y) {
    this->width = 32;
    this->height = 32;
    this->xOffset = 32;
    this->yOffset = -16;
    this->xHot = 40;
    this->yHot = -8;
    this->frame = 5;
    this->count = 1500;
    this->destX = context->getRandom((WORLD_W <<4) - 1);
    this->destY = context->getRandom((WORLD_H <<4) - 1);
    this->origX = x - 30;
    this->origY = y;
}

void HelicopterSprite::doMove() {
    static const short CDx[9] = { 0,  0,  3,  5,  3,  0, -3, -5, -3 };
    static const short CDy[9] = { 0, -5, -3,  0,  3,  5,  3,  0, -3 };
    
    if (this->soundCount > 0) {
        this->soundCount--;
    }
    
    if (this->control < 0) {
        
        if (this->count > 0) {
            this->count--;
        }
        
        if (this->count == 0) {
            
            /* Attract copter to monster so it blows up more often */
            MonsterSprite *ms = context->monsterSprite;
            if (ms != NULL) {
                this->destX = ms->x;
                this->destY = ms->y;
                
            } else {
                
                /* Attract copter to tornado so it blows up more often */
                TornadoSprite* ts = context->tornadoSprite;
                if (ts != NULL) {
                    this->destX = ts->x;
                    this->destY = ts->y;
                    
                } else {
                    
                    this->destX = this->origX;
                    this->destY = this->origY;
                }
            }
        }
        
        if (this->count == 0) { /* land */
            context->getDir(this->x, this->y, this->origX, this->origY);
            
            if (context->absDist < 30) {
                this->frame = 0;
                return;
            }
            
        }
        
    } else {
        
        context->getDir(this->x, this->y, this->destX, this->destY);
        
        if (context->absDist < 16) {
            this->destX = this->origX;
            this->destY = this->origY;
            this->control = -1;
        }
        
    }
    
    if (this->soundCount == 0) { /* send report  */
        
        // Convert sprite coordinates to world coordinates.
        short x = (this->x + 48) / 16;
        short y = this->y / 16;
        
        if (x >= 0 && x < WORLD_W && y >= 0 && y < WORLD_H) {
            
            /* Don changed from 160 to 170 to shut the #$%#$% thing up! */
            
            int chopperX = x + 1;
            int chopperY = y + 1;
            if (context->trafficDensityMap.worldGet(x, y) > 170 && (context->getRandom16() & 7) == 0) {
                context->sendMessage(MESSAGE_HEAVY_TRAFFIC, chopperX, chopperY, true);
                context->makeSound("city", "HeavyTraffic", chopperX, chopperY); /* chopper */
                this->soundCount = 200;
            }
            
        }
        
    }
    
    short z = this->frame;
    
    if ((context->spriteCycle & 3) == 0) {
        short d = context->getDir(this->x, this->y, this->destX, this->destY);
        z = context->turnTo(z, d);
        this->frame = z;
    }
    
    this->x += CDx[z];
    this->y += CDy[z];
}

enum MessageNumber HelicopterSprite::crashMsgNum() { return MESSAGE_HELICOPTER_CRASHED; }


/**
 * Create and initialize a sprite.
 * @param context Micropolis engine context (owner).
 * @param x X coordinate of the sprite (in pixels).
 * @param y Y coordinate of the sprite (in pixels).
 */
SimSprite::SimSprite(Micropolis* context, int x, int y) {
    this->context = context;
    this->spriteID = context->nextSpriteID++;
    this->x = x;
    this->y = y;
    this->frame = 0;
    this->origX = 0;
    this->origY = 0;
    this->destX = 0;
    this->destY = 0;
    this->count = 0;
    this->soundCount = 0;
    this->dir = 0;
    this->newDir = 0;
    this->step = 0;
    this->flag = 0;
    this->control = -1;
    this->turn = 0;
    this->accel = 0;
    this->speed = 100;
    globalSprites.push_back(this);
    context->didCreateSprite(this);
}

SimSprite::~SimSprite() {
    int thisID = this->spriteID;
    std::erase_if(globalSprites, [thisID](SimSprite* s){ return s->spriteID == thisID; });
}

/**
 * Compute Manhattan distance between two points.
 * @param x1 X coordinate first point.
 * @param y1 Y coordinate first point.
 * @param x2 X coordinate second point.
 * @param y2 Y coordinate second point.
 * @return Manhattan distance between both points.
 */
int SimSprite::getDistance(int x1, int y1, int x2, int y2)
{
    return absoluteValue(x1 - x2) + absoluteValue(y1 - y2);
}

/**
 * Check whether two sprites collide with each other.
 * @param other Second sprite.
 * @return Sprites are colliding.
 */
bool SimSprite::checkSpriteCollision(SimSprite *other)
{
    return this->frame != 0 && other && other->frame != 0
    && this->canCrash(other)
    && SimSprite::getDistance(this->x + this->xHot, this->y + this->yHot,
                              other->x + other->xHot, other->y + other->yHot) < 30;
}

enum MessageNumber SimSprite::crashMsgNum() {
    return (enum MessageNumber)-1;
}

/**
 * Check whether a sprite is still entirely on-map.
 * @return Sprite is at least partly off-map.
 */
bool SimSprite::notInBounds()
{
    int x = this->x + this->xHot;
    int y = this->y + this->yHot;
    
    return x < 0 || y < 0 || x >= (WORLD_W <<4) || y >= (WORLD_H <<4);
}

/**
 * Handle explosion of sprite (mostly due to collision?).
 * @todo Add a 'bus crashed' message to #MessageNumber.
 */
void SimSprite::explode()
{
    int x, y;
    
    this->frame = 0;
    
    x = this->x + this->xHot;
    y = this->y + this->yHot;
    context->makeExplosionAt(x, y);
    
    x = (x >>4);
    y = (y >>4);
    
    enum MessageNumber crashMsgNum = this->crashMsgNum();
    if (crashMsgNum >= 0) {
        context->sendMessage(crashMsgNum, x, y, true);
    }
    
    // Convert sprite coordinates to tile coordinates.
    context->makeSound("city", "ExplosionHigh", x, y); /* explosion */
    
    return;
}
