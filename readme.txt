Super simple city builder game built when playing around and learning SFML2.  This is an isometric game where you can make a city from a 2D grid of tiles that evolves over time.  Player can bulldoze areas and place new tiles and then watch as their population grows over time.  Actions are performed using city funds.  Tiles are grass, forest, water, roads, and zones (residential, commercial, industrial).  There is room for a lot of new tile types!  

Residential acts as housing for population, commercial zones sell goods and employ people, industrial zones supply goods to commercial zones and employ people.  Goods are transported via roads, while all other tiles act as restrictions for what can be placed there and otherwise are aesthetic.

There's some weirdness with the way populations are handled, so you'll see some things that don't make sense in that regard.  Need to fix that.

To download and setup:
1) Download the full repository.
2) Extract the SFML-2.4.2 zip in place
3) Build the project in VS2017 (if you want to build using something else you could download the correct version of SFML and overwrite the version in the project.  Should work fine).
4) Run the project

Playing the game:
Click the 'load game' button to start (it will autoload the map I've set up).  Middle mouse button pans, right click opens the build menu.  Left click to select the item you want to build in the menu, then left click and drag to build it, or right click to cancel building.  All zones should be connected with roads.
The info panel at the bottom shows your current money, the 'day' (bad measure, need to figure something better out) and then your:
Current Population (Homeless Population) and then Current Employees (Unemployed)
