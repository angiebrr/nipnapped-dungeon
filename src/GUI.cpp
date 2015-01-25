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
    console = new TCODConsole(engine.screenWidth, PANEL_HEIGHT);
}

// GUI: DESTRUCTOR
GUI::~GUI()
{
    delete console;
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
    console->setDefaultBackground(TCODColor::black);
    console->clear();
    
    // Draw a health bar.
    renderBar(1, 1, BAR_WIDTH, "HP", engine.player->destructible->hp, engine.player->destructible->maxHp,
        TCODColor::lightRed,TCODColor::darkerRed);
    
    // Draw a message log.
    int y = 1;
    float fadeCoeff = 0.4f;
    for (Message** iterator = log.begin(); iterator != log.end(); iterator++) 
    {
        Message* message= *iterator;
        console->setDefaultForeground(message->color * fadeCoeff);
        console->print(MSG_X, y ,message->text);
        y++;
        
        // The oldest line will have 40% luminosity, the second oldest 70%, and all other 100%.
        if (fadeCoeff < 1.0f)
            fadeCoeff += 0.3f;
    }
    
    // Update mouse hover cells with actors on it
    renderMouseLook();

    
    // blit the GUI console on the root console
    // console, xSrc, ySrc, wSrc, hSrc, destination console, blitxSrc, blitySrc 
    TCODConsole::blit(console, 0, 0, engine.screenWidth, PANEL_HEIGHT, TCODConsole::root, 0, engine.screenHeight-PANEL_HEIGHT);
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
// GUI: RENDERBAR()
// ----------------------------------------------------------------------------------------------------------------------------------
// Adds health bar of player along with some text.
// ==================================================================================================================================
void GUI::renderBar(int x, int y, int width, const char* name, float value, float maxValue, const TCODColor& barColor,
    const TCODColor& backColor)
{
    // Fill the background
    console->setDefaultBackground(backColor);
    console->rect(x, y, width, 1, false, TCOD_BKGND_SET); // x, y, w, h, only bg changed, bg changed behavior
    
    // Get fraction of bar to be filled and fill it.
    int barWidth = (int)(value / maxValue * width);   
    if ( barWidth > 0 )
    {
        console->setDefaultBackground(barColor);
        console->rect(x, y, barWidth, 1, false, TCOD_BKGND_SET);
    }
    
    // Print text on top of the bar
    console->setDefaultForeground(TCODColor::white);
    // x, y, how bg change, alignment, printf string with parameters
    console->printEx(x + width/2, y, TCOD_BKGND_NONE, TCOD_CENTER, "%s : %g/%g", name, value, maxValue); 
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
    console->setDefaultForeground(TCODColor::lightGrey);
    console->print(1, 0, buffer);
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
