#ifndef ENGINE_HPP
#define	ENGINE_HPP

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// Engine.hpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Header for a class that links together the different parts of the game and allows main to run it.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 // CLASS: ENGINE
class Engine 
{
    public:  
        // ENUM: GAME STATUS
        enum GameStatus 
        {
            STARTUP, // First frame of the game.
            IDLE, // No new turn. Redraw the same screen.
            NEW_TURN, // Update the monster's position.
            VICTORY, // The player won.
            DEFEAT // The player was killed.
         };
        TCODList<Actor*> actors;
        Actor* player;
        Map* map;
        int fovRadius;
        GameStatus gameStatus;
        int screenWidth;
        int screenHeight;
        TCOD_key_t lastKey;
 
        Engine(int screenWidth, int screenHeight);
        ~Engine();
        void update();
        void render();
        void sendToFront(Actor* actor);      
};
 
// INSTANCE: ENGINE CLASS
extern Engine engine;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif	// ENGINE_HPP

