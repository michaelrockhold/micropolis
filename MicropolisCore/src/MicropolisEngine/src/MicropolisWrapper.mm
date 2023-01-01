//
//  MicropolisWrapper.m
//  micropolis-engine
//
//  Created by Michael Rockhold on 12/31/22.
//

#import <Foundation/Foundation.h>
#import "MicropolisWrapper.hh"

#include "micropolis.h"

NSArray* argsArray(const char *params,
                   va_list arglist) {
    return [NSArray array]; // TODO: parse params to create args array from arglist
}

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
@property id<MicropolisDelegate> callbackDelegate;
@end

@implementation MicropolisWrapper

- (id)initWithCallbackDelegate:(id<MicropolisDelegate>)delegate {

    self = [super init];
    self.micropolis = new Micropolis;
    self.callbackDelegate = delegate;
    
    
    self.micropolis->userData = (__bridge void*)self;
    self.micropolis->callbackHook = MicropolisWrapper_CallbackFunction;
    self.micropolis->callbackData = (__bridge void*)self.callbackDelegate;
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
