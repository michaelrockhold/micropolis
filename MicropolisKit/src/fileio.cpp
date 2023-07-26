/* fileio.cpp
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

/** @file fileio.cpp */

////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "micropolis.h"
#include <string>

////////////////////////////////////////////////////////////////////////

#ifdef IS_INTEL

/**
 * Convert an array of short values between MAC and Intel endian formats.
 * @param buf Array with shorts.
 * @param len Number of short values in the array.
 */
#define SWAP_SHORTS(buf, len)        swap_shorts(buf, len)

/**
 * Convert an array of long values between MAC and Intel endian formats.
 * @param buf Array with longs.
 * @param len Number of long values in the array.
 */
#define HALF_SWAP_LONGS(buf, len)    half_swap_longs(buf, len)

/**
 * Swap upper and lower byte of all shorts in the array.
 * @param buf Array with shorts.
 * @param len Number of short values in the array.
 */
static void swap_shorts(short *buf, int len)
{
    int i;

    /* Flip bytes in each short! */
    for (i = 0; i < len; i++) {
        *buf = ((*buf & 0xFF) <<8) | ((*buf &0xFF00) >>8);
        buf++;
    }
}


/**
 * Swap upper and lower words of all longs in the array.
 * @param buf Array with longs.
 * @param len Number of long values in the array.
 */
static void half_swap_longs(Quad *buf, int len)
{
    int i;

    /* Flip bytes in each long! */
    for (i = 0; i < len; i++) {
        Quad l = *buf;
        *buf = ((l & 0x0000ffff) << 16) | ((l & 0xffff0000) >> 16);
        buf++;
    }
}


#else


/**
 * Convert an array of short values between MAC and MAC endian formats.
 * @param buf Array with shorts.
 * @param len Number of short values in the array.
 * @note This version does not change anything since the data is already in the
 *       correct format.
 */
#define SWAP_SHORTS(buf, len)

/**
 * Convert an array of long values between MAC and MAC endian formats.
 * @param buf Array with longs.
 * @param len Number of long values in the array.
 * @note This version does not change anything since the data is already in the
 *       correct format.
 */
#define HALF_SWAP_LONGS(buf, len)


#endif

/**
 * Load an array of short values from file to memory.
 *
 * Convert to the correct processor architecture, if necessary.
 * @param buf Buffer to put the loaded short values in.
 * @param len Number of short values to load.
 * @param f   File handle of the file to load from.
 * @return Load was succesfull.
 */
static bool load_short(short *buf, int len, FILE *f)
{
    size_t result = fread(buf, sizeof(short), len, f);

    if ((int)result != len) {
         return false;
    }

    SWAP_SHORTS(buf, len);        /* to intel */

    return true;
}

static void load_shorts_from_buffer(short *buf, size_t len, const unsigned short *src)
{
    memcpy(buf, src, sizeof(short) * len);
    SWAP_SHORTS(buf, len);        /* to intel */ // TODO: Use conventional ntoh functions
}


/**
 * Save an array of short values from memory to file.
 *
 * Convert to the correct endianness first, if necessary.
 * @param buf Buffer containing the short values to save.
 * @param len Number of short values to save.
 * @param f   File handle of the file to save to.
 * @return Save was succesfull.
 */
static bool save_short(short *buf, int len, FILE *f)
{
    SWAP_SHORTS(buf, len);        /* to MAC */

    if ((int)fwrite(buf, sizeof(short), len, f) != len) {
        return false;
    }

    SWAP_SHORTS(buf, len);        /* back to intel */

    return true;
}

void Micropolis::loadCityFromData(const unsigned short * cityData) {
    
    size_t sz = HISTORY_LENGTH / sizeof(short);
    size_t offset = 0;
    load_shorts_from_buffer(resHist, sz, cityData+offset);
    offset += sz;
    
    load_shorts_from_buffer(comHist, sz, cityData+offset);
    offset += sz;
    
    load_shorts_from_buffer(indHist, sz, cityData+offset);
    offset += sz;
    
    load_shorts_from_buffer(crimeHist, sz, cityData+offset);
    offset += sz;
    
    load_shorts_from_buffer(pollutionHist, sz, cityData+offset);
    offset += sz;
    
    load_shorts_from_buffer(moneyHist, sz, cityData+offset);
    offset += sz;
    
    sz = MISC_HISTORY_LENGTH / sizeof(short);
    load_shorts_from_buffer(miscHist, sz, cityData+offset);
    offset += sz;
    
    sz = WORLD_W * WORLD_H;
    load_shorts_from_buffer(((short *)&map[0][0]), sz, cityData+offset);
    offset += sz;
}

/**
 * Load a city file from a given filename and (optionally) directory.
 * @param filename Name of the file to load.
 * @param dir      If not \c NULL, name of the directory containing the file.
 * @return Load was succesfull.
 */
bool Micropolis::loadFileDir(const char *filename, const char *dir)
{
    char *path = NULL;
    FILE *f;
    Quad size;

    // If needed, construct a path to the file.
    if (dir != NULL) {
        size_t pathSize = strlen(dir) + 1 + strlen(filename) + 1;
        path = (char *)malloc(pathSize);
        snprintf(path, pathSize, "%s/%s", dir, filename);
        filename = path;
    }

    // Open the file.
    f = fopen(filename, "rb");

    // Before checking whether open() succeeded, first drop the path.
    if (path != NULL) {
        free(path);
    }

    // open() failed; report failure.
    if (f == NULL) {
        return false;
    }

    fseek(f, 0L, SEEK_END);
    size = (Quad)ftell(f);
    fseek(f, 0L, SEEK_SET);

    bool result =
      (size == 27120) &&
      load_short(resHist, HISTORY_LENGTH / sizeof(short), f) &&
      load_short(comHist, HISTORY_LENGTH / sizeof(short), f) &&
      load_short(indHist, HISTORY_LENGTH / sizeof(short), f) &&
      load_short(crimeHist, HISTORY_LENGTH / sizeof(short), f) &&
      load_short(pollutionHist, HISTORY_LENGTH / sizeof(short), f) &&
      load_short(moneyHist, HISTORY_LENGTH / sizeof(short), f) &&
      load_short(miscHist, MISC_HISTORY_LENGTH / sizeof(short), f) &&
      load_short(((short *)&map[0][0]), WORLD_W * WORLD_H, f);

    fclose(f);

    return result;
}

/**
 * Load a file, and initialize the game variables.
 * @param filename Name of the file to load.
 * @return Load was succesfull.
 */
bool Micropolis::loadFile(const char *filename)
{
    Quad n;

    if (!loadFileDir(filename, NULL)) {
        return false;
    }

    /* total funds is a long.....    miscHist is array of shorts */
    /* total funds is being put in the 50th & 51th word of miscHist */
    /* find the address, cast the ptr to a longPtr, take contents */

    n = *(Quad *)(miscHist + 50);
    HALF_SWAP_LONGS(&n, 1);
    setFunds(n);

    n = *(Quad *)(miscHist + 8);
    HALF_SWAP_LONGS(&n, 1);
    cityTime = n;

    setAutoBulldoze(miscHist[52] != 0);   // flag for autoBulldoze
    setAutoBudget(miscHist[53] != 0);     // flag for autoBudget
    setAutoGoto(miscHist[54] != 0);       // flag for auto-goto
    setEnableSound(miscHist[55] != 0);    // flag for the sound on/off
    setCityTax(miscHist[56]);
    setSpeed(miscHist[57]);
    changeCensus();
    mustUpdateOptions = true;

    /* yayaya */

    n = *(Quad *)(miscHist + 58);
    HALF_SWAP_LONGS(&n, 1);
    policePercent = ((float)n) / ((float)65536);

    n = *(Quad *)(miscHist + 60);
    HALF_SWAP_LONGS(&n, 1);
    firePercent = (float)n / (float)65536.0;

    n = *(Quad *)(miscHist + 62);
    HALF_SWAP_LONGS(&n, 1);
    roadPercent = (float)n / (float)65536.0;

    policePercent =
        (float)(*(Quad*)(miscHist + 58)) /
        (float)65536.0;   /* and 59 */
    firePercent =
        (float)(*(Quad*)(miscHist + 60)) /
        (float)65536.0;   /* and 61 */
    roadPercent =
        (float)(*(Quad*)(miscHist + 62)) /
        (float)65536.0;   /* and 63 */

    cityTime = max((Quad)0, cityTime);

    // If the tax is nonsensical, set it to a reasonable value.
    if (cityTax > 20 || cityTax < 0) {
        setCityTax(7);
    }

    // If the speed is nonsensical, set it to a reasonable value.
    if (simSpeed < 0 || simSpeed > 3) {
        setSpeed(3);
    }

    setSpeed(simSpeed);
    setPasses(1);
    initFundingLevel();

    // Set the scenario id to 0.
    initWillStuff();
    initSimLoad = SIMLOADER_FROMFILE;
    doInitialEval = false;
    doSimInit();
    invalidateMaps();

    return true;
}


/**
 * Save a game to disk.
 * @param filename Name of the file to use for storing the game.
 * @return The game was saved successfully.
 */
bool Micropolis::saveFile(const char *filename)
{
    Quad n;
    FILE *f;

    if ((f = fopen(filename, "wb")) == NULL) {
        /// @todo Report error saving file.
        return false;
    }

    /* total funds is a long.....    miscHist is array of ints */
    /* total funds is bien put in the 50th & 51th word of miscHist */
    /* find the address, cast the ptr to a longPtr, take contents */

    n = totalFunds;
    HALF_SWAP_LONGS(&n, 1);
    (*(Quad *)(miscHist + 50)) = n;

    n = cityTime;
    HALF_SWAP_LONGS(&n, 1);
    (*(Quad *)(miscHist + 8)) = n;

    miscHist[52] = autoBulldoze;   // flag for autoBulldoze
    miscHist[53] = autoBudget;     // flag for autoBudget
    miscHist[54] = autoGoto;       // flag for auto-goto
    miscHist[55] = enableSound;    // flag for the sound on/off
    miscHist[57] = simSpeed;
    miscHist[56] = cityTax;        /* post release */

    /* yayaya */

    n = (int)(policePercent * 65536);
    HALF_SWAP_LONGS(&n, 1);
    (*(Quad *)(miscHist + 58)) = n;

    n = (int)(firePercent * 65536);
    HALF_SWAP_LONGS(&n, 1);
    (*(Quad *)(miscHist + 60)) = n;

    n = (int)(roadPercent * 65536);
    HALF_SWAP_LONGS(&n, 1);
    (*(Quad *)(miscHist + 62)) = n;

    bool result =
        save_short(resHist, HISTORY_LENGTH / 2, f) &&
        save_short(comHist, HISTORY_LENGTH / 2, f) &&
        save_short(indHist, HISTORY_LENGTH / 2, f) &&
        save_short(crimeHist, HISTORY_LENGTH / 2, f) &&
        save_short(pollutionHist, HISTORY_LENGTH / 2, f) &&
        save_short(moneyHist, HISTORY_LENGTH / 2, f) &&
        save_short(miscHist, MISC_HISTORY_LENGTH / 2, f) &&
        save_short(((short *)&map[0][0]), WORLD_W * WORLD_H, f);

    fclose(f);

    return result;
}


/**
 * Load a scenario.
 * @param scenario Scenario to load.
 * @note \a s cannot be \c SC_NONE.
 */
void Micropolis::loadScenario(Scenario* scenario)
{
    this->scenario = *scenario;
    cityFileName = "";

    setGameLevel(LEVEL_EASY);

    cityTime = scenario->cityTime;
    setFunds(scenario->funds);

    setCleanCityName(scenario->name);
    setSpeed(3);
    setCityTax(7);

    loadCityFromData((const unsigned short*)scenario->cityData);

    initWillStuff();
    initFundingLevel();
    updateFunds();
    invalidateMaps();
    initSimLoad = SIMLOADER_FROMFILE;
    doInitialEval = false;
    doSimInit();
    didLoadScenario();
}

/** Report to the front-end that the scenario was loaded. */
void Micropolis::didLoadScenario()
{
    delegate->didLoadScenario(this);
}

/**
 * Try to load a new game from disk.
 * @param filename Name of the file to load.
 * @return Game was loaded successfully.
 * @todo In what state is the game left when loading fails?
 * @todo String normalization code is duplicated in Micropolis::saveCityAs().
 *       Extract to a sub-function.
 * @bug Function fails if \c lastDot<lastSlash (ie with \c "x.y/bla" )
 */
bool Micropolis::loadCity(const char *filename)
{
    if (loadFile(filename)) {

        cityFileName = filename;

        std::string::size_type lastSlash = cityFileName.find_last_of('/');
        std::string::size_type pos = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;

        std::string::size_type lastDot = cityFileName.find_last_of('.');
        std::string::size_type last =
            (lastDot == std::string::npos) ? cityFileName.length() : lastDot;

        std::string newCityName = cityFileName.substr(pos, last - pos);
        setCityName(newCityName);

        didLoadCity();

        return true;

    } else {

        didntLoadCity((filename && *filename) ? filename : "(null)");

        return false;

    }
}

/** Report to the frontend that the game was successfully loaded. */
void Micropolis::didLoadCity()
{
    delegate->didLoadCity(this);
}


/**
 * Report to the frontend that the game failed to load.
 * @param msg File that attempted to load
 */
void Micropolis::didntLoadCity(const char *msg)
{
    delegate->didntLoadCity(this, msg);
}


/**
 * Try to save the game.
 * @todo This is a no-op if the Micropolis::cityFileName is empty.
 *       In that case, we should probably warn the user about the failure.
 */
void Micropolis::saveCity()
{
    if (cityFileName.length() == 0) {

        doSaveCityAs();

    } else {
        if (saveFile(cityFileName.c_str())) {

            didSaveCity();

        } else {

            didntSaveCity(cityFileName.c_str());

        }
    }
}


/** Report to the frontend that the city is being saved. */
void Micropolis::doSaveCityAs()
{
    delegate->doSaveCityAs(this);
}


/** Report to the frontend that the city was saved successfully. */
void Micropolis::didSaveCity()
{
    delegate->didSaveCity(this);
}


/**
 * Report to the frontend that the city could not be saved.
 * @param msg Name of the file used
 */
void Micropolis::didntSaveCity(const char *msg)
{
    delegate->didntSaveCity(this, msg);
}


/**
 * Save the city under a new name (?)
 * @param filename Name of the file to use for storing the game.
 * @todo String normalization code is duplicated in Micropolis::loadCity().
 *       Extract to a sub-function.
 * @bug Function fails if \c lastDot<lastSlash (ie with \c "x.y/bla" )
 */
void Micropolis::saveCityAs(const char *filename)
{
    cityFileName = filename;

    if (saveFile(cityFileName.c_str())) {

        std::string::size_type lastDot = cityFileName.find_last_of('.');
        std::string::size_type lastSlash = cityFileName.find_last_of('/');

        std::string::size_type pos = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
        std::string::size_type last = (lastDot == std::string::npos) ? cityFileName.length() : lastDot;
        std::string::size_type len = last - pos;
        std::string newCityName = cityFileName.substr(pos, len);

        setCityName(newCityName);

        didSaveCity();

    } else {

        didntSaveCity(cityFileName.c_str());

    }
}


////////////////////////////////////////////////////////////////////////