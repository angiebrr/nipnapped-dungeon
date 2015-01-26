#ifndef PICKABLE_HPP
#define	PICKABLE_HPP

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// Pickable.hpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Header for a class that defines a pickable item for an actor to use/keep in a container.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: PICKABLE
class Pickable 
{
    public:
        int count; // Number of pickable items
        bool stackable; // Whether or not it can stack
        TCODColor color; // ASCII Color of item
        
        Pickable(int count, bool stackable, const TCODColor& color);
        bool pick(Actor* owner, Actor* wearer);
        virtual bool use(Actor* owner, Actor* wearer);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: HEALER
class Healer : public Pickable 
{
    public :
        float amount; // How much HP is healed

        Healer(int count, bool stackable, const TCODColor& color, float amount);
        bool use(Actor* owner, Actor* wearer);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif	// PICKABLE_HPP

