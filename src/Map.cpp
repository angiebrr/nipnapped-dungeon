#include "main.hpp"

using namespace LevelConstants;
using namespace ActorConstants;
using namespace GUIConstants;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// Map.hpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Class that helps define the map: its coloring, items, and monsters.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR
Map::Map(int width, int height) : width(width), height(height), roomMaxSize(ROOM_MAX_SIZE), roomMinSize(ROOM_MIN_SIZE),
    minRoomMonsters(0), monstersAdded(0), bossMap(false)
{
    // Make random map seed
    seed = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
    
    // Get maximum number of monsters (scales with level)
    maxRoomMonsters = MAX_ROOM_MONSTERS + (engine.level / LAST_LEVEL);
}
    
// DESTRUCTOR
Map::~Map() 
{
    delete [] tiles;
    delete walkMap;
}

// INITIALIZATION
void Map::init(bool withActors)
{
    // Make sure only one monster is generated
    if(bossMap)
    {
        this->maxRoomMonsters = 1;
        this->minRoomMonsters = 0;
    }
    
    myRand = new TCODRandom(seed, TCOD_RNG_CMWC);
    
    tiles = new Tile[width * height];
    walkMap = new TCODMap(width, height);
    
    // Create a binary space partition tree to partition the map.
    TCODBsp bsp(0, 0, width, height);
    
    // Splits the area up recursively into rectangle
    bsp.splitRecursive(myRand, RECURSION_DEPTH, roomMaxSize, roomMaxSize, MAX_H_RATIO, MAX_V_RATIO);
    
    // Traverse tree with helper listener class, BspListener.
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, (void*)withActors);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// RENDER()
// ----------------------------------------------------------------------------------------------------------------------------------
// Builds the map based on whether a tile is a wall or if it's the ground and colors the map accordingly.
// ==================================================================================================================================
void Map::render() const
{
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
            else if( isExplored(x, y) )
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
void Map::createRoom(bool first, int x1, int y1, int x2, int y2, bool withActors)
{
    dig (x1, y1, x2, y2);
    
    // Only generate the map
    if(!withActors)
        return;
    
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
        int numMonsters = myRand->getInt(minRoomMonsters, maxRoomMonsters);
        
        // Add them
        while(numMonsters > 0) 
        {
            int x = myRand->getInt(x1, x2);
            int y = myRand->getInt(y1, y2);
            
            
            if ( canWalk(x,y) )
            {
                // We only want to add one monster if it's a boss map
                if( (bossMap && monstersAdded == 0) || !bossMap)
                    addMonster(x, y);
            }
            
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
    
    // Set stairs position to be in the center of every room until the end
    engine.stairs->x = (x1 + x2)/2;
    engine.stairs->y = (y1 + y2)/2;
}

// ==================================================================================================================================
// ADDMONSTER()
// ----------------------------------------------------------------------------------------------------------------------------------
// Creates a monster and add it to the actors.
// ==================================================================================================================================
void Map::addMonster(int x, int y) 
{
    // Add another monster
    monstersAdded++;
    
    // Generator random slot
    TCODRandom* myRand = TCODRandom::getInstance();
    int prob = myRand->getInt(0, NUM_MONSTER_SLOTS - 1);
    
    // Get it from monster slot array (0-index)
    MonsterType monster = (MonsterType)LEVEL_MONSTERS[engine.level - 1][prob];
    
    // Create monster from what was retrieved
    switch(monster)
    {
        case MOUSE:
        {
            Actor* mouse = new Actor(x, y, MOUSE_CHAR, MOUSE_NAME, MOUSE_COLOR);     
            mouse->destructible = new MonsterDestructible(MOUSE_MAX_HEALTH, MOUSE_DEFENSE, MOUSE_CORPSE_NAME, MOUSE_BASE_XP_DROP, !IS_BOSS);
            mouse->attacker = new Attacker(MOUSE_ATTACK);
            mouse->ai = new MonsterAI();
            engine.actors.push(mouse); 
        }
        break;
        case PUPPY:
        {
            Actor* puppy = new Actor(x, y, PUPPY_CHAR, PUPPY_NAME, PUPPY_COLOR);     
            puppy->destructible = 
                    new MonsterDestructible(PUPPY_MAX_HEALTH, PUPPY_DEFENSE, PUPPY_CORPSE_NAME, PUPPY_BASE_XP_DROP, !IS_BOSS);
            puppy->attacker = new Attacker(PUPPY_ATTACK);
            puppy->ai = new MonsterAI();
            engine.actors.push(puppy); 
        }
        break;
        case DOG:
        {
            Actor* dog = new Actor(x, y, DOG_CHAR, DOG_NAME, DOG_COLOR);     
            dog->destructible = new MonsterDestructible(DOG_MAX_HEALTH, DOG_DEFENSE, DOG_CORPSE_NAME, DOG_BASE_XP_DROP, !IS_BOSS);
            dog->attacker = new Attacker(DOG_ATTACK);
            dog->ai = new MonsterAI();
            engine.actors.push(dog); 
        }
        break;
        case VACUUM:
        {
            Actor* vacuum = new Actor(x, y, VACUUM_CHAR, VACUUM_NAME, VACUUM_COLOR);
            vacuum->destructible = 
                    new MonsterDestructible(VACUUM_MAX_HEALTH, VACUUM_DEFENSE, VACUUM_CORPSE_NAME, VACUUM_BASE_XP_DROP, !IS_BOSS);
            vacuum->attacker = new Attacker(VACUUM_ATTACK);
            vacuum->ai = new MonsterAI();
            engine.actors.push(vacuum);
        }
        break;
        case KITTY_THE_GRAY:
        {
            Actor* kittyTheGray = new Actor(x, y, KITTY_THE_GRAY_CHAR, KITTY_THE_GRAY_NAME, KITTY_THE_GRAY_COLOR);     
            kittyTheGray->destructible = 
                    new MonsterDestructible(KITTY_THE_GRAY_MAX_HEALTH, KITTY_THE_GRAY_DEFENSE, KITTY_THE_GRAY_CORPSE_NAME, 
                    KITTY_THE_GRAY_BASE_XP_DROP, IS_BOSS);
            kittyTheGray->attacker = new Attacker(KITTY_THE_GRAY_ATTACK);
            kittyTheGray->ai = new MonsterAI();
            engine.actors.push(kittyTheGray); 
        }
        break;
        default: break;
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
        Actor* healthPotion = new Actor(x, y, POTION_CHAR, HEALER_NAME, POTION_COLOR);
        healthPotion->blocks = false;
        healthPotion->pickable = new Healer(ONE_ITEM_COUNT, IS_STACKABLE, HEALER_COLOR, HEALER, HEALER_AMOUNT);
        engine.actors.push(healthPotion);
    }
    // Generates scroll of lightning bolt 10% of the time
    else if (prob < 70 + 10) 
    {
        Actor* scrollOfLightningBolt = new Actor(x, y, SCROLL_CHAR, LIGHTNING_NAME, SCROLL_COLOR);
        scrollOfLightningBolt->blocks = false;
        scrollOfLightningBolt->pickable = 
                new LightningBolt(ONE_ITEM_COUNT, IS_STACKABLE, LIGHTNING_COLOR, LIGHTNING_BOLT, LIGHTNING_RANGE, LIGHTNING_DAMAGE);
        engine.actors.push(scrollOfLightningBolt);
    }
    // Generates scroll of fireball 10% of the time
    else if(prob < 70 + 10 + 10)
    {
        Actor* scrollOfFireball = new Actor(x, y, SCROLL_CHAR, FIREBALL_NAME, SCROLL_COLOR);
        scrollOfFireball->blocks = false;
        scrollOfFireball->pickable = 
                new FireBall(ONE_ITEM_COUNT, IS_STACKABLE, FIREBALL_COLOR, FIREBALL, FIREBALL_RANGE, FIREBALL_DAMAGE);
        engine.actors.push(scrollOfFireball);
    }
    // Generates scroll of confusion 10% of the time
    else if(prob < 70 + 10 + 10 + 10)
    {
        Actor* scrollOfConfusion = new Actor(x, y, SCROLL_CHAR, CONFUSER_NAME, SCROLL_COLOR);
        scrollOfConfusion->blocks = false;
        scrollOfConfusion->pickable = 
                new Confuser(ONE_ITEM_COUNT, IS_STACKABLE, CONFUSER_COLOR, CONFUSER, CONFUSER_RANGE, CONFUSER_NUM_TURNS);
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



