//
//  MicropolisWrapper.hh
//  micropolis-engine
//
//  Created by Michael Rockhold on 12/31/22.
//

#import <Foundation/Foundation.h>

#ifndef MicropolisWrapper_h
#define MicropolisWrapper_h

@protocol MicropolisDelegate;

@interface MicropolisWrapper: NSObject

- (id)initWithCallbackDelegate:(id<MicropolisDelegate>)delegate;

- (NSString*)getMicropolisVersion;
@end


@protocol MicropolisDelegate <NSObject>
- (void)micropolisWrapper:(MicropolisWrapper*)wrapper
    callback:(NSString*)name
    args:(NSArray*)args;
@end

#endif /* MicropolisWrapper_h */
