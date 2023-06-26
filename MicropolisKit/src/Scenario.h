//
//  Scenario.h
//  MicropolisKit
//
//  Created by Michael Rockhold on 6/1/23.
//

#ifndef Scenario_h
#define Scenario_h

enum CRISIS_TYPE {
    CRISIS_DEADLINE = 0,
    CRISIS_EARTHQUAKE,
    CRISIS_FIREBOMBS,
    CRISIS_MONSTER,
    CRISIS_MELTDOWN,
    CRISIS_FLOOD
};

enum CRISIS_TIME_MODE {
    CRISIS_TIME_DEADLINE = 0,
    CRISIS_TIME_AT,
    CRISIS_TIME_MODULO
};

enum WINCRITERION {
    WINCRITERION_UNKNOWN = 0,
    WINCRITERION_METROPOLIS,
    WINCRITERION_TRAFFIC,
    WINCRITERION_CITYSCORE,
    WINCRITERION_CRIME
};

struct Scenario {
    char* name;
    char* filename;
    char* title;
    char* description;
    int cityTime;
    int funds;
    enum CRISIS_TYPE crisisType;
    enum CRISIS_TIME_MODE crisisTimeMode;
    int crisisTime;
    int disasterWait;
    int scoreWait;
    enum WINCRITERION winCriterion;
    int winCriterionArg;
    bool hasFoghorns;
};

#ifndef __cplusplus__
typedef struct Scenario Scenario;
#endif
#endif /* Scenario_h */
