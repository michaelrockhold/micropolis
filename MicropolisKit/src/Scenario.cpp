//
//  Scenario.cpp
//  MicropolisKit
//
//  Created by Michael Rockhold on 7/7/23.
//

#include <stdio.h>
#include "Scenario.h"

// Expose a pure-C "constructor" for Scenario until Swift has better integration with C++ post-5.7
extern "C" {
Scenario * newScenario(
                               char* name,
                               char* title,
                               char* description,
                               unsigned char* cityData,
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
                               ) {
    Scenario* s = new Scenario;
    s->name = name;
    s->title = title;
    s->description = description;
    memcpy(&s->cityData[0], cityData, CITY_DATASIZE);
    s->cityTime = cityTime;
    s->crisisType = crisisType;
    s->crisisTimeMode = crisisTimeMode;
    s->crisisTime = crisisTime;
    s->disasterWait = disasterWait;
    s->scoreWait = scoreWait;
    s->winCriterion = winCriterion;
    s->winCriterionArg = winCriterionArg;
    s->hasFoghorns = hasFoghorns;

    return s;
}
}
