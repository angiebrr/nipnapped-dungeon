#include "main.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// GUI.cpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Class that maintains and builds the game's GUI.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// GUI: CONSTRUCTOR
GUI::GUI()
{
    mainConsole = new TCODConsole(SCREEN_WIDTH, PANEL_HEIGHT);
    inventoryConsole = new TCODConsole(INVENTORY_WIDTH, INVENTORY_HEIGHT);
}

// GUI: DESTRUCTOR
GUI::~GUI()
{
    delete mainConsole;
    delete inventoryConsole;
    clear();
}

// GUI: CLEAR
void GUI::clear()
{
    log.clearAndDelete();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// GUI: RENDER()
// ----------------------------------------------------------------------------------------------------------------------------------
// Clears console, draws health bar, and blits the GUI console on the root console.
// ==================================================================================================================================
void GUI::render()
{
    // Clear the console.
    mainConsole->setDefaultBackground(TCODColor::black);
    mainConsole->clear();
    
    // Draw a health bar.
    renderBar(1, 1, BAR_WIDTH, "HP", engine.player->destructible->hp, engine.player->destructible->maxHp,
        TCODColor::lightRed,TCODColor::darkerRed);
    
    // Draw a message log.
    int y = 1;
    float fadeCoeff = 0.4f;
    for (Message** iterator = log.begin(); iterator != log.end(); iterator++) 
    {
        Message* message= *iterator;
        mainConsole->setDefaultForeground(message->color * fadeCoeff);
        mainConsole->print(MSG_X, y ,message->text);
        y++;
        
        // The oldest line will have 40% luminosity, the second oldest 70%, and all other 100%.
        if (fadeCoeff < 1.0f)
            fadeCoeff += 0.3f;
    }
    
    // Update mouse hover cells with actors on it
    renderMouseLook();
    
    // Display dungeon level
    mainConsole->setDefaultForeground(TCODColor::white);
    mainConsole->print(3, 3, "Dungeon Level %d", engine.level);
    
    // Display controls
    //mainConsole->setDefaultForeground(TCODColor::grey);
    //mainConsole->print(0, 0, "[i] inv    [g] pick up    [d] drop    [>] descend    [esc] pause");
    
    // Draw an XP bar
    PlayerAI* ai = (PlayerAI*)engine.player->ai;
    char xpTxt[128];
    sprintf(xpTxt,"XP(%d)", ai->xpLevel);
    renderBar(1, 5, BAR_WIDTH, xpTxt, engine.player->destructible->xp, ai->getNextLevelXP(), TCODColor::lightViolet, 
            TCODColor::darkerViolet);

    
    // blit the GUI console on the root console
    // console, xSrc, ySrc, wSrc, hSrc, destination console, blitxSrc, blitySrc 
    TCODConsole::blit(mainConsole, 0, 0, engine.screenWidth, PANEL_HEIGHT, TCODConsole::root, 0, engine.screenHeight-PANEL_HEIGHT);
}

// ==================================================================================================================================
// GUI: MESSAGE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Prints message out to log on GUI.
// ==================================================================================================================================
void GUI::message(const TCODColor& color, const char* text, ...)
{
    // Build the text
    va_list ap;
    char buffer[128];
    va_start(ap, text);
    vsprintf(buffer, text, ap);
    va_end(ap);
    
    // Hold beginning and end
    char* lineBegin = buffer;
    char* lineEnd;
    
    // Write a new line to the log.
    do 
    {
        // Make room for the new message if log is full.
        if ( log.size() == MSG_HEIGHT ) 
        {
            Message* toRemove=log.get(0);
            log.remove(toRemove);
            delete toRemove;
        }
        
        // Detect end of the line (find carriage return) to make it single-lined.
        lineEnd = strchr(lineBegin,'\n');
        
        // Insert null terminator
        if(lineEnd)
            *lineEnd = '\0';
        
        // Add the new (possibly split) message to the log.
        Message* msg = new Message(lineBegin, color);
        log.push(msg);
        
        // Go to next line
        lineBegin = lineEnd + 1;
    } while(lineEnd);
}

// ==================================================================================================================================
// GUI: RENDERINVENTORY()
// ----------------------------------------------------------------------------------------------------------------------------------
// Renders inventory on the screen.
// ==================================================================================================================================
void GUI::renderInventory(Actor* owner)
{   
    // Display the inventory frame
    inventoryConsole->setDefaultForeground( TCODColor(200, 180, 50) );
    inventoryConsole->printFrame(0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "Inventory");
    
    // Display the items with their keyboard shortcut (a - z)
    inventoryConsole->setDefaultForeground(TCODColor::white);
    int shortcut = 'a';
    int y = 1;
    for (Actor** iterator = owner->container->inventory.begin(); iterator != owner->container->inventory.end(); iterator++) 
    {
        Actor* actor = *iterator;
        
        // Print color, count, shortcut, and item name
        inventoryConsole->setCharBackground(2, y, actor->pickable->color);
        inventoryConsole->print(4, y, "(%c) %-s [%003i]", shortcut, actor->name, actor->pickable->count);
        
        // Update y and shortcut letter
        y += 2;
        shortcut++;
    }
    
    // blit the inventory console on the root console
    TCODConsole::blit(inventoryConsole, 0, 0, INVENTORY_WIDTH,INVENTORY_HEIGHT, TCODConsole::root, 
        engine.screenWidth/2 - INVENTORY_WIDTH/2, engine.screenHeight/2 - INVENTORY_HEIGHT/2);
    
    TCODConsole::flush();
}

// ==================================================================================================================================
// GUI: CLEARINVENTORYCONSOLE()
// ----------------------------------------------------------------------------------------------------------------------------------
// Resets inventory console colors to default background color.
// ==================================================================================================================================
void GUI::clearInventoryConsole()
{
    inventoryConsole->clear();
    TCODConsole::flush();
}

// ==================================================================================================================================
// GUI: RENDERBAR()
// ----------------------------------------------------------------------------------------------------------------------------------
// Adds health bar of player along with some text.
// ==================================================================================================================================
void GUI::renderBar(int x, int y, int width, const char* name, float value, float maxValue, const TCODColor& barColor,
    const TCODColor& backColor)
{
    // Fill the background
    mainConsole->setDefaultBackground(backColor);
    mainConsole->rect(x, y, width, 1, false, TCOD_BKGND_SET); // x, y, w, h, only bg changed, bg changed behavior
    
    // Get fraction of bar to be filled and fill it.
    int barWidth = (int)(value / maxValue * width);   
    if ( barWidth > 0 )
    {
        mainConsole->setDefaultBackground(barColor);
        mainConsole->rect(x, y, barWidth, 1, false, TCOD_BKGND_SET);
    }
    
    // Print text on top of the bar
    mainConsole->setDefaultForeground(TCODColor::white);
    // x, y, how bg change, alignment, printf string with parameters
    mainConsole->printEx(x + width/2, y, TCOD_BKGND_NONE, TCOD_CENTER, "%s : %g/%g", name, value, maxValue); 
}

// ==================================================================================================================================
// GUI: RENDERMOUSELOOK()
// ----------------------------------------------------------------------------------------------------------------------------------
// Draws some text for what the player is hovering over (if it's in the players field of view)
// ==================================================================================================================================
void GUI::renderMouseLook()
{
    // Don't render anything if mouse's coordinates are outside player's field of view
    if ( !engine.map->isInFov(engine.mouse.cx, engine.mouse.cy) )
        return;
    
    // List for what's on cell
    char buffer[128] = {'\0'};
    
    // Iterate over actors and find some that are under the cursor
    bool first = true;
    for (Actor** iterator = engine.actors.begin(); iterator != engine.actors.end(); iterator++) 
    {
        Actor* actor = *iterator;
        
        if (actor->x == engine.mouse.cx && actor->y == engine.mouse.cy ) 
        {
            // If it isn't the first, add a comma.
            if (!first) 
                strcat(buffer, ", ");
            else
                first = false;
            
            strcat(buffer, actor->name);
        }
    }
    
    // Display the list of actors under the mouse cursor
    mainConsole->setDefaultForeground(TCODColor::lightGrey);
    mainConsole->print(1, 0, buffer);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MESSAGE: CONSTRUCTOR
GUI::Message::Message(const char* text, const TCODColor& color) : text( strdup(text) ), color(color) {}

// MESSAGE: DESTRUCTOR
GUI::Message::~Message() 
{
    free(text);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
