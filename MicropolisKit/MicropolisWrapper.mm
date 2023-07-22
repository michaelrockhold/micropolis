//
//  MicropolisWrapper.m
//  MicropolisKit
//
//  Created by Michael Rockhold on 3/27/23.
//

#import <Foundation/Foundation.h>

#import "MicropolisWrapper.hh"
#import "MicropolisDelegate.hh"
#import "micropolis.h"
#import <MicropolisKit/MicropolisKit-Swift.h>

@interface MicropolisWrapper ()
@property (strong, nonatomic) id<MicropolisOutgoing> micropolisOutgoing;
@end

class MicropolisCallback: public MicropolisDelegate {
private:
    __weak id<MicropolisOutgoing> micropolisOutgoing;
    
public:
    MicropolisCallback(id<MicropolisOutgoing> mo) {
        micropolisOutgoing = mo;
    }
        
    // Outgoing messages from the city engine are proxied from straight C++ to objc calls on the ObjC++ Wrapper
    
    virtual void doPlayNewCity(Micropolis* micropolis) {
        [micropolisOutgoing doPlayNewCity];
    }
    virtual void doEarthquake(Micropolis* micropolis, int strength) {
        [micropolisOutgoing doEarthquakeWithStrength:strength];
    }
    virtual void update(Micropolis* micropolis, const char * target) {
        NSString* tgt = [NSString stringWithCString:target encoding:NSASCIIStringEncoding];

        if ([tgt isEqualToString:@"date"]) {
            [micropolisOutgoing updateTime:micropolis->cityTime];
        } else {
            [micropolisOutgoing updateTarget:tgt];
        }
    }
    virtual void makeSound(Micropolis* micropolis, const char *channel, const char *sound, int x, int y) {
        NSString* snd = [NSString stringWithCString:sound encoding:NSASCIIStringEncoding];
        NSString* ch = [NSString stringWithCString:channel encoding:NSASCIIStringEncoding];
        [micropolisOutgoing makeSound:snd forChannel:ch atX:x andY:y];
    }
    virtual void doRobots(Micropolis* micropolis) {
        [micropolisOutgoing doRobots];
    }
    virtual void doSimulateChurch(Micropolis* micropolis, short x, short y, int churchNumber) {
        [micropolisOutgoing doSimulateChurch:churchNumber atX:x andY:y];
    }
    virtual void sendMessage(Micropolis* micropolis, enum MessageNumber mesgNum, short x, short y, bool picture, bool important) {
        [micropolisOutgoing sendMessageWithMesgNum:mesgNum atX:x andY:y withPicture:picture andImportance:important];
    }
    virtual void doAutoGoto(Micropolis* micropolis, short x, short y, const char *msg) {
        [micropolisOutgoing doAutoGotoX:x andY:y withMsg:[NSString stringWithCString:msg encoding:NSASCIIStringEncoding]];
    }
    virtual void doLoseGame(Micropolis* micropolis) {
        [micropolisOutgoing doLoseGame];
    }
    virtual void doWinGame(Micropolis* micropolis) {
        [micropolisOutgoing doWinGame];
    }
    virtual void showBudgetAndWait(Micropolis* micropolis) {
        [micropolisOutgoing showBudgetAndWait];
    }
    virtual void didLoadScenario(Micropolis* micropolis) {
        [micropolisOutgoing didLoadScenario];
    }
    virtual void didLoadCity(Micropolis* micropolis) {
        [micropolisOutgoing didLoadCity];
    }
    virtual void didntLoadCity(Micropolis* micropolis, const char* msg) {
        [micropolisOutgoing didntLoadCityWithMsg:[NSString stringWithCString:msg encoding:NSASCIIStringEncoding]];
    }
    virtual void doSaveCityAs(Micropolis* micropolis) {
        [micropolisOutgoing doSaveCityAs];
    }
    virtual void didSaveCity(Micropolis* micropolis) {
        [micropolisOutgoing didSaveCity];
    }
    virtual void didntSaveCity(Micropolis* micropolis, const char* msg) {
        [micropolisOutgoing didntSaveCityWithMsg:[NSString stringWithCString:msg encoding:NSASCIIStringEncoding]];
    }
    virtual void doNewGame(Micropolis* micropolis) {
        [micropolisOutgoing doNewGame];
    }
    virtual void doShowZoneStatus(Micropolis* micropolis, int tileCategory, int s0, int s1, int s2, int s3, int s4, int x, int y) {
        [micropolisOutgoing doShowZoneStatusForTileCategory:tileCategory s0:s0 s1:s1 s2:s2 s3:s3 s4:s4 atX:x andY:y];
    }
    virtual void didTool(Micropolis* micropolis, const char *name, short x, short y) {
        [micropolisOutgoing didToolWithName:[NSString stringWithCString:name encoding:NSASCIIStringEncoding] atX:x andY:y];
    }
    virtual void didGenerateMap(Micropolis* micropolis) {
        [micropolisOutgoing didGenerateMap];
    }
    
    virtual void newSprite(Micropolis* sender, int sprite_id) {
        [micropolisOutgoing didCreateSprite:sprite_id];
    }
    virtual void destroySprite(Micropolis* sender, int sprite_id) {
        [micropolisOutgoing didDestroySprite:sprite_id];
    }
    virtual void didUpdateSprite(Micropolis* sender, int sprite_id) {
        [micropolisOutgoing didUpdateSprite:sprite_id];
    }
};


@implementation MicropolisWrapper {
    // plain C++ object
    Micropolis* micropolis;
    // Objective C++ object
    MicropolisCallback* micropolisDelegate;
    
    int* mapTileCache;
}

- (instancetype)init:(id<MicropolisOutgoing>)mo {
    self = [super init];
    if (self) {
        _micropolisOutgoing = mo;
        mapTileCache = new int[WORLD_H * WORLD_W];
        
        micropolisDelegate = new MicropolisCallback(_micropolisOutgoing);
        micropolis = new Micropolis(micropolisDelegate);
        NSBundle *myBundle = [NSBundle bundleForClass:[self class]];
        micropolis->resourceDir = [[myBundle resourcePath] cStringUsingEncoding:kCFStringEncodingUTF8];
        micropolis->initGame();
    }
    return self;
}

// Wrapper owns the city engine and the callback object
- (void)dealloc {
    _micropolisOutgoing = nil;
    delete micropolis;
    delete micropolisDelegate;
    delete[] mapTileCache;
}

// Incoming messages from the client

- (void)simTick {
    micropolis->simTick();
}

- (void)setSimSpeed:(int)newSpeed {
    micropolis->setSpeed(newSpeed);
}

- (void)generateSomeCity:(int)seed {
    micropolis->generateSomeCity(seed);
}

- (void)loadScenario:(Scenario*)scenario {
    micropolis->loadScenario(scenario);
}

- (void)generateMapUpdates {
    
    uint16* mapBuffer = (uint16*)micropolis->getMapBuffer();
    for (int h = 0; h < WORLD_H; h++) {
        for (int w = 0; w < WORLD_W; w++) {
            int tile = mapBuffer[h + w * WORLD_H];
            int tileIdx = tile & 0x03ff;
            int dh = WORLD_H - 1 - h;
            if (mapTileCache[h + w * WORLD_H] != tileIdx) {
                mapTileCache[h + w * WORLD_H] = tileIdx;
                [self.micropolisOutgoing updateTileAtX:w andY:dh tileIdx:tileIdx];
            }
        }
    }
}
@end
