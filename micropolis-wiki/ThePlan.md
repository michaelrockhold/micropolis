# Plan for developing Micropolis for OLPC. #

_(This page is several years old. A lot of progress rewriting the code has been made since the plan described below, but there is still a lot of exciting work to do towards the higher level stuff.)_

First, get it running on the current OLPC platform.

DONE:

  * Screen depth and size: fixed size of OLPC screen.
  * Window management: one window.
  * Simplify window management. Single map and editor view. Tiled graph and evaluation windows.
  * Rearrange startup screen so it works in the full window mode.
  * Make budget window and file dialogs be transient popup windows.
  * Add "Enable/Disable Auto Budget" button to budget window.
  * Click on small R/C/I gauge to toggle evaluation window.
  * Click on small graph to toggle graph window.
  * Click on funds or tax rate to show budget window.
  * Disable multi player features by default (added -m flag to enable multi player mode).
  * Disable other experimental features (cellular automata, dynamic query, etc).
  * Add -S flag to enable Sugar features, and TCL access to test flag.
  * Enable audio: Use Sugar audio server.
    * Convert sounds to new audio format (currently wav).
  * Fix fonts.
    * Pick nice Sugar fonts to use, based on Sugar flag.

TODO:

  * Update manual and documentation.
  * Make Sugar activity wrapper in Python.
  * Put new graphics and icons into interface and menus.

Long term plan for MicropolisCore:

  * Replace TCL/Tk with Python/GTK, and use Pango/Cairo for graphics.
    * Implement custom Tk components in GTK.
      * Map view.
      * Editor view.
      * Date view.
      * Graph view.
      * Pie menus.
      * Cellular Automata Machine view.
    * Integrate with Sugar and OLPC hardware.
      * Audio.
        * Support CSound audio mixer.
        * Switch to a smaller audio format.
        * Record better sounds.
      * Internationalization.
      * Graphics.
        * Hires monochrome.
        * SVG graphics and icons.
        * Create better icons.
      * Sugar integration.
        * Integrate with OLPC activity system, window manager, etc.
        * Rework user interface to follow Sugar user interface guidelines.
      * Keyboard control.
        * Support OLPC keyboard layout and device buttons.
        * Game keypad interface for book mode.
      * Optimization.
        * Optimize power management.
        * Optimize size.
        * Optimize speed.
      * Documentation.
        * Integrated help, tutorials, essays on city design from manual, etc.
        * Voiceovers, demos, journal playback.
        * Courseware.
        * Map annotation.
      * Journaling.
        * Event history.
        * Edit history.
        * Checkpointing.
        * Branching histories.
        * Rewinding and fast forwarding.
        * Publishing newspaper articles linking to save files.
        * URL to link into a city at a particular location on the map.
      * Grid networking.
        * Multi player stupport.
        * Sharing cities.
        * Colaborative editing.
        * Voting.
        * Chat.
        * White board.
      * Educational.
        * Bring old Micropolis courseware up to date, and integrate with the game.
        * Export simulation data to spreadsheet or xml.
        * Creative writing, storytelling, newspaper reporting, blogging, etc.
        * Scenarios and experiments.
        * What-if?
        * Branching history at decision point, and comparing different results.
        * Scripting.
        * Open up the simulator to Python.
        * Web services to remotely monitor and control simulation.
        * HTML or AJAX web server remote control interface.
          * Support multi player interactions through web server.
            * Submit a proposal to build a stadium over the web.
            * Style it like a real civic government web page, that allows citizens to participate online.
        * Enable extending the graphics, tiles, sprites.
        * Enable programming behaviors, defining new zones, new global variables, overlays, etc.
        * Cellular automata.
        * Visual programming.
        * Programming by example.
        * KidSim, AgentSheets.
        * Robot Odyssey.
    * Visual Programming
      * Simplify the Micropolis interface and make it easier for kids to use it with the game controller, in a way that will support multi player interaction.
      * Collapse the separate concepts of game editing tool (bulldozer, road, residential zone, etc) and agent (sprites like the monster, tornado, helicopter, train, etc).
      * Agents with specialized tool represent different roles that kids can play. A bunch of kids can join together and play different roles at the same time in the same city. Instead of having a bunch of editing tools to switch between, you have a bunch of different agents you can drive around the map, like using a monster to crush things instead of a bulldozer, or riding around in a helicopter to scroll around and observe the map. Make a meta-game like pokemon trading cards or magic the gathering, about acquiring and deploying and using agents on the map. Give agents different budgets and constraints.
      * Use an agent to represent a user in the world, and control an editing tool. You see other users in the map driving around their editing tool agents.
      * Each editing tool can be associated with a particular agent, with a keyboard/game controller based user interface for moving around, as well as a mouse based interface for picking it up and dragging it around.
      * The road tool becomes a road building vehicle, that you can easily move up/down/left/right/diagonally with the game controller directional input. Requires much less coordination to draw straight roads than with a mouse.
      * The bulldozer tool becomes an actual bulldozer that you can drive around the map, crushing things in your wake.
      * This makes the game easily usable by little kids in book mode.
      * Also support small children using Micropolis like a drawing tool or etch-a-sketch, simply doodling with the editing tools for the visceral pleasure of it, and setting fires and other disasters to watch it burn and mutate.
      * Logo Turtles (as a generalization of the monster, tornado, helicopter, etc)
        * Implement programmable logo turtles as agents that can move around on the map, sense it, and edit it.
        * Like Robot Odyssey agents, so you can go "inside" an agent, and travel around with it, operate its controls, read its sensors, and automate its behavior by wiring up visual programs with logic and math and nested "ic chip" components.
        * Plug in graphics to represent the agent: use classic logo turtle and Micropolis sprites, but also allow kids to plug in their own.
        * Micropolis sprites have 8 rotations.
        * SVG or Cairo drawings can be rotated continuously.
        * Re-implement the classic Micropolis agents like the monster, tornado, helicopter, train, etc in terms of logo turtles, that kids can drive around, learn to use, open up and modify (by turning internal tuning knobs, or even rewiring).
        * Let kids reprogram the agents to do all kinds of other stuff.
        * Mobile robots, that you can double click to open up into Robot-Odyssey-esque visual program editors.
        * Agents have local cellular-automata-like sensors to read information about the current and surrounding tiles.
        * KidSim / Cocoa / StageCraft Creator let kids define visual cellular automata rules by example, based on tile patterns and rules. Show it a pattern that you want to match by selecting an instance of that pattern in the world, then abstract it with wildcards if necessary, then demonstrate the result you want it to change the cell to in the next generation.
        * Sense high level information about zones and overlays, so the agents can base their behavior on any aspect of the world model.
        * Support an extensible model by allowing users to add more layers.
          * Add layers with arbitrary names and data types at different resolutions:
            * byte, int, float, n-dimensional vector, color, boolean mask, musical note, dict, parametric field (i.e. perlin noise or other mathematical function) at each cell, etc.
        * Edit the world.
        * All Micropolis editing tools (including colored pens that draw on overlays) should be available to the agent.
        * Enable users to plug in their own editing tools, that they can use themselves with the mouse, keyboard or game controller, or program agents to use to edit the map under program control.
      * Robot Odyssey
        * Build your own universal programmable editing tool.  Roll your own von Neuman Universal Constructor.  Smart robots you program to perform special purpose editing tasks.
        * The "Painter" picture editing program had a way of recording and playing back high level editing commands, relative to the current cursor position.
        * Remixing. Journaling. Programming by demonstration or example.  You could use a tape recorder to record a bunch of Micropolis editing commands that you act out (or you can just select them from the journal), then you can play those tapes back with relative coordinates, so they apply relative to where the agent currently is on the map. You can copy and paste and cut and splice any editing commands into tapes that you can use to program the robot to play back in arbitrary sequences.
        * Program an urban sprawl development-bot to lay out entire residential subdivisions, complete with zones, roads, parks and wires. Then program a luddite roomba-bot that sucks them all up and plants trees in their place.
        * This becomes really fun when we let players plug in their own programmed zones for the robot to lay out, and layers of data to control the robot's behavior, out of which they can program their own cellular automata rules and games (like KidSim / Cocoa / StageCraft Creator).