#include "main.hpp"

using namespace GUIConstants;
using namespace LevelConstants;
using namespace ActorConstants;

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
    screenWidth(screenWidth), screenHeight(screenHeight), fovRadius(FOV_RADIUS), gameStatus(STARTUP) 
{
    TCODConsole::initRoot(screenWidth, screenHeight, WINDOW_TITLE, IS_FULLSCREEN);
    
    // Generate the GUI
    gui = new GUI();
}

// DESTRUCTOR
Engine::~Engine() 
{   
    term();
    delete gui;
}

// TERMINATION
void Engine::term()
{
    actors.clearAndDelete();
    if (map) delete map;
    gui->clear();
}

// INITIALIZATION
void Engine::init()
{
    // Make and add player
    player = new Actor(PLAYER_DEFAULT_X, PLAYER_DEFAULT_Y, PLAYER_CHAR, PLAYER_NAME, PLAYER_COLOR);
    player->destructible = new PlayerDestructible(PLAYER_MAX_HEALTH, PLAYER_DEFENSE, PLAYER_CORPSE_NAME);
    player->attacker = new Attacker(PLAYER_ATTACK);
    player->ai = new PlayerAI();
    player->container = new Container(INVENTORY_SIZE);
    actors.push(player);
    
    // Generate the map
    map = new Map(SCREEN_WIDTH, SCREEN_HEIGHT - 7);
    map->init(true);
   
    // Add beginning message and start game.
    gui->message(TCODColor::red, "May the odds be ever in your favor, Lucky.\n Prepare to perish in the Catacombs of Kitty The Grey.");   
    gameStatus = STARTUP;
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
    
    // Pause to menu
    if ( lastKey.vk == TCODK_ESCAPE ) 
    {
        save();
        load();
    }
    
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

// ==================================================================================================================================
// GETCLOSESTMONSTER()
// ----------------------------------------------------------------------------------------------------------------------------------
// Get the closest monster in range. If range is 0, then it's considered infinite.
// ==================================================================================================================================
Actor* Engine::getClosestMonster(int x, int y, float range) const
{
    Actor* closest = NULL;
    float bestDistance = 1E6f;
    
    // Loop over all actors
    for (Actor** iterator = actors.begin(); iterator != actors.end(); iterator++) 
    {
        Actor* actor = *iterator;
        
        // If it's alive, see if it's closer to the target and in range.
        if ( actor != player && actor->destructible && !actor->destructible->isDead() )
        {
            float distance = actor->getDistance(x,y);
            if ( distance < bestDistance && ( distance <= range || range == 0.0f ) ) 
            {
                bestDistance = distance;
                closest = actor;
            }
        }
    }
    
    return closest;
}

// ==================================================================================================================================
// PICKATILE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Creates another main game loop so user can pick a tile.
// ==================================================================================================================================
bool Engine::pickATile(int* x, int* y, float maxRange) 
{
    while ( !TCODConsole::isWindowClosed() ) 
    {
        // Draw map and actors
        render();
        
        // Highlight where user can pick.
        for (int cx = 0; cx < map->width; cx++) 
        {
            for (int cy = 0; cy < map->height; cy++) 
            {
                if ( map->isInFov(cx, cy) && ( maxRange == 0 || player->getDistance(cx, cy) <= maxRange) ) 
                {
                    TCODColor color = TCODConsole::root->getCharBackground(cx,cy);
                     color = color * 1.2f;
                    TCODConsole::root->setCharBackground(cx, cy, color);
                }
            }
        }
        
        // Wait for key or mouse
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE, &lastKey, &mouse);
        
        // Fill the title under mouse cursor with white
        if ( map->isInFov(mouse.cx,mouse.cy) && ( maxRange == 0 || player->getDistance(mouse.cx, mouse.cy) <= maxRange) )
        {
            TCODConsole::root->setCharBackground(mouse.cx,mouse.cy,TCODColor::white);
        
            // If left button pressed, return and update coordinates.
            if (mouse.lbutton_pressed) 
            {
                *x = mouse.cx;
                *y = mouse.cy;

                return true;
            }
            
            // Cancel if a key was pressed or the right button is pressed.
            if (mouse.rbutton_pressed || lastKey.vk != TCODK_NONE) 
                return false;       
        }
        
        TCODConsole::flush();
    }
    
    // Exit if main window closed
    return false;
}

// ==================================================================================================================================
// GETACTOR()
// ----------------------------------------------------------------------------------------------------------------------------------
// Retrieves an actor if found at the specified coordinates.
// ==================================================================================================================================
Actor* Engine::getActor(int x, int y) const
{
    // Loop over all actors
    for (Actor** iterator = actors.begin(); iterator != actors.end(); iterator++) 
    {
        Actor* actor = *iterator;
        
        // Found actor
        if(actor->x == x && actor->y == y)
            return actor;
    }
    
    // Didn't find actor
    return NULL;
}
        

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

