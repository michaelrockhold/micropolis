//
//  MicropolisIncoming.hh
//  micropolis-engine
//
//  Created by Michael Rockhold on 2/14/23.
//

#ifndef MicropolisIncoming_hh
#define MicropolisIncoming_hh

#import <Foundation/Foundation.h>

@protocol MicropolisIncoming <NSObject>

- (void)simTick;
- (void)setSimSpeed:(int)newSpeed;
- (void)generateSomeCity:(int)seed;
- (void)loadScenario:(int)scenarioIndex;
- (void)generateMapUpdates;

@end

#endif /* MicropolisIncoming_hh */
