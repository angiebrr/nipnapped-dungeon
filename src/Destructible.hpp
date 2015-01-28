#ifndef DESTRUCTIBLE_HPP
#define	DESTRUCTIBLE_HPP

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// Destructible.hpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Header for a class that helps define objects that can be destroyed.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: DESTRUCTIBLE
class Destructible : IPersistent
{
    public:
        float maxHp; // Maximum health points
        float hp; // Current health points
        float defense; // Hit points deflected
        const char* corpseName; // The actor's name once dead/destroyed
        
        Destructible(float maxHp, float defense, const char* corpseName);
        float takeDamage(Actor* owner, float damage);
        float heal(float amount);
        inline bool isDead(){ return hp <= 0; }
        virtual void die(Actor* owner);
        void load(TCODZip& zip);
        void save(TCODZip& zip);
        static Destructible* create(TCODZip& zip);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: MONSTERDESTRUCTIBLE
class MonsterDestructible: public Destructible
{
    public:
        MonsterDestructible(float maxHp, float defense, const char* corpseName);
        void die(Actor* owner);
        void save(TCODZip& zip);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: PLAYERDESTRUCTIBLE
class PlayerDestructible: public Destructible
{
    public:
        PlayerDestructible(float maxHp, float defense, const char* corpseName);
        void die(Actor* owner);
        void save(TCODZip& zip);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif	// DESTRUCTIBLE_HPP