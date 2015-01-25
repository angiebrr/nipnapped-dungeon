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
Engine::Engine() : fovRadius(10), gameStatus(STARTUP) 
{
    TCODConsole::initRoot(80, 50, "NipNapped Dungeon", false); // x, y, windowTitle, isFullscreen
    player = new Actor(40, 25, '@', "lucky", TCODColor::white); // Arbitrarily place player (will be centered in first room)
    actors.push(player); // Add player
    map = new Map(80, 45); // Generate the map
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
// Handles key events so player's movements are tracked. Checks walls so the player can't walk through them. Also updates player's 
// field of view.
// ==================================================================================================================================
void Engine::update()
{    
    TCOD_key_t key;
    int dx = 0, dy = 0;
    
    if ( gameStatus == STARTUP ) map->computeFov();
    gameStatus = IDLE;
    
    // Calculate the direction of change of movement.
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
    switch(key.vk) 
    {
        case TCODK_UP: dy = -1; break;
        case TCODK_DOWN: dy = 1; break;
        case TCODK_LEFT: dx = -1; break;
        case TCODK_RIGHT: dx = 1; break;
        default: break;
    }
    
    // If we tried to move, then start new turn, move/attack, and recompute FOV.
    if ( (dx != 0) || (dy != 0) ) 
    {
        gameStatus = NEW_TURN;
        
        if ( player->moveOrAttack(player->x + dx, player->y + dy) ) 
            map->computeFov();
    }
    
    // Loop over actors and update all of them (except the player).
    if (gameStatus == NEW_TURN) 
    {
        for (Actor** iterator = actors.begin(); iterator != actors.end(); iterator++) 
        {
            Actor* actor = *iterator;
            
            if (actor != player) 
                actor->update();
        }
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
    
    // Only draw actors if they are in the player's FOV.
    for (Actor** iterator = actors.begin(); iterator != actors.end(); iterator++) 
    {
        Actor* actor = *iterator;
        
        if ( map->isInFov(actor->x, actor->y) ) 
            actor->render();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

