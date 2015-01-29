#include "main.hpp"

using namespace ActorConstants;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// AI.cpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Class that helps define the behavior of an object.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// MONSTER: UPDATE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Listens for player movements to move toward or attack.
// ==================================================================================================================================
void MonsterAI::update(Actor* owner) 
{
    // Don't do anything if monster is dead.
    if ( owner->destructible && owner->destructible->isDead() )
        return;
    
    // If we can see the player(or the player sees us), move toward him.
    if ( engine.map->isInFov(owner->x, owner->y) )
        moveCount = TRACKING_TURNS;
    // If he went out of our sight, keep track of him for a few turns.
    else 
        moveCount--;
    
    // If he isn't out of sight, move or attack toward him.
   if (moveCount > 0)
       moveOrAttack(owner, engine.player->x, engine.player->y); 
}


// ==================================================================================================================================
// MONSTER: MOVEORATTACK()
// ----------------------------------------------------------------------------------------------------------------------------------
// Moves toward player if the monster saw or recently saw player. Attack if player is close enough.
// ==================================================================================================================================
void MonsterAI::moveOrAttack(Actor* owner, int targetx, int targety) 
{
    // Calculate Euclidean distance between monster and target coordinates
    int dx = targetx - owner->x;
    int dy = targety - owner->y;
    float distance = sqrtf( dx*dx + dy*dy );

    // Add side steps to enable wall sliding
    int stepdx = (dx > 0 ? 1 : -1);
    int stepdy = (dy > 0 ? 1 : -1);    
    
    // If we're out of melee range, go toward the target
    if (distance >= 2) 
    {
        // Normalize the distance vector between the target and the monster
        dx = (int)( round(dx/distance) );
        dy = (int)( round(dy/distance) );
        
        // If the tiles are walkable, then walk on them. 
        if ( engine.map->canWalk(owner->x + dx,owner->y + dy) ) 
        {
            owner->x += dx;
            owner->y += dy;
        }
        // If we can move in the x-direction this way, then go this way
        else if ( engine.map->canWalk(owner->x + stepdx, owner->y) )
        {
            owner->x += stepdx;
        }
        // If we can move in the y-direction this way, then go this way
        else if ( engine.map->canWalk(owner->x, owner->y + stepdy) )
        {
            owner->y += stepdy;
        }
    }
    // If the monster is an attacker and is in melee range, attack!
    else if (owner->attacker) 
    {
        owner->attacker->attack(owner, engine.player);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// PLAYER: CONSTRUCTOR
PlayerAI::PlayerAI() : xpLevel(1) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// PLAYER: UPDATE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Listens for player movements and updates actor's status accordingly.
// ==================================================================================================================================
void PlayerAI::update(Actor* owner) 
{
    // Level up player if they have gathered enough XP
    int levelUpXP = getNextLevelXP();
    if ( owner->destructible->xp >= levelUpXP ) 
    {
        xpLevel++;
        owner->destructible->xp -= levelUpXP;
        engine.gui->message(TCODColor::yellow, "Your battle skills grow stronger! You reached level %d", xpLevel);
        
        // Load menu to level up
        engine.gui->menu.clear();
        engine.gui->menu.addItem(CONSTITUTION,"Constitution (+20HP)");
        engine.gui->menu.addItem(STRENGTH,"Strength (+1 attack)");
        engine.gui->menu.addItem(AGILITY,"Agility (+1 defense)");
        MenuItemCode menuItem = engine.gui->menu.pick(PAUSE);
        
        // Improve chosen player attribute
        switch (menuItem) 
        {
            case CONSTITUTION:
                owner->destructible->maxHp += 20;
                owner->destructible->hp += 20;
                break;
            case STRENGTH:
                owner->attacker->power += 1;
                break;
            case AGILITY:
                owner->destructible->defense += 1;
                break;
            default:break;
        }
    }
    
    // Don't do anything if player is dead.
    if ( owner->destructible && owner->destructible->isDead() ) 
        return;
    
    // Calculate the direction of the change of movement
    int dx = 0,dy = 0;
    switch(engine.lastKey.vk) 
    {
        case TCODK_UP : dy = -1; break;
        case TCODK_DOWN : dy = 1; break;
        case TCODK_LEFT : dx = -1; break;
        case TCODK_RIGHT : dx = 1; break;
        case TCODK_CHAR : handleActionKey(owner, engine.lastKey.c); break;
        default:break;
    }
    
    // If we tried to move, then start new turn, move/attack, and recompute FOV.
    if (dx != 0 || dy != 0) 
    {
        engine.gameStatus = NEW_TURN;
        
        if ( moveOrAttack(owner, owner->x + dx, owner->y + dy) )
            engine.map->computeFov();
    }
}

// ==================================================================================================================================
// PLAYER: MOVEORATTACK()
// ----------------------------------------------------------------------------------------------------------------------------------
// Whether or not actor can move. It can and does move if there isn't a wall or the actor isn't moving over them (attacking).
// ==================================================================================================================================
bool PlayerAI::moveOrAttack(Actor* owner, int targetx,int targety) 
{
    // Can't move or attack if it's a wall
    if ( engine.map->isWall(targetx, targety) ) 
        return false;

    // Loop through actors to see if there's an actor to attack or display.
    for (Actor** iterator = engine.actors.begin(); iterator != engine.actors.end(); iterator++) 
    {
        Actor* actor = *iterator;
        
        // Is the actor a corpse or an item?
        bool corpseOrItem = ( actor->destructible && actor->destructible->isDead() ) || actor->pickable;
         
        if ( (actor->x == targetx) && (actor->y == targety) ) 
        {      
            // If it isn't dead, attack. Can't move.
            if( actor->destructible && !actor->destructible->isDead() )
            {
               owner->attacker->attack(owner, actor);
                return false; 
            }
            
            // If it's a corpse or item, display its information.
            if(corpseOrItem)
                engine.gui->message(TCODColor::white, "There's a %s here\n", actor->name);
        }
    }
    
    owner->x = targetx;
    owner->y = targety;
    
    return true;
}

// ==================================================================================================================================
// PLAYER: HANDLEACTIONKEY()
// ----------------------------------------------------------------------------------------------------------------------------------
// Handles key events from the player that help them pick items into their inventory.
// ==================================================================================================================================
void PlayerAI::handleActionKey(Actor* owner, int code) 
{
    switch(code) 
    {
        case 'g' : // pickup item
        {
            bool picked = false;
            
            for (Actor** iterator = engine.actors.begin(); iterator != engine.actors.end(); iterator++) 
            {
                Actor* actor = *iterator;
                
                if ( actor->pickable && actor->x == owner->x && actor->y == owner->y ) 
                {
                    // Can the player pick up the item?
                    if ( actor->pickable->pick(actor, owner) ) 
                    {
                        picked = true;
                        engine.gui->message(TCODColor::lightGrey,"You pick the %s.", actor->name);
                        break;
                    } 
                    // If not, inventory must be full.
                    else if (!picked) 
                    {
                        picked = true;
                        engine.gui->message(TCODColor::red,"Your inventory is full.");
                    }
                }
            }
            
            // Didn't find anything
            if(!picked)
                engine.gui->message(TCODColor::lightGrey, "There's nothing here.");
            
            // Start new turn
            engine.gameStatus = NEW_TURN;
        }
        break;
        case 'i' : // Display inventory
        {
            // Get actor from inventory
            Actor* actor = chooseFromInventory(owner);
            
            // If it isn't null, use the item and start a new turn.
            if (actor) 
            {
                actor->pickable->use(actor,owner);
                engine.gameStatus = NEW_TURN;
            }
        }
        break;
        case 'd' : // drop item 
        {
            // Get actor from inventory
            Actor* actor = chooseFromInventory(owner);
            
            // If it isn't null, drop the item and start a new turn.
            if (actor) 
            {
                actor->pickable->drop(actor,owner);
                engine.gameStatus = NEW_TURN;
            }           
        }
        break;
        case '>' : // go down stairs
        {
            if ( engine.stairs->x == owner->x && engine.stairs->y == owner->y )
                engine.nextLevel();
            else
                engine.gui->message(TCODColor::lightGrey, "There are no stairs here.");
        }
        break;
    }
}

// ==================================================================================================================================
// PLAYER: CHOOSEFROMINVENTORY()
// ----------------------------------------------------------------------------------------------------------------------------------
// Displays inventory and wait for valid key press.
// ==================================================================================================================================
Actor* PlayerAI::chooseFromInventory(Actor* owner)
{
    // Draw inventory on screen
    engine.gui->renderInventory(owner);
    
    // Wait for a key press
    TCOD_key_t key;
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);
    
    if ( key.vk == TCODK_CHAR ) 
    {
        // Get item number in inventory (a - z)
        int actorIndex = key.c - 'a';
        
        // Get item
        if ( actorIndex >= 0 && actorIndex < owner->container->inventory.size() )
            return owner->container->inventory.get(actorIndex);
    }
    
    // No valid item was selected
    return NULL;
}

// ==================================================================================================================================
// PLAYER: GETNEXTLEVELXP
// ----------------------------------------------------------------------------------------------------------------------------------
// Returns the amount of XP for the next level.
// ==================================================================================================================================
int PlayerAI::getNextLevelXP()
{
    return LEVEL_UP_BASE + xpLevel*LEVEL_UP_FACTOR;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONFUSEDMONSTER: CONSTRUCTOR
ConfusedMonsterAI::ConfusedMonsterAI(int numTurns, AI* oldAI) : numTurns(numTurns), oldAI(oldAI) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// CONFUSEDMONSTER: UPDATE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Random moves the agent around and attacks anything it runs into.
// ==================================================================================================================================
void ConfusedMonsterAI::update(Actor* owner)
{
    // Get random direction to move
    TCODRandom* myRand = TCODRandom::getInstance();
    int dx = myRand->getInt(-1, 1);
    int dy = myRand->getInt(-1, 1);
    
    // If the agent moves in any direction
    if(dx != 0 || dy != 0)
    {
        int targetx = owner->x + dx;
        int targety = owner->y + dy;
        
        // If the target coordinates are walkable, walk on them
        if( engine.map->canWalk(targetx, targety) )
        {
            owner->x = targetx;
            owner->y = targety;
        }
        // Otherwise, try to attack them
        else
        {
           Actor* actor = engine.getActor(targetx, targety);
           
           if(actor)
               owner->attacker->attack(owner, actor);
        }
    }
    
    // Decrement the number of turns the agent is affected and, if its turns are up, then reset the AI
    numTurns--;
    if(numTurns == 0)
    {
        owner->ai = oldAI;
        delete this;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

