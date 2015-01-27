#include "main.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// Pickable.cpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Class that defines a pickable item for an actor to use/keep in a container.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR
Pickable::Pickable(int count, bool stackable, const TCODColor& color) : count(count), stackable(stackable), color(color) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// PARENT: PICK()
// ----------------------------------------------------------------------------------------------------------------------------------
// Picks up an actor and tries to add it to the inventory.
// ==================================================================================================================================
bool Pickable::pick(Actor* owner, Actor* wearer)
{
    if( wearer->container && wearer->container->add(owner) )
    {
       engine.actors.remove(owner);
       return true;
    }
    
    return false;
}

// ==================================================================================================================================
// PARENT: USE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Destroys the actor/item.
// ==================================================================================================================================
bool Pickable::use(Actor* owner, Actor* wearer)
{
    if (wearer->container) 
    {
        // If we have more than one of the item, just decrement the count.
        if(owner->pickable->count > 1)
        {
            owner->pickable->count--;
        }
        // If it's the last one, then destroy it and resets the inventory (to clear the color from the item).
        else
        {
            wearer->container->remove(owner);
            delete owner;
            engine.gui->clearInventoryConsole();
        }
        
        return true;
    }
    
    return false;
}

// ==================================================================================================================================
// PARENT: DROP()
// ----------------------------------------------------------------------------------------------------------------------------------
// Drops the item from the container (if the wearer has one)
// ==================================================================================================================================
void Pickable::drop(Actor* owner, Actor* wearer)
{
    // If the wearer has a container, drop the item to the map.
   if (wearer->container) 
   {
        wearer->container->remove(owner);
        engine.actors.push(owner);
        owner->x = wearer->x;
        owner->y = wearer->y;
        engine.gui->message(TCODColor::lightGrey,"%s drops a %s.", wearer->name, owner->name);
    } 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// HEALER: CONSTRUCTOR
Healer::Healer(int count, bool stackable, const TCODColor& color, float amount) : Pickable(count, stackable, color), amount(amount) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// HEALER: USE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Heals the wearer. If the wearer already had full health, then it isn't used.
// ==================================================================================================================================
bool Healer::use(Actor* owner, Actor* wearer)
{
    if ( wearer->destructible ) 
    {
        float amountHealed = wearer->destructible->heal(amount);
        
        // If the potion could heal the wearer, then destroy it.
        if ( amountHealed > 0 )
            return Pickable::use(owner, wearer);
    }
    
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// LIGHTNINGBOLT: CONSTRUCTOR
LightningBolt::LightningBolt(int count, bool stackable, const TCODColor& color, float range, float damage) : 
    Pickable(count, stackable, color), range(range), damage(damage) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// LIGHTNINGBOLT: USE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Does damage to the closest monster.
// ==================================================================================================================================
bool LightningBolt::use(Actor* owner, Actor* wearer)
{
    Actor* closestMonster = engine.getClosestMonster(wearer->x, wearer->y, range);
    if (! closestMonster ) 
    {
        engine.gui->message(TCODColor::lightGrey,"No enemy is close enough to strike.");
        return false;
    }
    // Target and hit the closest monster.
    engine.gui->message(TCODColor::lightBlue, "A lighting bolt strikes the %s with a loud thunder!\n" "The damage is %g hit points.",
        closestMonster->name, damage);
    
    closestMonster->destructible->takeDamage(closestMonster, damage);
    
    // Destroy/consume
    return Pickable::use(owner, wearer);  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FIREBALL: CONSTRUCTOR
FireBall::FireBall(int count, bool stackable, const TCODColor& color, float range, float damage) : 
    Pickable(count, stackable, color), range(range), damage(damage) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// FIREBALL: USE()
// ----------------------------------------------------------------------------------------------------------------------------------
//  Make a fire ball that burns everyone in range.
// ==================================================================================================================================
bool FireBall::use(Actor* owner, Actor* wearer)
{
    int x, y;
    
    engine.gui->message(TCODColor::cyan, "Left-click a target tile for the fireball,\nor right-click to cancel.");
    
    // Go into another loop to choose a tile.
    if ( !engine.pickATile(&x, &y) )
        return false;
    
    // Burn everything in <range> (including player!)
    engine.gui->message(TCODColor::orange,"The fireball explodes, burning everything within %g tiles!",range);
    for (Actor** iterator=engine.actors.begin(); iterator != engine.actors.end(); iterator++) 
    {
        Actor* actor = *iterator;
        
        // Burn the actor
        if ( actor->destructible && !actor->destructible->isDead() && actor->getDistance(x,y) <= range ) 
        {
            engine.gui->message(TCODColor::orange,"The %s gets burned for %g hit points.", actor->name, damage);
            actor->destructible->takeDamage(actor, damage);
        }
    }
    
    // Destroy
    return Pickable::use(owner, wearer);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONFUSER:CONSTRUCTOR
Confuser::Confuser(int count, bool stackable, const TCODColor& color, float range, int numTurns) :
    Pickable(count, stackable, color), range(range), numTurns(numTurns) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// CONFUSER: USE()
// ----------------------------------------------------------------------------------------------------------------------------------
//  Makes targeted agent confused.
// ==================================================================================================================================
bool Confuser::use(Actor* owner, Actor* wearer)
{
   int x, y;
    
    engine.gui->message(TCODColor::cyan, "Left-click a target tile for the fireball,\nor right-click to cancel.");
    
    // Go into another loop to choose a tile.
    if ( !engine.pickATile(&x,&y) )
        return false;
    
    // Retrieve actor, or fail if an actor wasn't selected
    Actor* actor = engine.getActor(x, y);
    if(!actor)
        return false;
    
    // Update retrieved actor's AI.
    AI* confusedAI = new ConfusedMonsterAI(numTurns, actor->ai);
    actor->ai = confusedAI;
    
    engine.gui->message(TCODColor::lightGreen,"The eyes of the %s look vacant,\nas he starts to stumble around!", actor->name);
    
    // Destroy
    return Pickable::use(owner, wearer); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////