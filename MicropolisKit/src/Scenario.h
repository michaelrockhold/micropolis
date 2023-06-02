//
//  Scenario.h
//  MicropolisKit
//
//  Created by Michael Rockhold on 6/1/23.
//

#ifndef Scenario_h
#define Scenario_h

struct Scenario {
    char name[32];
    char filename[32];
    char title[48];
    char description[288];
    int cityTime;
    int funds;
    int disasterWait;
    int scoreWait;
    bool hasFoghorns;
}

#ifndef __cplusplus__
typedef struct Scenario Scenario;
#endif
#endif /* Scenario_h */
