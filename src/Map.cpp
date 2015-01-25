#include "libtcod.hpp"
#include "Map.hpp"
#include "Actor.hpp"
#include "Engine.hpp"
#include "BspListener.hpp"

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
    
    for (int x = 0; x < width; x++) 
    {
        for (int y = 0; y < height; y++) 
        {
            if(isWall(x, y))
                TCODConsole::root->setCharBackground( x,y, darkWall);
            else
                TCODConsole::root->setCharBackground( x,y, darkGround);
        }
    }
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
            tiles[tilex + tiley*width].canWalk = true;
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
        
        if ( myRand->getInt(0,3) == 0 ) 
            engine.actors.push(new Actor((x1 + x2)/2, (y1 + y2)/2, '@', TCODColor::yellow));
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// ISWALL()
// ----------------------------------------------------------------------------------------------------------------------------------
// Returns whether or not an actor can walk on the tile, and so if they can't it must be a wall.
// ==================================================================================================================================
bool Map::isWall(int x, int y) const
{
    return tiles[x + y*width].canWalk == false;
}

// ==================================================================================================================================
// SETWALL()
// ----------------------------------------------------------------------------------------------------------------------------------
// Set the tile's "can walk" field to false so it behaves like a wall.
// ==================================================================================================================================
void Map::setWall(int x, int y)
{
    tiles[x + y*width].canWalk = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



