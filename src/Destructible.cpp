#include "main.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// Destructible.hpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Class that helps define objects that can be destroyed.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// PARENT: CONSTRUCTOR
Destructible::Destructible(float maxHp, float defense, const char* corpseName, int xp, bool isBoss) : 
    maxHp(maxHp), hp(maxHp), defense(defense), corpseName(corpseName), xp(xp), isBoss(isBoss) {}
        
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// PARENT: TAKEDAMAGE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Handles damage given to the targeted actor.
// ==================================================================================================================================
float Destructible::takeDamage(Actor* owner, float damage)
{
    damage -= defense;
    
    // Damage done and/or actor is killed.
    if (damage > 0) 
    {
        hp -= damage;
        
        if (hp <= 0)
            die(owner);      
    } 
    // No damage done if defense is high enough
    else
    {
        damage = 0;
    }
    
    return damage;
}

// ==================================================================================================================================
// PARENT: HEAL()
// ----------------------------------------------------------------------------------------------------------------------------------
// Handles healing 
// ==================================================================================================================================
float Destructible::heal(float amount)
{
    hp += amount;
    
    // Prevents overhealing
    if (hp > maxHp) 
    {
        amount -= hp - maxHp;
        hp = maxHp;
    }
    
    return amount;
}

// ==================================================================================================================================
// PARENT: DIE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Transforms targeted actor into a corpse.
// ==================================================================================================================================
void Destructible::die(Actor* owner) 
{
    // Update actor to be a corpse.
    owner->code = '%';
    owner->color = TCODColor::darkRed;   
    owner->name = corpseName;
    owner->blocks = false;
    
    // Make sure corpses are drawn before living actors.
    engine.sendToFront(owner);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MONSTER: CONSTRUCTOR
MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char* corpseName, int xp, bool isBoss) :
    Destructible(maxHp, defense, corpseName, xp, isBoss) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// MONSTER: DIE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Transforms monster into a corpse. Can't block, can't attack, and can't move.
// ==================================================================================================================================
void MonsterDestructible::die(Actor* owner) 
{
    engine.gui->message(TCODColor::lightGrey, "%s is dead. You gain %d xp", owner->name, xp);
    engine.player->destructible->xp += xp;
    
    // Notify user of success if they got the boss
    if(isBoss)
        engine.gui->message(TCODColor::green, 
                "You have defeated %s! This catacomb is \n abandoned, but your quest is far from over...", owner->name);
    
    Destructible::die(owner);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
// PLAYER: CONSTRUCTOR
PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char* corpseName, int xp, bool isBoss) :
    Destructible(maxHp, defense, corpseName, xp, isBoss) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// PLAYER: DIE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Transforms player into a corpse. Can't block, can't attack, and can't move. The game ends and the player loses.
// ==================================================================================================================================
void PlayerDestructible::die(Actor* owner) 
{
    engine.gui->message(TCODColor::white, "You died!\n");
    Destructible::die(owner);
    engine.gameStatus = DEFEAT; 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

