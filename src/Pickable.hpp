#ifndef PICKABLE_HPP
#define	PICKABLE_HPP

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// Pickable.hpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Header for a class that defines a pickable item for an actor to use/keep in a container.
// Currently has:
//  - Healer potion
//  - Lightniing bolt scroll
//  - Fire ball scroll
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
        void drop(Actor* owner, Actor* wearer);
        virtual bool use(Actor* owner, Actor* wearer);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: HEALER
class Healer : public Pickable 
{
    public:
        float amount; // How much HP is healed

        Healer(int count, bool stackable, const TCODColor& color, float amount);
        bool use(Actor* owner, Actor* wearer);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: LIGHTENINGBOLT
class LightningBolt: public Pickable 
{
    public:
        float range,damage;
        
        LightningBolt(int count, bool stackable, const TCODColor& color, float range, float damage);
        bool use(Actor* owner, Actor* wearer);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: FIREBALL
class FireBall: public Pickable 
{
    public:
        float range, damage;
        
        FireBall(int count, bool stackable, const TCODColor& color, float range, float damage);
        bool use(Actor* owner, Actor* wearer);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: CONFUSER
class Confuser: public Pickable 
{
    public:
        float range;
        int numTurns;
        
        Confuser(int count, bool stackable, const TCODColor& color, float range, int numTurns);
        bool use(Actor* owner, Actor* wearer);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif	// PICKABLE_HPP

