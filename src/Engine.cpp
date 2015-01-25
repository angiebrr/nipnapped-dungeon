#include "main.hpp"

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
Engine::Engine(int screenWidth, int screenHeight) : 
    screenWidth(screenWidth), screenHeight(screenHeight), fovRadius(10), gameStatus(STARTUP) 
{
    TCODConsole::initRoot(screenWidth, screenHeight, "NipNapped Dungeon", false); // x, y, windowTitle, isFullscreen
    
    // Make and add player
    player = new Actor(40, 25, '@', "Lucky", TCODColor::white);
    player->destructible = new PlayerDestructible(30, 2, "Lucky's corpse");
    player->attacker = new Attacker(5);
    player->myAI = new PlayerAI();
    actors.push(player);
    
    // Generate the map
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
// Updates all actors' statuses and 
// ==================================================================================================================================
void Engine::update()
{    
    // Computer field of view for the first frame of the game.
    if (gameStatus == STARTUP) 
        map->computeFov();
    
    // Change to idle and lister for key press.
    gameStatus = IDLE;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
    
    // Update the player
    player->update();
    
    // Loop through all the actors and update (except the player)
    if ( gameStatus == NEW_TURN ) 
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
    
    // Only draw actors if they are in the player's FOV and isn't the player.
    for (Actor** iterator = actors.begin(); iterator != actors.end(); iterator++) 
    {
        Actor* actor = *iterator;
        
        if ( (actor != player) && map->isInFov(actor->x, actor->y) ) 
            actor->render();
    }
    
    // Draw player.  
    player->render();
    
    // Show the player's statistics.
    TCODConsole::root->print(1,screenHeight-2, "HP : %d/%d", (int)player->destructible->hp, (int)player->destructible->maxHp);
}

// ==================================================================================================================================
// SENDTOFRONT()
// ----------------------------------------------------------------------------------------------------------------------------------
// Send the actor to the front of the list.
// ==================================================================================================================================
void Engine::sendToFront(Actor* actor)
{
    actors.remove(actor);
    actors.insertBefore(actor, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

