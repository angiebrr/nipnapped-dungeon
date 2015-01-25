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
    player->destructible = new PlayerDestructible(30, 2, "Lucky's Corpse");
    player->attacker = new Attacker(5);
    player->ai = new PlayerAI();
    actors.push(player);
    
    // Generate the map
    map = new Map(80, 43);
    
    // Generate the GUI
    gui = new GUI();
    
    // Add beginning message
    gui->message(TCODColor::red, "May the odds be ever in your favor, Lucky.\n Prepare to perish in the Catacombs of Kitty The Grey.");
}

// DESTRUCTOR
Engine::~Engine() 
{
    actors.clearAndDelete();
    delete map;
    delete gui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// UPDATE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Updates all actors' statuses and states.
// ==================================================================================================================================
void Engine::update()
{    
    // Computer field of view for the first frame of the game.
    if (gameStatus == STARTUP) 
        map->computeFov();
    
    // Change to idle and lister for key press and mouse movement.
    gameStatus = IDLE;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE, &lastKey, &mouse);
    
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
    
    // Draw player and the player's statistics  
    player->render();
    gui->render();
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

