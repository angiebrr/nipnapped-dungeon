#include "main.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// Map.hpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Class that helps define a given map and its coloring for a level.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR
Map::Map(int width, int height) : width(width), height(height) 
{
    tiles = new Tile[width * height];
    walkMap = new TCODMap(width,height);
    
    // Create a binary space partition tree to partition the map.
    TCODBsp bsp(0, 0, width, height);
    
    // Splits the area up recursively into rectangle
    bsp.splitRecursive(NULL, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
    
    // Traverse tree with helper listener class, BspListener.
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, NULL);
}
    
// DESTRUCTOR
Map::~Map() 
{
    delete [] tiles;
    delete walkMap;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// RENDER()
// ----------------------------------------------------------------------------------------------------------------------------------
// Builds the map based on whether a tile is a wall or if it's the ground and colors the map accordingly.
// ==================================================================================================================================
void Map::render() const
{
    static const TCODColor darkWall(0, 0, 100);
    static const TCODColor darkGround(50, 50, 150);
    static const TCODColor lightWall(130, 110, 50);
    static const TCODColor lightGround(200, 180, 50);
    
    for (int x = 0; x < width; x++) 
    {
        for (int y = 0; y < height; y++) 
        {
            if( isInFov(x, y) )
            {
                if( isWall(x, y) )
                    TCODConsole::root->setCharBackground( x,y, lightWall);
                else
                    TCODConsole::root->setCharBackground( x,y, lightGround);
            }
            else
            {
                if( isWall(x, y) )
                    TCODConsole::root->setCharBackground( x,y, darkWall);
                else
                    TCODConsole::root->setCharBackground( x,y, darkGround);
            }
        }
    }
}

// ==================================================================================================================================
// COMPUTEFOV()
// ----------------------------------------------------------------------------------------------------------------------------------
// Updates the player's FOV.
// ==================================================================================================================================
void Map::computeFov() 
{
    walkMap->computeFov(engine.player->x, engine.player->y, engine.fovRadius);
}

// ==================================================================================================================================
// DIG()
// ----------------------------------------------------------------------------------------------------------------------------------
// Digs out rooms by changing tiles to walls.
// ==================================================================================================================================
void Map::dig(int x1, int y1, int x2, int y2)
{
    // Swap to make sure x2 > x1
    if (x2 < x1) 
    {
        int tmp = x2;
        x2 = x1;
        x1 = tmp;
    }
    // Swap to make sure y2 > y1
    if (y2 < y1) 
    {
        int tmp = y2;
        y2 = y1;
        y1 = tmp;
    }
    
    // Dig out rooms (or places we can walk) for the given area
    for (int tilex = x1; tilex <= x2; tilex++) 
    {
        for (int tiley = y1; tiley <= y2; tiley++)
            walkMap->setProperties(tilex, tiley, true, true); // x, y, transparent, walkable
    }
}

// ==================================================================================================================================
// CREATEROOM()
// ----------------------------------------------------------------------------------------------------------------------------------
// Dig out a room and place actors in them.
// ==================================================================================================================================
void Map::createRoom(bool first, int x1, int y1, int x2, int y2)
{
    dig (x1, y1, x2, y2);
    
    // If it's the first room, then we want to place the player in the center.
    if (first)
    {
        engine.player->x = (x1 + x2)/2;
        engine.player->y = (y1 + y2)/2;
    }
    // Otherwise, 25% chance to place an NPC in the room.
    else 
    {
        TCODRandom* myRand = TCODRandom::getInstance();
        
        // Generate number of monsters for the room.
        int numMonsters = myRand->getInt(0, MAX_ROOM_MONSTERS);
        
        // Add them
        while(numMonsters > 0) 
        {
            int x = myRand->getInt(x1, x2);
            int y = myRand->getInt(y1, y2);
            
            if ( canWalk(x,y) )
                addMonster(x,y);
            
            numMonsters--;  
        }
        
        // Generate number of items for the room
        int numItems = myRand->getInt(0, MAX_ROOM_ITEMS);
        
        // Add them
        while (numItems > 0) 
        {
            int x = myRand->getInt(x1, x2);
            int y = myRand->getInt(y1, y2);

            if ( canWalk(x,y) )
                addItem(x,y);

            numItems--;
        }
    }
}

// ==================================================================================================================================
// ADDMONSTER()
// ----------------------------------------------------------------------------------------------------------------------------------
// Creates a monster and add it to the actors.
// ==================================================================================================================================
void Map::addMonster(int x, int y) 
{
    TCODRandom* myRand = TCODRandom::getInstance();
    
    // We create a mouse 80% of the time
    if ( myRand->getInt(0, 100) < 80 ) 
    {
        Actor* mouse = new Actor(x, y, 'm', "Mouse", TCODColor::desaturatedSky);     
        mouse->destructible = new MonsterDestructible(10, 0, "Dead Mouse");
        mouse->attacker = new Attacker(3);
        mouse->ai = new MonsterAI();
        engine.actors.push(mouse);
    }
    // We create a vacuum 20% of the time
    else
    {
        Actor* vacuum = new Actor(x, y,'V',"Vacuum", TCODColor::darkerBlue);
        vacuum->destructible = new MonsterDestructible(16, 1, "Perished Vacuum");
        vacuum->attacker = new Attacker(4);
        vacuum->ai = new MonsterAI();
        engine.actors.push(vacuum);
    }
}

// ==================================================================================================================================
// ADDITEM()
// ----------------------------------------------------------------------------------------------------------------------------------
// Adds an item to the map.
// ==================================================================================================================================
void Map::addItem(int x, int y) 
{
    TCODRandom* myRand = TCODRandom::getInstance();
    
    // Generate random number
    int prob = myRand->getInt(0, 100);
    
    // Generates health potion 70% of the time
    if (prob < 70) 
    {
        Actor* healthPotion = new Actor(x, y, '!', "health potion", TCODColor::green);
        healthPotion->blocks = false;
        healthPotion->pickable = new Healer(1, true, TCODColor::green, 4);
        engine.actors.push(healthPotion);
    }
    // Generates scroll of lightning bolt 10% of the time
    else if (prob < 70 + 10) 
    {
        Actor* scrollOfLightningBolt = new Actor(x,y,'#',"scroll of lightning bolt", TCODColor::lightYellow);
        scrollOfLightningBolt->blocks = false;
        scrollOfLightningBolt->pickable = new LightningBolt(1, true, TCODColor::lightYellow, 5, 20);
        engine.actors.push(scrollOfLightningBolt);
    }
    // Generates scroll of fireball 10% of the time
    else if(prob < 70 + 10 + 10)
    {
        Actor* scrollOfFireball = new Actor(x, y, '#', "scroll of fireball", TCODColor::lightYellow);
        scrollOfFireball->blocks = false;
        scrollOfFireball->pickable = new FireBall(1, true, TCODColor::lightYellow, 3, 12);
        engine.actors.push(scrollOfFireball);
    }
    // Generates scroll of confusion 10% of the time
    else if(prob < 70 + 10 + 10 + 10)
    {
        Actor* scrollOfConfusion = new Actor(x, y, '#', "scroll of confusion", TCODColor::lightYellow);
        scrollOfConfusion->blocks = false;
        scrollOfConfusion->pickable = new Confuser(1, true, TCODColor::lightYellow, 10, 8);
        engine.actors.push(scrollOfConfusion);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// CANWALK()
// ----------------------------------------------------------------------------------------------------------------------------------
// Returns whether or not player can walk on this tile (covers walls and NPCs)
// ==================================================================================================================================
bool Map::canWalk(int x, int y) const 
{
    // Can't walk; this is a wall.
    if ( isWall(x,y) )
        return false;

    // Cycle through NPCs to see if coordinates are the NPCs. If so, we can't walk.
    for (Actor** iterator = engine.actors.begin(); iterator != engine.actors.end(); iterator++) 
    {
        Actor* actor = *iterator;
        
        if ( (actor->blocks) && (actor->x == x) && (actor->y == y) )
            return false;
    }
    
    return true;
}

// ==================================================================================================================================
// ISFOV()
// ----------------------------------------------------------------------------------------------------------------------------------
// Returns whether or not an the coordinates are in the player's field of view.
// ==================================================================================================================================
bool Map::isInFov(int x, int y) const 
{
    // Make sure it's within the map's boundaries
    if ( x < 0 || x >= width || y < 0 || y >= height ) 
        return false;
    
    // If the tiles are in the player's field of view, set explored to true
    if ( walkMap->isInFov(x, y) ) 
    {
        tiles[x + y*width].explored = true;
        return true;
    }
    
    return false;
}

// ==================================================================================================================================
// ISEXPLORED()
// ----------------------------------------------------------------------------------------------------------------------------------
// Returns whether or not a tile has been explored by the plauer.
// ==================================================================================================================================
bool Map::isExplored(int x, int y) const 
{
    return tiles[x + y*width].explored;
}

// ==================================================================================================================================
// ISWALL()
// ----------------------------------------------------------------------------------------------------------------------------------
// Returns whether or not an actor can walk on the tile, and so if they can't it must be a wall.
// ==================================================================================================================================
bool Map::isWall(int x, int y) const
{
    return walkMap->isWalkable(x,y) == false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



