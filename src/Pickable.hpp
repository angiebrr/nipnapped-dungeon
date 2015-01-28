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
class Pickable : IPersistent
{
    public:
        int count; // Number of pickable items
        bool stackable; // Whether or not it can stack
        TCODColor color; // ASCII Color of item
        PickableType type; // Type of item
        
        Pickable(int count, bool stackable, const TCODColor& color, PickableType type);
        bool pick(Actor* owner, Actor* wearer);
        void drop(Actor* owner, Actor* wearer);
        virtual bool use(Actor* owner, Actor* wearer);
        static Pickable* create (TCODZip& zip);
        void load(TCODZip& zip);
        void save(TCODZip& zip);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: HEALER
class Healer : public Pickable 
{
    public:
        float amount; // How much HP is healed

        Healer(int count, bool stackable, const TCODColor& color, PickableType type, float amount);
        bool use(Actor* owner, Actor* wearer);
        void load(TCODZip& zip);
        void save(TCODZip& zip);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: LIGHTENINGBOLT
class LightningBolt: public Pickable 
{
    public:
        float range, damage;
        
        LightningBolt(int count, bool stackable, const TCODColor& color, PickableType type, float range, float damage);
        bool use(Actor* owner, Actor* wearer);
        void load(TCODZip& zip);
        void save(TCODZip& zip);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: FIREBALL
class FireBall : public Pickable
{
    public:
        float range, damage;
        
        FireBall(int count, bool stackable, const TCODColor& color, PickableType type, float range, float damage);
        bool use(Actor* owner, Actor* wearer);
        void load(TCODZip& zip);
        void save(TCODZip& zip);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: CONFUSER
class Confuser: public Pickable 
{
    public:
        float range;
        int numTurns;
        
        Confuser(int count, bool stackable, const TCODColor& color, PickableType type, float range, int numTurns);
        bool use(Actor* owner, Actor* wearer);
        void load(TCODZip& zip);
        void save(TCODZip& zip);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif	// PICKABLE_HPP

