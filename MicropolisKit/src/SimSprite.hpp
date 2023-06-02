//
//  SimSprite.hpp
//  MicropolisKit
//
//  Created by Michael Rockhold on 5/16/23.
//

#ifndef SimSprite_hpp
#define SimSprite_hpp

#include "micropolis.h"

/** Sprite in the simulator.
 * @todo SimSprite::name is never set to anything else than \c "", and only
 *       used to detect a non-removed non-active sprite (in a non-robust way).
 */
class SimSprite {

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
    
    virtual enum SpriteType type() = 0;

    static SimSprite* make(Micropolis* context, enum SpriteType type, int spriteID, int x, int y);
    
    SimSprite(Micropolis* context, int id, int x, int y);
    virtual ~SimSprite() {};
        
    virtual enum MessageNumber crashMsgNum();
    
    virtual bool isShip() {
        return false;
    }
    virtual void doMove(Micropolis* mp) = 0;
    
    virtual bool canCrash(SimSprite* other) {
        return false;
    }
    bool checkSpriteCollision(SimSprite *other);

    static int getDistance(int x1, int y1, int x2, int y2);

};

#endif /* SimSprite_hpp */
