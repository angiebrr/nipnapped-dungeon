#include "main.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// IPersistent.cpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Implementations of saving and loading features for different parts of the game.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// ENGINE
// ==================================================================================================================================

// ----------------------------------------------------------------------------------------------------------------------------------
// Load game if save file exists. Otherwise, make a new game.
// ----------------------------------------------------------------------------------------------------------------------------------
void Engine::load() 
{
    // Clear menu
    engine.gui->menu.clear();
    
    // Add new game menu item
    engine.gui->menu.addItem(NEW_GAME, "New game");
    
    // If the file exists, add continue menu item
    if ( TCODSystem::fileExists("game.sav") )
        engine.gui->menu.addItem(CONTINUE, "Continue");
    
    // Add exit menu item
    engine.gui->menu.addItem(EXIT, "Exit");
    
    // Wait for the user to choose an item
    MenuItemCode menuItem = engine.gui->menu.pick();
    
    // User chose exit or closed windows
    if ( menuItem == EXIT || menuItem == NONE ) 
    {
        exit(0);
    }
    // User chose a new game
    else if(menuItem == NEW_GAME)
    {
        engine.term();
        engine.init();
    }
    else
    {
        TCODZip zip;
        engine.term();
        zip.loadFromFile("game.sav");
        
        // Load the map
        int width = zip.getInt();
        int height = zip.getInt();
        map = new Map(width, height);
        map->load(zip);
        
        // Load the player
        player = new Actor(0, 0, 0, NULL, TCODColor::white);
        player->load(zip);
        actors.push(player);
        
        // Load all the actors
        int numActors = zip.getInt();
        while (numActors > 0) 
        {
                Actor* actor = new Actor(0, 0, 0, NULL, TCODColor::white);
                actor->load(zip);
                actors.push(actor);
                numActors--;
        }
        
        // Load the GUI
        gui->load(zip);
        
        // Force FOV computation
        gameStatus = STARTUP;
    }    
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Save the game if the player isn't dead.
// ----------------------------------------------------------------------------------------------------------------------------------
void Engine::save() 
{   
    if ( player->destructible->isDead() ) 
    {
        TCODSystem::deleteFile("game.sav");
    } 
    else 
    {
        TCODZip zip;
        
        // Save the map
        zip.putInt(map->width);
        zip.putInt(map->height);
        map->save(zip);
        
        // Save the player
        player->save(zip);
        
        // Save all the actors
        zip.putInt(actors.size() - 1);
        for (Actor** iterator = actors.begin(); iterator != actors.end(); iterator++) 
        {
            if (*iterator != player)
                    (*iterator)->save(zip);
        }
        
        // Save the GUI
        gui->save(zip);
        
        zip.saveToFile("game.sav");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// MAP
// ==================================================================================================================================

// ----------------------------------------------------------------------------------------------------------------------------------
// Load the map.
// ----------------------------------------------------------------------------------------------------------------------------------
void Map::load(TCODZip& zip) 
{
    // Get the seed
    seed = zip.getInt();

    // Initialize the map
    init(false);
    
    // Populate tiles
    for (int i = 0; i < width*height; i++)
        tiles[i].explored=zip.getInt();
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Save the map.
// ----------------------------------------------------------------------------------------------------------------------------------
void Map::save(TCODZip& zip) 
{
    // Save the seed
    zip.putInt(seed);
    
    // Save the tiles
    for (int i = 0; i < width*height; i++)
        zip.putInt(tiles[i].explored);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// GUI
// ==================================================================================================================================

// ----------------------------------------------------------------------------------------------------------------------------------
// Load the GUI log messages.
// ----------------------------------------------------------------------------------------------------------------------------------
void GUI::load(TCODZip& zip) 
{
    int numMessages = zip.getInt();
    while (numMessages > 0) 
    {
        const char* text = zip.getString();
        TCODColor color = zip.getColor();
        message(color, text);
        numMessages--;
    }
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Save the GUI log messages.
// ----------------------------------------------------------------------------------------------------------------------------------
void GUI::save(TCODZip& zip) 
{
    zip.putInt( log.size() );
    
    for (Message**iterator = log.begin(); iterator != log.end(); iterator++) 
    {
        zip.putString((*iterator)->text);
        zip.putColor(&(*iterator)->color);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// ACTOR
// ==================================================================================================================================

// ----------------------------------------------------------------------------------------------------------------------------------
// Load actor attributes and features.
// ----------------------------------------------------------------------------------------------------------------------------------
void Actor::load(TCODZip& zip) 
{
    // Load attributes
    x = zip.getInt();
    y = zip.getInt();
    code = zip.getInt();
    color = zip.getColor();
    name = strdup(zip.getString());
    blocks = zip.getInt();

    // Figure out what features actor has
    bool hasAttacker = zip.getInt();
    bool hasDestructible = zip.getInt();
    bool hasAI = zip.getInt();
    bool hasPickable = zip.getInt();
    bool hasContainer = zip.getInt();
    
    // Create and load features that the actor has
    if (hasAttacker) 
    {
        attacker = new Attacker(0.0f);
        attacker->load(zip);
    }
    if (hasDestructible) 
    {
        destructible = Destructible::create(zip);
    }
    if (hasAI) 
    {
        ai = AI::create(zip);
    }
    if (hasPickable) 
    {
        pickable = Pickable::create(zip);
    }
    if (hasContainer) 
    {
        container = new Container(0);
        container->load(zip);
    }
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Save actor attributes and features.
// ----------------------------------------------------------------------------------------------------------------------------------
void Actor::save(TCODZip& zip) 
{
    // Save main attributes
    zip.putInt(x);
    zip.putInt(y);
    zip.putInt(code);
    zip.putColor(&color);
    zip.putString(name);
    zip.putInt(blocks);
    
    // Keep track of what features the actor has
    zip.putInt(attacker != NULL);
    zip.putInt(destructible != NULL);
    zip.putInt(ai != NULL);
    zip.putInt(pickable != NULL);
    zip.putInt(container != NULL);
    
    // Save the features that we have
    if (attacker) attacker->save(zip);
    if (destructible) destructible->save(zip);
    if (ai) ai->save(zip);
    if (pickable) pickable->save(zip);
    if (container) container->save(zip);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// ATTACKER
// ==================================================================================================================================

// ----------------------------------------------------------------------------------------------------------------------------------
// Load attacker attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void Attacker::load(TCODZip& zip) 
{
    power = zip.getFloat();
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Save attacker attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void Attacker::save(TCODZip& zip) 
{
    zip.putFloat(power);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// DESTRUCTIBLE
// ==================================================================================================================================

// ----------------------------------------------------------------------------------------------------------------------------------
// Creates and loads the saved instance of the destructible.
// ----------------------------------------------------------------------------------------------------------------------------------
Destructible* Destructible::create(TCODZip &zip) 
{
    Destructible* destructible = NULL;
    
    // Get type of instance
    DestructibleType type = (DestructibleType)zip.getInt();
    
    // Create new instance
    switch(type) 
    {
        case MONSTER : destructible = new MonsterDestructible(0, 0, NULL); break;
        case PLAYER : destructible = new PlayerDestructible(0, 0, NULL); break;
    }
    
    // Load it
    destructible->load(zip);
    
    return destructible;
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Loads destructible attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void Destructible::load(TCODZip& zip) 
{
    maxHp = zip.getFloat();
    hp = zip.getFloat();
    defense = zip.getFloat();
    corpseName = strdup(zip.getString());
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Saves destructible attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void Destructible::save(TCODZip& zip) 
{
    zip.putFloat(maxHp);
    zip.putFloat(hp);
    zip.putFloat(defense);
    zip.putString(corpseName);
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Saves player destructible attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void PlayerDestructible::save(TCODZip& zip) 
{
    zip.putInt(PLAYER);
    Destructible::save(zip);
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Saves monster destructible attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void MonsterDestructible::save(TCODZip& zip) 
{
    zip.putInt(MONSTER);
    Destructible::save(zip);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// PICKABLE
// ==================================================================================================================================

// ----------------------------------------------------------------------------------------------------------------------------------
// Creates and loads the saved instance of the pickable.
// ----------------------------------------------------------------------------------------------------------------------------------
Pickable* Pickable::create(TCODZip& zip) 
{
    Pickable* pickable = NULL;
    
    // Get type of instance
    PickableType type = (PickableType)zip.getInt();
    
    // Create new instance
    switch(type) 
    {
        case HEALER : pickable = new Healer(1, true, TCODColor::white, HEALER, 0); break;
        case LIGHTNING_BOLT : pickable = new LightningBolt(1, true, TCODColor::white, LIGHTNING_BOLT, 0, 0); break;
        case CONFUSER : pickable = new Confuser(1, true, TCODColor::white, CONFUSER, 0, 0); break;
        case FIREBALL : pickable = new FireBall(1, true, TCODColor::white, FIREBALL, 0, 0); break;
    }
    
    // Load instance
    pickable->load(zip);
    
    return pickable;
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Loads pickable attributes
// ----------------------------------------------------------------------------------------------------------------------------------
void Pickable::load(TCODZip& zip)
{
    count = zip.getInt();
    stackable = zip.getInt();
    color = zip.getColor();
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Saves pickable attributes
// ----------------------------------------------------------------------------------------------------------------------------------
void Pickable::save(TCODZip& zip)
{
    zip.putInt(count);
    zip.putInt(stackable);
    zip.putColor(&color);
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Loads healer attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void Healer::load(TCODZip& zip) 
{
    amount = zip.getFloat();
    Pickable::load(zip);
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Saves healer attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void Healer::save(TCODZip& zip) 
{
    zip.putInt(type);
    zip.putFloat(amount);
    Pickable::save(zip);
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Loads lightning bolt attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void LightningBolt::load(TCODZip& zip) 
{
    range = zip.getFloat();
    damage = zip.getFloat();
    Pickable::load(zip);
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Saves lightning bolt attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void LightningBolt::save(TCODZip& zip) 
{
    zip.putInt(type);
    zip.putFloat(range);
    zip.putFloat(damage);
    Pickable::save(zip);
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Loads confuser attributes
// ----------------------------------------------------------------------------------------------------------------------------------
void Confuser::load(TCODZip& zip) 
{
    numTurns = zip.getInt();
    range = zip.getFloat();
    Pickable::load(zip);
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Saves confuser attributes
// ----------------------------------------------------------------------------------------------------------------------------------
void Confuser::save(TCODZip& zip) 
{
    zip.putInt(type);
    zip.putInt(numTurns);
    zip.putFloat(range);
    Pickable::save(zip);
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Loads fireball attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void FireBall::load(TCODZip& zip) 
{
    range = zip.getFloat();
    damage = zip.getFloat();
    Pickable::load(zip);
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Saves fireball attributes
// ----------------------------------------------------------------------------------------------------------------------------------
void FireBall::save(TCODZip& zip) 
{
    zip.putInt(type);
    zip.putFloat(range);
    zip.putFloat(damage);
    Pickable::save(zip);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// CONTAINER
// ==================================================================================================================================

// ----------------------------------------------------------------------------------------------------------------------------------
// Load container items.
// ----------------------------------------------------------------------------------------------------------------------------------
void Container::load(TCODZip& zip) 
{
    // Get size and number of actors in the inventory
    size = zip.getInt();
    int numActors = zip.getInt();
    
    // Load actors into inventory
    while (numActors > 0) 
    {
        Actor* actor = new Actor(0, 0, 0, NULL, TCODColor::white);
        actor->load(zip);
        inventory.push(actor);
        numActors--;
    }
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Save container items.
// ----------------------------------------------------------------------------------------------------------------------------------
void Container::save(TCODZip& zip) 
{
    // Save size and number of actors
    zip.putInt(size);
    zip.putInt( inventory.size() );
    
    // Save actors
    for (Actor** iterator=inventory.begin(); iterator != inventory.end(); iterator++)
        (*iterator)->save(zip);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// AI
// ==================================================================================================================================

// ----------------------------------------------------------------------------------------------------------------------------------
// Creates and loads the saved instance of the AI.
// ----------------------------------------------------------------------------------------------------------------------------------
AI* AI::create(TCODZip& zip) 
{           
    AI* ai = NULL;

    // Get type of instance
    AIType type = (AIType)zip.getInt();

    // Create new instance
    switch(type) 
    {
        case PLAYER_AI : ai = new PlayerAI(); break;
        case MONSTER_AI : ai = new MonsterAI(); break;
        case CONFUSED_MONSTER_AI : ai = new ConfusedMonsterAI(0, NULL); break;
    }

    // Load instance
    ai->load(zip);

    return ai;
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Load monster AI attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void MonsterAI::load(TCODZip& zip) 
{
    moveCount = zip.getInt();
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Save monster AI attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void MonsterAI::save(TCODZip& zip) 
{
    zip.putInt(MONSTER_AI);
    zip.putInt(moveCount);
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Load confused monster AI attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void ConfusedMonsterAI::load(TCODZip& zip) 
{
    numTurns = zip.getInt();
    oldAI = AI::create(zip);
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Save confused monster AI attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void ConfusedMonsterAI::save(TCODZip& zip) 
{
    zip.putInt(CONFUSED_MONSTER_AI);
    zip.putInt(numTurns);
    oldAI->save(zip);
}

// ----------------------------------------------------------------------------------------------------------------------------------
// Load player AI attributes, but there are none.
// ----------------------------------------------------------------------------------------------------------------------------------
void PlayerAI::load(TCODZip& zip) {}

// ----------------------------------------------------------------------------------------------------------------------------------
// Save player AI attributes.
// ----------------------------------------------------------------------------------------------------------------------------------
void PlayerAI::save(TCODZip& zip) 
{
    zip.putInt(PLAYER_AI);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
