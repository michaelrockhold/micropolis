//
//  MicropolisDelegate.hh
//  micropolis-engine
//
//  Created by Michael Rockhold on 2/14/23.
//

#ifndef MicropolisDelegate_hh
#define MicropolisDelegate_hh

#import <MicropolisKit/text.h>

class Micropolis;

/**
  * Abstract interface for interacting with the front-end components
  * Roughly equivalent to the functionality of the userData/callbackHook/callbackData
  * mechanism in the old code.
 */
class MicropolisDelegate {
public:
    virtual void doPlayNewCity(Micropolis* sender) = 0;
    virtual void doEarthquake(Micropolis* sender, int strength) = 0;
    virtual void update(Micropolis* sender, const char * target) = 0;
    virtual void makeSound(Micropolis* sender, const char *channel, const char *sound,int x, int y) = 0;
    virtual void doRobots(Micropolis* sender) = 0;
    virtual void doSimulateChurch(Micropolis* sender, short x, short y, int churchNumber) = 0;
    virtual void sendMessage(Micropolis* sender, enum MessageNumber mesgNum, short x, short y, bool picture, bool important) = 0;
    virtual void doAutoGoto(Micropolis* sender, short x, short y, const char *msg) = 0;
    virtual void doLoseGame(Micropolis* sender) = 0;
    virtual void doWinGame(Micropolis* sender) = 0;
    virtual void showBudgetAndWait(Micropolis* sender) = 0;
    virtual void didLoadScenario(Micropolis* sender) = 0;
    virtual void didLoadCity(Micropolis* sender) = 0;
    virtual void didntLoadCity(Micropolis* sender, const char* msg) = 0;
    virtual void doSaveCityAs(Micropolis* sender) = 0;
    virtual void didSaveCity(Micropolis* sender) = 0;
    virtual void didntSaveCity(Micropolis* sender, const char* msg) = 0;
    virtual void doNewGame(Micropolis* sender) = 0;
    virtual void doShowZoneStatus(Micropolis* sender, int tileCategory, int s0, int s1, int s2, int s3, int s4, int x, int y) = 0;
    virtual void didTool(Micropolis* sender, const char *name, short x, short y) = 0;
    virtual void didGenerateMap(Micropolis* sender) = 0;
    
    virtual void newSprite(Micropolis* sender, int sprite_id) = 0;
    virtual void destroySprite(Micropolis* sender, int sprite_id) = 0;
};

#endif /* MicropolisDelegate_hh */
