//
//  MicropolisWrapper.m
//  micropolis-engine
//
//  Created by Michael Rockhold on 12/31/22.
//

#import <Foundation/Foundation.h>
#import "MicropolisWrapper.hh"

#include "micropolis.h"

void MicropolisWrapper_CallbackFunction(
    Micropolis *micropolis,
    void *data,
    const char *name,
    const char *params,
    va_list arglist) {

    if (data == nil || micropolis->userData == nil) return;

    id<MicropolisDelegate> delegate = (__bridge id<MicropolisDelegate>)data;
    MicropolisWrapper* micropolisWrapper = (__bridge MicropolisWrapper*)micropolis->userData;
    
    [delegate micropolisWrapper:micropolisWrapper
                       callback:[NSString stringWithCString:name encoding:NSUTF8StringEncoding]
                           args:argsArray(params, arglist)];
}


@interface MicropolisWrapper ()
@property Micropolis* micropolis;
@property id<MicropolisDelegate> delegate;
@end

@implementation MicropolisWrapper

- (id)initWithCallbackDelegate:(id<MicropolisDelegate>)delegate {

    self = [super init];
    self.micropolis = new Micropolis;
    self.delegate = delegate;
    
    
    self.micropolis->userData = (__bridge void*)self;
    self.micropolis->callbackHook = MicropolisWrapper_CallbackFunction;
    self.micropolis->delegate = (__bridge void*)delegate;
    return self;
}

-(void)dealloc {
    delete self.micropolis;
    self.micropolis = nil;
}

- (NSString *)getMicropolisVersion {
    return [NSString stringWithCString:self.micropolis->getMicropolisVersion() encoding:NSUTF8StringEncoding];
}
@end
