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
        int screenWidth;
        int screenHeight;
        GUI* gui;
        TCODList<Actor*> actors;
        Actor* player;
        Map* map;
        int fovRadius;
        GameStatus gameStatus;
        TCOD_key_t lastKey;
        TCOD_mouse_t mouse;
 
        Engine(int screenWidth, int screenHeight);
        ~Engine();
        void update();
        void render();
        void sendToFront(Actor* actor);
        Actor* getClosestMonster(int x, int y, float range) const;
        bool pickATile(int* x, int* y, float maxRange = 0.0f);
        Actor* getActor(int x, int y) const;
};
 
// INSTANCE: ENGINE CLASS
extern Engine engine;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif	// ENGINE_HPP

