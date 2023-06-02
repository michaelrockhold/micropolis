/* micropolis.cpp
 *
 * Micropolis, Unix Version.  This game was released for the Unix platform
 * in or about 1990 and has been modified for inclusion in the One Laptop
 * Per Child program.  Copyright (C) 1989 - 2007 Electronic Arts Inc.  If
 * you need assistance with this program, you may contact:
 *   http://wiki.laptop.org/go/Micropolis  or email  micropolis@laptop.org.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  You should have received a
 * copy of the GNU General Public License along with this program.  If
 * not, see <http://www.gnu.org/licenses/>.
 *
 *             ADDITIONAL TERMS per GNU GPL Section 7
 *
 * No trademark or publicity rights are granted.  This license does NOT
 * give you any right, title or interest in the trademark SimCity or any
 * other Electronic Arts trademark.  You may not distribute any
 * modification of this program using the trademark SimCity or claim any
 * affliation or association with Electronic Arts Inc. or its employees.
 *
 * Any propagation or conveyance of this program must include this
 * copyright notice and these terms.
 *
 * If you convey this program (or any modifications of it) and assume
 * contractual liability for the program to recipients of it, you agree
 * to indemnify Electronic Arts for any liability that those contractual
 * assumptions impose on Electronic Arts.
 *
 * You may not misrepresent the origins of this program; modified
 * versions of the program must be marked as such and not identified as
 * the original program.
 *
 * This disclaimer supplements the one included in the General Public
 * License.  TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, THIS
 * PROGRAM IS PROVIDED TO YOU "AS IS," WITH ALL FAULTS, WITHOUT WARRANTY
 * OF ANY KIND, AND YOUR USE IS AT YOUR SOLE RISK.  THE ENTIRE RISK OF
 * SATISFACTORY QUALITY AND PERFORMANCE RESIDES WITH YOU.  ELECTRONIC ARTS
 * DISCLAIMS ANY AND ALL EXPRESS, IMPLIED OR STATUTORY WARRANTIES,
 * INCLUDING IMPLIED WARRANTIES OF MERCHANTABILITY, SATISFACTORY QUALITY,
 * FITNESS FOR A PARTICULAR PURPOSE, NONINFRINGEMENT OF THIRD PARTY
 * RIGHTS, AND WARRANTIES (IF ANY) ARISING FROM A COURSE OF DEALING,
 * USAGE, OR TRADE PRACTICE.  ELECTRONIC ARTS DOES NOT WARRANT AGAINST
 * INTERFERENCE WITH YOUR ENJOYMENT OF THE PROGRAM; THAT THE PROGRAM WILL
 * MEET YOUR REQUIREMENTS; THAT OPERATION OF THE PROGRAM WILL BE
 * UNINTERRUPTED OR ERROR-FREE, OR THAT THE PROGRAM WILL BE COMPATIBLE
 * WITH THIRD PARTY SOFTWARE OR THAT ANY ERRORS IN THE PROGRAM WILL BE
 * CORRECTED.  NO ORAL OR WRITTEN ADVICE PROVIDED BY ELECTRONIC ARTS OR
 * ANY AUTHORIZED REPRESENTATIVE SHALL CREATE A WARRANTY.  SOME
 * JURISDICTIONS DO NOT ALLOW THE EXCLUSION OF OR LIMITATIONS ON IMPLIED
 * WARRANTIES OR THE LIMITATIONS ON THE APPLICABLE STATUTORY RIGHTS OF A
 * CONSUMER, SO SOME OR ALL OF THE ABOVE EXCLUSIONS AND LIMITATIONS MAY
 * NOT APPLY TO YOU.
 */

/** @file micropolis.cpp */

#include "stdafx.h"
#include "micropolis.h"
#include "SimSprite.hpp"

/**
 * Check whether \a dir points to a directory.
 * If not, report an error.
 * @param dir    Directory to search.
 * @param envVar Environment variable controlling searchpath of the directory.
 * @return Directory has been found.
 */
static bool testDirectory(const std::string& dir, const char *envVar)
{
    struct stat statbuf;

    if (stat(dir.c_str(), &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {

        return true;
    }

    fprintf(stderr, "Can't find the directory \"%s\"!\n", dir.c_str());
    fprintf(stderr,
            "The environment variable \"%s\" should name a directory.\n",
            envVar);

    return false;
}


/**
 * Simulator constructor.
 */
Micropolis::Micropolis(MicropolisDelegate* micropolisDelegate) :
        delegate(micropolisDelegate),
        populationDensityMap(0),
        trafficDensityMap(0),
        pollutionDensityMap(0),
        landValueMap(0),
        crimeRateMap(0),
        terrainDensityMap(0),
        tempMap1(0),
        tempMap2(0),
        tempMap3(0),
        powerGridMap(0),
        rateOfGrowthMap(0),
        fireStationMap(0),
        fireStationEffectMap(0),
        policeStationMap(0),
        policeStationEffectMap(0),
        comRateMap(0)
{
    roadTotal = 0;
    railTotal = 0;
    firePop = 0;
    resPop = 0;
    comPop = 0;
    indPop = 0;
    totalPop = 0;
    totalPopLast = 0;
    resZonePop = 0;
    comZonePop = 0;
    indZonePop = 0;
    totalZonePop = 0;
    hospitalPop = 0;
    churchPop = 0;
    faith = 0;
    stadiumPop = 0;
    policeStationPop = 0;
    fireStationPop = 0;
    coalPowerPop = 0;
    nuclearPowerPop = 0;
    seaportPop = 0;
    airportPop = 0;
    needHospital = 0;
    needChurch = 0;
    crimeAverage = 0;
    pollutionAverage = 0;
    landValueAverage = 0;
    cityTime = 0;
    cityMonth = 0;
    cityYear = 0;
    startingYear = 0;
    memset(map, 0, sizeof(short *) * WORLD_W);

    resHist10Max = 0;
    resHist120Max = 0;
    comHist10Max = 0;
    comHist120Max = 0;
    indHist10Max = 0;
    indHist120Max = 0;

    censusChanged = false;
    roadSpend = 0;
    policeSpend = 0;
    fireSpend = 0;
    roadFund = 0;
    policeFund = 0;
    fireFund = 0;
    roadEffect   = 0;
    policeEffect = 0;
    fireEffect   = 0;
    taxFund = 0;
    cityTax = 0;
    taxFlag = false;

    populationDensityMap.clear();
    trafficDensityMap.clear();
    pollutionDensityMap.clear();
    landValueMap.clear();
    crimeRateMap.clear();
    powerGridMap.clear();
    terrainDensityMap.clear();
    rateOfGrowthMap.clear();
    fireStationMap.clear();
    fireStationEffectMap.clear();
    policeStationMap.clear();
    policeStationEffectMap.clear();
    comRateMap.clear();

    mapBase = NULL;
    resHist = NULL;
    comHist = NULL;
    indHist = NULL;
    moneyHist = NULL;
    pollutionHist = NULL;
    crimeHist = NULL;
    miscHist = NULL;
    roadPercent = (float)0.0;
    policePercent = (float)0.0;
    firePercent = (float)0.0;
    roadValue = 0;
    policeValue = 0;
    fireValue = 0;
    mustDrawBudget = 0;
    floodCount = 0;

    cityYes = 0;

    // short problemVotes[PROBNUM]; /* these are the votes for each  */
    memset(problemVotes, 0, sizeof(short) * PROBNUM);

    // short problemOrder[CVP_PROBLEM_COMPLAINTS]; /* sorted index to above  */
    memset(problemOrder, 0, sizeof(short) * CVP_PROBLEM_COMPLAINTS);

    cityPop = 0;
    cityPopDelta = 0;
    cityAssessedValue = 0;
    cityClass = CC_VILLAGE;
    cityScore = 0;
    cityScoreDelta = 0;
    trafficAverage = 0;

    /* level for tree creation */
    terrainTreeLevel = -1;

    /* level for lake creation */
    terrainLakeLevel = -1;

    /* level for river curviness */
    terrainCurveLevel = -1;

    /* -1 => 10%, 0 => never, 1 => always */
    terrainCreateIsland = -1;

    graph10Max = 0;
    graph120Max = 0;

    simLoops = 0;
    simPasses = 0;
    simPass = 0;

    simPaused = false; // Simulation is running
    simPausedSpeed = 3;

    heatSteps = 0;
    heatFlow = -7;
    heatRule = 0;
    heatWrap = 3;

    cityFileName = "";
    cityName = "";

    tilesAnimated = false;

    doAnimation = true;
    doMessages = true;
    doNotices = true;

    cellSrc = NULL;
    cellDst = NULL;

#if 0

    // Disabled this small map drawing, filtering and overlaying code.
    // Going to re-implement it in the tile engine and Python.


    // int dynamicData[32];
    memset(dynamicData, 0, sizeof(int) * 32);

#endif

    cityPopLast = 0;
    categoryLast = 0;
    autoGoto = false;
    powerStackPointer = 0;

    for (int i = 0; i < POWER_STACK_SIZE; i++) {
        powerStackXY[i] = Position();
    }

    nextRandom = 1;
    homeDir = "";
    resourceDir = "";
    resources = NULL;
    stringTables = NULL;
    newMap = 0;

    memset(newMapFlags, 0, sizeof(short) * MAP_TYPE_COUNT);

    cityCenterX = 0;
    cityCenterY = 0;
    pollutionMaxX = 0;
    pollutionMaxY = 0;
    crimeMaxX = 0;
    crimeMaxY = 0;
    donDither = 0;
    valveFlag = false;
    crimeRamp = 0;
    pollutionRamp = 0;

    resCap = false; // Do not block residential growth
    comCap = false; // Do not block commercial growth
    indCap = false; // Do not block industrial growth

    cashFlow = 0;
    externalMarket = (float)4.0;
    disasterWait = 0;
    scoreWait = 0;
    poweredZoneCount = 0;
    unpoweredZoneCount = 0;
    newPower = false;
    cityTaxAverage = 0;
    simCycle = 0;
    phaseCycle = 0;
    speedCycle = 0;
    doInitialEval = false;
    mapSerial = 1;
    resValve = 0;
    comValve = 0;
    indValve = 0;
    nextSpriteID = 1;

    memset(globalSprites, 0, sizeof(SimSprite *) * SPRITE_COUNT);

    absDist = 0;
    spriteCycle = 0;
    totalFunds = 0;
    autoBulldoze = true;
    autoBudget = true;

    gameLevel = LEVEL_EASY;
    scenario = NULL;
    simSpeed = 0;
    simSpeedMeta = 0;
    enableSound = false;
    enableDisasters = true;
    evalChanged = false;
    blinkFlag = 0;
    curMapStackPointer = 0;

    // Position curMapStackXY[MAX_TRAFFIC_DISTANCE+1];
    for (int i = 0; i < MAX_TRAFFIC_DISTANCE + 1; i++) {
        curMapStackXY[i] = Position();
    }

    trafMaxX = 0;
    trafMaxY = 0;

    mustUpdateFunds = false;
    mustUpdateOptions = false;

    cityTimeLast = 0;
    cityYearLast = 0;
    cityMonthLast = 0;

    totalFundsLast = 0;
    resLast = 0;
    comLast = 0;
    indLast = 0;

    /** Initialize for a simulation */
    setEnableSound(true); // Enable sound
    mustUpdateOptions = true; // Update options displayed at user
    startingYear = 1900;
    simPasses = 1;
    simPass = 0;
    setAutoGoto(true); // Enable auto-goto
    setCityTax(7);
    cityTime = 50;
    setEnableDisasters(true); // Enable disasters
    setAutoBulldoze(true); // Enable auto bulldoze
    setAutoBudget(true); // Enable auto-budget
    blinkFlag = 1;
    simSpeed = 3;
    changeEval();
    simPaused = false; // Simulation is running
    simLoops = 0;
    initSimLoad = SIMLOADER_GENERATECITY;

    // initMapArrays:
    /** Allocate and initialize arrays for the maps */

    if (!mapBase) {
        mapBase = (unsigned short *)malloc(
            sizeof(unsigned short) *
            WORLD_W * WORLD_H);
    }

    for (short i = 0; i < WORLD_W; i++) {
        map[i] = (unsigned short *)(mapBase + (i * WORLD_H));
    }

    resHist = (short *)malloc(HISTORY_LENGTH);
    comHist = (short *)malloc(HISTORY_LENGTH);
    indHist = (short *)malloc(HISTORY_LENGTH);
    moneyHist = (short *)malloc(HISTORY_LENGTH);
    pollutionHist = (short *)malloc(HISTORY_LENGTH);
    crimeHist = (short *)malloc(HISTORY_LENGTH);
    miscHist = (short *)malloc(MISC_HISTORY_LENGTH);
    
    
    initGraphs();
    initFundingLevel();
    resetMapState();
    resetEditorState();
    clearMap();
    initWillStuff();
    setFunds(5000);
    setGameLevelFunds(LEVEL_EASY);
    setSpeed(0);
    setPasses(1);
}

/** Simulator destructor. */
Micropolis::~Micropolis()
{
    // TODO: release delegate
    
    /** Free all map arrays */

    if (mapBase != NULL) {
        free(mapBase);
        mapBase = NULL;
    }

    memset(map, 0, sizeof(short *) * WORLD_W);

    populationDensityMap.clear();
    trafficDensityMap.clear();
    pollutionDensityMap.clear();
    landValueMap.clear();
    crimeRateMap.clear();

    tempMap1.clear();
    tempMap2.clear();
    tempMap3.clear();

    terrainDensityMap.clear();

    if (resHist != NULL) {
        free(resHist);
        resHist = NULL;
    }

    if (comHist != NULL) {
        free(comHist);
        comHist = NULL;
    }

    if (indHist != NULL) {
        free(indHist);
        indHist = NULL;
    }

    if (moneyHist != NULL) {
        free(moneyHist);
        moneyHist = NULL;
    }

    if (pollutionHist != NULL) {
        free(pollutionHist);
        pollutionHist = NULL;
    }

    if (crimeHist != NULL) {
        free(crimeHist);
        crimeHist = NULL;
    }

    if (miscHist != NULL) {
        free(miscHist);
        miscHist = NULL;
    }

    // TODO: Clean up all other stuff:
}

/**
 * Get version of Micropolis program.
 * @todo Use this function or eliminate it.
 * @return Textual version.
 */
const char *Micropolis::getMicropolisVersion()
{
    return MICROPOLIS_VERSION;
}


/** Locate resource directory. */
void Micropolis::environmentInit()
{
    const char *s = getenv("SIMHOME");
    if (s == NULL) {
        s = ".";
    }
    homeDir = s;

    if (testDirectory(homeDir, "$SIMHOME")) {

        resourceDir = homeDir + "/res/";
        if (testDirectory(resourceDir, "$SIMHOME/res")) {

            return; // All ok
        }
    }

    // Failed on $SIMHOME, ".", or the 'res' directory.
    fprintf(stderr,
            "Please check the environment or reinstall Micropolis and try again! Sorry!\n");
    exit(1);
}


/**
 * Update ????
 * @todo What is the purpose of this function? (also in relation with
 *       Micropolis::simTick()).
 */
void Micropolis::simUpdate()
{
    blinkFlag = ((tickCount() % 60) < 30) ? 1 : -1;

    if (simSpeed && !heatSteps) {
      tilesAnimated = false;
    }

    doUpdateHeads();
    graphDoer();
    updateBudget();
    scoreDoer();
}


/**
 * ????
 * @todo Why is Micropolis::cellSrc not allocated together with all the other
 *       variables?
 * @todo What is the purpose of this function?
 * @todo KILL the define.
 */
void Micropolis::simHeat()
{
    int x, y;
    static int a = 0;
    short *src, *dst;
    int fl = heatFlow;

    const int SRCCOL = WORLD_H + 2;
    const int DSTCOL = WORLD_H;


    if (cellSrc == NULL) {
        cellSrc = (short *)malloc((WORLD_W + 2) * (WORLD_H + 2) * sizeof (short));
        cellDst = (short *)&map[0][0];
    }

    src = cellSrc + SRCCOL + 1;
    dst = cellDst;

    /*
     * Copy wrapping edges:
     *
     *  0   ff  f0 f1 ... fe ff     f0
     *
     *  1   0f  00 01 ... 0e 0f     00
     *  2   1f  10 11 ... 1e 1f     10
     *      ..  .. ..     .. ..     ..
     *      ef  e0 e1 ... ee ef     e0
     *  h   ff  f0 f1 ... fe ff     f0
     *
     *  h+1 0f  00 01 ... 0e 0f     00
     *
     * wrap value:  effect:
     *  0   no effect
     *  1   copy future=>past, no wrap
     *  2   no copy, wrap edges
     *  3   copy future=>past, wrap edges
     *  4   copy future=>past, same edges
     */

    switch (heatWrap) {
        case 0:
            break;
        case 1:
            for (x = 0; x < WORLD_W; x++) {
                memcpy(src, dst, WORLD_H * sizeof (short));
                src += SRCCOL;
                dst += DSTCOL;
            }
            break;
        case 2:
            for (x = 0; x < WORLD_W; x++) {
                src[-1] = src[WORLD_H - 1];
                src[WORLD_H] = src[0];
                src += SRCCOL;
                dst += DSTCOL;
            }
            memcpy(
                cellSrc,
                cellSrc + (SRCCOL * WORLD_W),
                SRCCOL * sizeof (short));
            memcpy(
                cellSrc + SRCCOL * (WORLD_W + 1),
                cellSrc + SRCCOL,
                SRCCOL * sizeof (short));
            break;
        case 3:
            for (x = 0; x < WORLD_W; x++) {
                memcpy(src, dst, WORLD_H * sizeof (short));
                src[-1] = src[WORLD_H - 1];
                src[WORLD_H] = src[0];
                src += SRCCOL;
                dst += DSTCOL;
            }
            memcpy(
                cellSrc,
                cellSrc + (SRCCOL * WORLD_W),
                SRCCOL * sizeof (short));
            memcpy(
                cellSrc + SRCCOL * (WORLD_W + 1),
                cellSrc + SRCCOL,
                SRCCOL * sizeof (short));
            break;
        case 4:
            src[0] =
                dst[0];
            src[1 + WORLD_H] =
                dst[WORLD_H - 1];
            src[(1 + WORLD_W) * SRCCOL] =
                dst[(WORLD_W - 1) * DSTCOL];
            src[((2 + WORLD_W) * SRCCOL) - 1] =
                dst[(WORLD_W * WORLD_H) - 1];
            for (x = 0; x < WORLD_W; x++) {
                memcpy(src, dst, WORLD_H * sizeof (short));
                src[-1] = src[0];
                src[WORLD_H] =  src[WORLD_H - 1];
                src += SRCCOL;
                dst += DSTCOL;
            }
            memcpy(
                cellSrc + (SRCCOL * (WORLD_W + 1)),
                cellSrc + (SRCCOL * WORLD_W),
                SRCCOL * sizeof (short));
            memcpy(
                cellSrc,
                cellSrc + SRCCOL,
                SRCCOL * sizeof (short));
            break;
        default:
            NOT_REACHED();
            break;
    }


#define CLIPPER_LOOP_BODY(CODE) \
    src = cellSrc; dst = cellDst; \
    for (x = 0; x < WORLD_W;) { \
        short nw, n, ne, w, c, e, sw, s, se; \
        src = cellSrc + (x * SRCCOL); \
        dst = cellDst + (x * DSTCOL); \
        w = src[0]; c = src[SRCCOL]; e = src[2 * SRCCOL]; \
        sw = src[1]; s = src[SRCCOL + 1]; se = src[(2 * SRCCOL) + 1]; \
        for (y = 0; y < WORLD_H; y++) { \
            nw = w; w = sw; sw = src[2]; \
            n = c; c = s; s = src[SRCCOL + 2]; \
            ne = e; e = se; se = src[(2 * SRCCOL) + 2]; \
            { CODE } \
            src++; dst++; \
        } \
        x++; \
        src = cellSrc + ((x + 1) * SRCCOL) - 3; \
        dst = cellDst + ((x + 1) * DSTCOL) - 1; \
        nw = src[1]; n = src[SRCCOL + 1]; ne = src[(2 * SRCCOL) + 1]; \
        w = src[2]; c = src[SRCCOL + 2]; e = src[(2 * SRCCOL) + 2]; \
        for (y = WORLD_H - 1; y >= 0; y--) { \
            sw = w; w = nw; nw = src[0]; \
            s = c; c = n; n = src[SRCCOL]; \
            se = e; e = ne; ne = src[2 * SRCCOL]; \
            { CODE } \
            src--; dst--; \
        } \
        x++; \
    }


    switch (heatRule) {

      case 0:

#define HEAT \
    a += nw + n + ne + w + e + sw + s + se + fl; \
    dst[0] = ((a >> 3) & LOMASK) | ANIMBIT | BURNBIT | BULLBIT; \
    a &= 7;

        CLIPPER_LOOP_BODY(HEAT);
        break;

      case 1:

#define ECOMASK 0x3fc
#define ECO \
    { \
        c -= fl; n -= fl; s -= fl; e -= fl; w -= fl; \
        ne -= fl; nw -= fl; se -= fl; sw -= fl; \
        /* anneal */ \
        int sum = \
            (c&1) + (n&1) + (s&1) + (e&1) + (w&1) + \
            (ne&1) + (nw&1) + (se&1) + (sw&1), cell; \
        if (((sum > 5) || (sum == 4))) { \
            /* brian's brain */ \
            cell = \
                ((c <<1) & (0x3fc)) | \
                (((((c >>1)&3) == 0) && \
                  (((n&2) + (s&2) + (e&2) + (w&2) + \
                    (ne&2) + (nw&2) + (se&2) + (sw&2)) == (2 <<1))      \
                 ) ? 2 : 0) | \
                 1; \
        } else { \
            /* anti-life */ \
            sum = \
                ((n&2) + (s&2) + (e&2) + (w&2) + \
                 (ne&2) + (nw&2) + (se&2) + (sw&2)) >>1; \
            cell = \
                (((c ^ 2) <<1) & ECOMASK) | \
                ((c&2) \
                   ? ((sum != 5) ? 2 : 0) \
                   : (((sum != 5) && (sum != 6)) ? 2 : 0)); \
        } \
        dst[0] = \
            ((fl + cell) & LOMASK) | ANIMBIT | BURNBIT | BULLBIT; \
        c += fl; n += fl; s += fl; e += fl; w += fl; \
        ne += fl; nw += fl; se += fl; sw += fl; \
    }

        CLIPPER_LOOP_BODY(ECO);

        break;

        default:
            NOT_REACHED();
            break;
    }
}


void Micropolis::simLoop(bool doSim)
{
   if (heatSteps) {
       for (int j = 0; j < heatSteps; j++) {
           simHeat();
       }

       moveObjects();
       simRobots();

       newMap = 1;

   } else {
       if (doSim) {
           simFrame();
       }

       moveObjects();
       simRobots();
   }

   simLoops++;
}


/**
 * Move simulaton forward.
 * @todo What is the purpose of this function? (also in relation with
 *       Micropolis::simUpdate()).
 */
void Micropolis::simTick()
{
    if (simSpeed) {
        for (simPass = 0; simPass < simPasses; simPass++) {
            simLoop(true);
        }
    }
    simUpdate();
}


void Micropolis::simRobots()
{
    if (delegate) delegate->doRobots(this);
}
