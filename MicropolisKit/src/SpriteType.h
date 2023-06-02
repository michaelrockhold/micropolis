//
//  SpriteType.h
//  Micropolis
//
//  Created by Michael Rockhold on 5/31/23.
//

#ifndef SpriteType_h
#define SpriteType_h

/* Object & Sound Numbers */

enum SpriteType {
    SPRITE_NOTUSED = 0,

    SPRITE_TRAIN, ///< Train sprite
    SPRITE_HELICOPTER, ///< Helicopter sprite
    SPRITE_AIRPLANE, ///< Airplane sprite
    SPRITE_SHIP, ///< Ship
    SPRITE_MONSTER, ///< Scary monster
    SPRITE_TORNADO, ///< Tornado sprite
    SPRITE_EXPLOSION, ///< Explosion sprite
    SPRITE_BUS, ///< Bus sprite

    SPRITE_COUNT, ///< Number of sprite objects
};

static const int MIN_SPRITE_TYPE_IDX = SPRITE_TRAIN;
static const int MAX_SPRITE_TYPE_IDX = SPRITE_BUS;


#endif /* SpriteType_h */
