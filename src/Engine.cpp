#include "libtcod.hpp"
#include "Actor.hpp"
#include "Map.hpp"
#include "Engine.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// Engine.cpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Class that links together the different parts of the game and allows main to run it.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR
Engine::Engine() 
{
    TCODConsole::initRoot(80, 50, "NipNapped Dungeon", false);
    player = new Actor(40, 25, '@', TCODColor::white);
    actors.push(player);
    actors.push(new Actor(60, 13, '@', TCODColor::yellow));
    map = new Map(80, 45);
}

// DESTRUCTOR
Engine::~Engine() 
{
    actors.clearAndDelete();
    delete map;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// UPDATE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Handles key events so player's movements are tracked. Checks walls so the player can't walk through them.
// ==================================================================================================================================
void Engine::update()
{
    TCOD_key_t key;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
    
    switch(key.vk) 
    {
        case TCODK_UP: 
            if ( !map->isWall(player->x, player->y-1) )
                player->y--;   
        break;
        case TCODK_DOWN: 
            if ( !map->isWall(player->x, player->y+1) )
                player->y++;
        break;
        case TCODK_LEFT: 
            if ( !map->isWall(player->x-1, player->y) )
                player->x--;
        break;
        case TCODK_RIGHT: 
            if ( !map->isWall(player->x+1, player->y) )
                player->x++;
        break;
        default:break;
    }
}

// ==================================================================================================================================
// RENDER()
// ----------------------------------------------------------------------------------------------------------------------------------
// Clears the console and draws the map and the actors.
// ==================================================================================================================================
void Engine::render()
{
    // Clear console and draw map.
    TCODConsole::root->clear();
    map->render();
    
    // Draw actors.
    for (Actor **iterator = actors.begin(); iterator != actors.end(); iterator++) 
    {
        (*iterator)->render();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

