//
//  SimSprite.hpp
//  MicropolisKit
//
//  Created by Michael Rockhold on 5/16/23.
//

#ifndef SimSprite_hpp
#define SimSprite_hpp

#include "micropolis.h"
#include <functional>

/** Sprite in the simulator.
 * @todo SimSprite::name is never set to anything else than \c "", and only
 *       used to detect a non-removed non-active sprite (in a non-robust way).
 */
class SimSprite {
private:
    static std::vector<SimSprite*> globalSprites;

protected:
    Micropolis* context;
    
public:
    int spriteID; ///< unique identifier of this sprite.
    int frame; ///< Frame (\c 0 means non-active sprite)
    int x; ///< X coordinate of the sprite in pixels?
    int y; ///< Y coordinate of the sprite in pixels?
    int width;
    int height;
    int xOffset;
    int yOffset;
    int xHot; ///< Offset of the hot-spot relative to SimSprite::x?
    int yHot; ///< Offset of the hot-spot relative to SimSprite::y?
    int origX;
    int origY;
    int destX; ///< Destination X coordinate of the sprite.
    int destY; ///< Destination Y coordinate of the sprite.
    int count;
    int soundCount;
    int dir;
    int newDir;
    int step;
    int flag;
    int control;
    int turn;
    int accel;
    int speed;
            
    static void doForEach(std::function<void(SimSprite*)> f);

    SimSprite(Micropolis* context, int x, int y);
    virtual ~SimSprite();
        
    virtual enum MessageNumber crashMsgNum();
    
    virtual bool isShip() const {
        return false;
    }
    virtual void doMove() = 0;
    
    virtual bool canCrash(SimSprite* other) {
        return false;
    }
    
    bool checkSpriteCollision(SimSprite *other);
    
    bool notInBounds();
    
    void explode();

    static int getDistance(int x1, int y1, int x2, int y2);
};

class ExplosionSprite: public SimSprite {
public:
    ExplosionSprite(Micropolis* context, int x, int y);
    
    ~ExplosionSprite();

    virtual void doMove();    
};

class ShipSprite: public SimSprite {
public:
    ShipSprite(Micropolis* context, int x, int y);

    virtual void doMove();
    
    virtual bool isShip() const;

    virtual enum MessageNumber crashMsgNum();
};

class HelicopterSprite: public SimSprite {
public:
    HelicopterSprite(Micropolis* context, int x, int y);

    virtual void doMove();
    
    virtual enum MessageNumber crashMsgNum();
};

class MonsterSprite: public SimSprite {
public:
    MonsterSprite(Micropolis* context, int x, int y);

    virtual void doMove();
    
    virtual bool canCrash(SimSprite* other);
};

class TornadoSprite: public SimSprite {
public:
    TornadoSprite(Micropolis* context, int x, int y);

    virtual void doMove();
    
    virtual bool canCrash(SimSprite* other);
};

class BusSprite: public SimSprite {
public:
    BusSprite(Micropolis* context, int x, int y);

    virtual void doMove();
    
    virtual bool canCrash(SimSprite* other);

    virtual enum MessageNumber crashMsgNum();
};

class TrainSprite: public SimSprite {
public:

    TrainSprite(Micropolis* context, int x, int y);

    virtual void doMove();
    
    virtual enum MessageNumber crashMsgNum();
};

class AirplaneSprite: public SimSprite {
public:

    AirplaneSprite(Micropolis* context, int x, int y);

    virtual void doMove();
    
    virtual bool canCrash(SimSprite* other);

    virtual enum MessageNumber crashMsgNum();
};


#endif /* SimSprite_hpp */
