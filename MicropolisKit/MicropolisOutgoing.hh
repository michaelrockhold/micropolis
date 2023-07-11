//
//  MicropolisOutgoing.hh
//  micropolis-engine
//
//  Created by Michael Rockhold on 2/14/23.
//

#ifndef MicropolisOutgoing_h
#define MicropolisOutgoing_h

#import <Foundation/Foundation.h>
#import <MicropolisKit/text.h>

@protocol MicropolisOutgoing <NSObject>

- (void)doPlayNewCity;
- (void)doEarthquakeWithStrength:(int)strength;
- (void)updateTarget:(NSString*)target;
- (void)updateTime:(int)cityTime;
- (void)makeSound:(NSString*)sound forChannel:(NSString *)channel atX:(int)x andY:(int)y;
- (void)doRobots;
- (void)doSimulateChurch:(int)churchNumber atX:(int)x andY:(int)y;
- (void)sendMessageWithMesgNum:(enum MessageNumber)mesgNum atX:(int)x andY:(int)y withPicture:(bool)picture andImportance:(bool)important;
- (void)doAutoGotoX:(int)x andY:(int)y withMsg:(NSString*)msg;
- (void)doLoseGame;
- (void)doWinGame;
- (void)showBudgetAndWait;
- (void)didLoadScenario;
- (void)didLoadCity;
- (void)didntLoadCityWithMsg:(NSString*)msg;
- (void)doSaveCityAs;
- (void)didSaveCity;
- (void)didntSaveCityWithMsg:(NSString*)msg;
- (void)doNewGame;
- (void)doShowZoneStatusForTileCategory:(int)tileCategory s0:(int)s0 s1:(int)s1 s2:(int)s2 s3:(int)s3 s4:(int)s4 atX:(int)x andY:(int)y;
- (void)didToolWithName:(NSString*)name atX:(int)x andY:(int)y;
- (void)didGenerateMap;
- (void)updateTileAtX:(int)x andY:(int)y tileIdx:(int)tIdx;
@end

#endif /* MicropolisOutgoing_h */
