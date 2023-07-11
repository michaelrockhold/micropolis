//
//  Scenario.h
//  MicropolisKit
//
//  Created by Michael Rockhold on 6/1/23.
//

#ifndef Scenario_h
#define Scenario_h

#ifdef __cplusplus
#include <string>
#endif

enum CITY {
    CITY_DATASIZE = 27120
};

enum CRISIS_TYPE {
    CRISIS_TYPE_DEADLINE = 0,
    CRISIS_TYPE_EARTHQUAKE,
    CRISIS_TYPE_FIREBOMBS,
    CRISIS_TYPE_MONSTER,
    CRISIS_TYPE_MELTDOWN,
    CRISIS_TYPE_FLOOD
};

enum CRISIS_TIME_MODE {
    CRISIS_TIME_MODE_DEADLINE = 0,
    CRISIS_TIME_MODE_AT,
    CRISIS_TIME_MODE_MODULO
};

enum WINCRITERION {
    WINCRITERION_UNKNOWN = 0,
    WINCRITERION_METROPOLIS,
    WINCRITERION_TRAFFIC,
    WINCRITERION_CITYSCORE,
    WINCRITERION_CRIME
};

struct Scenario {
#ifdef __cplusplus
    std::string name;
    std::string title;
    std::string description;
    unsigned char cityData[CITY_DATASIZE];
    int cityTime;
    int funds;
    enum CRISIS_TYPE crisisType;
    enum CRISIS_TIME_MODE crisisTimeMode;
    int crisisTime;
    int disasterWait;   ///< Count-down timer for the disaster
    int scoreWait;      ///< Time to wait before computing the score
    enum WINCRITERION winCriterion;
    int winCriterionArg;
    bool hasFoghorns; // TODO: this should really be an attribute of City, not Scenario
#endif
};

#ifndef __cplusplus
typedef struct Scenario Scenario;

Scenario* newScenario(
                      char* name,
                      char* title,
                      char* description,
                      const unsigned char * cityData,
                      int cityTime,
                      int funds,
                      enum CRISIS_TYPE crisisType,
                      enum CRISIS_TIME_MODE crisisTimeMode,
                      int crisisTime,
                      int disasterWait,
                      int scoreWait,
                      enum WINCRITERION winCriterion,
                      int winCriterionArg,
                      bool hasFoghorns
                      );

#endif
#endif /* Scenario_h */
