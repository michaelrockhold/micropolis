//
//  MicropolisWrapper.hh
//  Micropolis
//
//  Created by Michael Rockhold on 3/28/23.
//

#ifndef MicropolisWrapper_h
#define MicropolisWrapper_h

#import "MicropolisIncoming.hh"
#import "MicropolisOutgoing.hh"

@interface MicropolisWrapper : NSObject <MicropolisIncoming>

- (instancetype)init:(id<MicropolisOutgoing>)mo;

@end

#endif /* MicropolisWrapper_h */
