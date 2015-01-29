#ifndef ACTOR_HPP
#define ACTOR_HPP

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// Actor.hpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Header for a class that defines an actor's ASCII character along with their background and foreground colors.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Actor : IPersistent
{
    public:
        int x, y; // Position on map
        int code; // ASCII Code
        TCODColor color; // ASCII Color
        const char* name; // The actor's name
        bool blocks; // Can we walk on this actor?
        bool fovOnly; // Only display when in FOV
        Attacker* attacker; // Something that deals damages
        Destructible* destructible; // Something that can be damaged
        AI* ai; // Something self-updating
        Pickable* pickable; // Something that can be picked and used
        Container* container; // Something that can contain actors
 
        Actor(int x, int y, int code, const char* name, const TCODColor& color);
        ~Actor();
        void render() const;
        void update();
        float getDistance(int cx, int cy) const;
        void load(TCODZip& zip);
        void save(TCODZip& zip);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // ACTOR_HPP