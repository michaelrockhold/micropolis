//
//  SimSprite.cpp
//  MicropolisKit
//
//  Created by Michael Rockhold on 5/16/23.
//

#include "micropolis.h"
#include "SimSprite.hpp"


class TrainSprite: public SimSprite {
public:
    TrainSprite(Micropolis* context, int id, int x, int y) : SimSprite(context, id, x, y) {
        this->width = 32;
        this->height = 32;
        this->xOffset = 32;
        this->yOffset = -16;
        this->xHot = 40;
        this->yHot = -8;
        this->frame = 1;
        this->dir = 4;
    }
    virtual enum SpriteType type() { return SPRITE_TRAIN; }
    
    /**
     * Move train sprite.
     * @param mp Micropolis context.
     */
    virtual void doMove(Micropolis* mp) {
                
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
        
        if ((mp->spriteCycle & 3) == 0) {
            
            dir = mp->getRandom16() & 3;
            for (z = dir; z < dir + 4; z++) {
                dir2 = z & 3;
                
                if (this->dir != 4) {
                    if (dir2 == ((this->dir + 2) & 3)) {
                        continue;
                    }
                }
                
                c = mp->getChar(this->x + Cx[dir2] + 48, this->y + Cy[dir2]);
                
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
        
        mp->didUpdateSprite(this->spriteID);
    }



enum MessageNumber crashMsgNum() { return MESSAGE_TRAIN_CRASHED; }
};

class ShipSprite: public SimSprite {
public:
    ShipSprite(Micropolis* context, int id, int x, int y) : SimSprite(context, id, x, y) {
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
    virtual enum SpriteType type() { return SPRITE_SHIP; }
    virtual void doMove(Micropolis* mp) {
        mp->doShipSprite(this);
    }
    enum MessageNumber crashMsgNum() { return MESSAGE_SHIP_CRASHED; }
    virtual bool isShip() { return this->frame != 0; }
};

class MonsterSprite: public SimSprite {
public:
    MonsterSprite(Micropolis* context, int id, int x, int y) : SimSprite(context, id, x, y) {
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
    virtual enum SpriteType type() { return SPRITE_MONSTER; }
    virtual void doMove(Micropolis* mp) {
        mp->doMonsterSprite(this);
    }
    virtual bool canCrash(SimSprite* s) {
        return s->type() == SPRITE_AIRPLANE || s->type() == SPRITE_HELICOPTER
        || s->type() == SPRITE_SHIP || s->type() == SPRITE_TRAIN;
    }
};

class HelicopterSprite: public SimSprite {
public:
    HelicopterSprite(Micropolis* context, int id, int x, int y) : SimSprite(context, id, x, y) {
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
    virtual enum SpriteType type() { return SPRITE_HELICOPTER; }
    virtual void doMove(Micropolis* mp) {
        mp->doCopterSprite(this);
    }
    enum MessageNumber crashMsgNum() { return MESSAGE_HELICOPTER_CRASHED; }
};

class AirplaneSprite: public SimSprite {
public:
    AirplaneSprite(Micropolis* context, int id, int x, int y) : SimSprite(context, id, x, y) {
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
    virtual enum SpriteType type() { return SPRITE_AIRPLANE; }
    virtual void doMove(Micropolis* mp) {
        mp->doAirplaneSprite(this);
    }
    enum MessageNumber crashMsgNum() { return MESSAGE_PLANE_CRASHED; }
    
    virtual bool canCrash(SimSprite* other) {
        return other->type() == SPRITE_HELICOPTER || other->type() == SPRITE_AIRPLANE;
    }
};

class TornadoSprite: public SimSprite {
public:
    TornadoSprite(Micropolis* context, int id, int x, int y) : SimSprite(context, id, x, y) {
        this->width = 48;
        this->height = 48;
        this->xOffset = 24;
        this->yOffset = 0;
        this->xHot = 40;
        this->yHot = 36;
        this->frame = 1;
        this->count = 200;
    }
    virtual enum SpriteType type() { return SPRITE_TORNADO; }
    virtual void doMove(Micropolis* mp) {
        mp->doTornadoSprite(this);
    }
    virtual bool canCrash(SimSprite* s) {
        return s->type() == SPRITE_AIRPLANE || s->type() == SPRITE_HELICOPTER
        || s->type() == SPRITE_SHIP || s->type() == SPRITE_TRAIN;
    }
};

class ExplosionSprite: public SimSprite {
public:
    ExplosionSprite(Micropolis* context, int id, int x, int y) : SimSprite(context, id, x, y) {
        this->width = 48;
        this->height = 48;
        this->xOffset = 24;
        this->yOffset = 0;
        this->xHot = 40;
        this->yHot = 16;
        this->frame = 1;
    }
    virtual ~ExplosionSprite() {
        printf("deleting ExplosionSprite\n");
    }
    virtual enum SpriteType type() { return SPRITE_EXPLOSION; }
    virtual void doMove(Micropolis* mp) {
        mp->doExplosionSprite(this);
    }
};

class BusSprite: public SimSprite {
public:
    BusSprite(Micropolis* context, int id, int x, int y) : SimSprite(context, id, x, y) {
        this->width = 32;
        this->height = 32;
        this->xOffset = 30;
        this->yOffset = -18;
        this->xHot = 40;
        this->yHot = -8;
        this->frame = 1;
        this->dir = 1;
    }
    virtual enum SpriteType type() { return SPRITE_BUS; }
    virtual void doMove(Micropolis* mp) {
        mp->doBusSprite(this);
    }
    enum MessageNumber crashMsgNum() { return MESSAGE_TRAIN_CRASHED; } // there is no msg for bus crashes?
    virtual bool canCrash(SimSprite* other) {
        return other->type() == SPRITE_BUS || (other->type() == SPRITE_TRAIN && other->frame != 5);
    }
};

SimSprite::SimSprite(Micropolis* context, int id, int x, int y) {
    this->context = context;
    this->spriteID = id;
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

SimSprite* SimSprite::make(Micropolis* context, enum SpriteType type, int id, int x, int y) {
    switch (type) {
        case SPRITE_TRAIN:
            return new TrainSprite(context, id, x, y);
            break;
            
        case SPRITE_SHIP:
            return new ShipSprite(context, id, x, y);
            break;
            
        case SPRITE_MONSTER:
            return new MonsterSprite(context, id, x, y);
            break;
            
        case SPRITE_HELICOPTER:
            return new HelicopterSprite(context, id, x, y);
            break;
            
        case SPRITE_AIRPLANE:
            return new AirplaneSprite(context, id, x, y);
            break;
            
        case SPRITE_TORNADO:
            return new TornadoSprite(context, id, x, y);
            break;
            
        case SPRITE_EXPLOSION:
            return new ExplosionSprite(context, id, x, y);
            break;
            
        case SPRITE_BUS:
            return new BusSprite(context, id, x, y);
            break;
            
        default:
            return NULL;
            break;
    }
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
