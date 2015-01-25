#include "main.hpp"

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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// PLAYER: UPDATE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Listens for player movements and updates actor's status accordingly.
// ==================================================================================================================================
void PlayerAI::update(Actor* owner) 
{
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
        default:break;
    }
    
    // If we tried to move, then start new turn, move/attack, and recompute FOV.
    if (dx != 0 || dy != 0) 
    {
        engine.gameStatus = Engine::NEW_TURN;
        
        if ( moveOrAttack(owner, owner->x + dx, owner->y + dy) )
            engine.map->computeFov();
    }
}

// ==================================================================================================================================
// PLAYER: moveOrAttack()
// ----------------------------------------------------------------------------------------------------------------------------------
// Whether or not actor can move. It can and does move if there isn't a wall or the actor isn't moving over them (attacking).
// ==================================================================================================================================
bool PlayerAI::moveOrAttack(Actor* owner, int targetx,int targety) 
{
    // Can't move or attack if it's a wall
    if ( engine.map->isWall(targetx, targety) ) 
        return false;

    // Loop through actors to see if played tried to move over (attack) a destructible actor..
    for (Actor** iterator = engine.actors.begin(); iterator != engine.actors.end(); iterator++) 
    {
        Actor* actor = *iterator;
         
        if ( actor->destructible && (actor->x == targetx) && (actor->y == targety) ) 
        {
            // If it isn't dead, attack. Can't move.
            if( !actor->destructible->isDead() )
            {
               owner->attacker->attack(owner, actor);
                return false; 
            }
            // It's dead. Actor can move over it.
            else
            {   
                printf("There's a %s here\n", actor->name);
            }
        }
    }
    
    owner->x = targetx;
    owner->y = targety;
    
    return true;
}

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
    if ( engine.map->isInFov(owner->x,owner->y) )
        moveOrAttack(owner, engine.player->x, engine.player->y);
}


// ==================================================================================================================================
// MONSTER: MOVEORATTACK()
// ----------------------------------------------------------------------------------------------------------------------------------
// 
// ==================================================================================================================================
void MonsterAI::moveOrAttack(Actor* owner, int targetx, int targety) 
{
    // Calculate Euclidean distance between monster and target coordinates
    int dx = targetx - owner->x;
    int dy = targety - owner->y;
    float distance = sqrtf( dx*dx + dy*dy );    
    
    // If we're out of melee range, go toward the target
    if ( distance >= 2 ) 
    {
        // Normalize the distance vector between the target and the monster
        dx = (int)( round(dx/distance) );
        dy = (int)( round(dy/distance) );
        
        // If the tiles are walkable, then walk on them. 
        if ( engine.map->canWalk(owner->x+dx,owner->y + dy) ) 
        {
            owner->x += dx;
            owner->y += dy;
        }
    }
    // If the monster is an attacker and is in melee range, attack!
    else if (owner->attacker) 
    {
        owner->attacker->attack(owner, engine.player);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

