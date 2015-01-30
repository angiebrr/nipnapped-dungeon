#include "main.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// Menu.cpp
// -----------------------------------------------------------------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// DESTRUCTOR
Menu::~Menu() 
{
    clear();
}

// CLEAR
void Menu::clear()
{
    items.clearAndDelete();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// ADDITEM()
// ----------------------------------------------------------------------------------------------------------------------------------
// Adds an item to the menu
// ==================================================================================================================================
void Menu::addItem(MenuItemCode code, const char* label) 
{
    MenuItem* item = new MenuItem();
    item->code = code;
    item->label = label;
    items.push(item);
}

// ==================================================================================================================================
// PICK()
// ----------------------------------------------------------------------------------------------------------------------------------
// Choose a menu item option.
// ==================================================================================================================================
MenuItemCode Menu::pick(DisplayMode mode) 
{
    int selectedItem = 0;
    int menux, menuy;
    
    if (mode == PAUSE) 
    {
        // Center pause menu
        menux = engine.screenWidth/2 - PAUSE_MENU_WIDTH/2;
        menuy = engine.screenHeight/2 - PAUSE_MENU_HEIGHT/2;
        
        // Draw frame
        TCODConsole::root->setDefaultForeground( TCODColor(200, 180, 50) );
        TCODConsole::root->printFrame( menux, menuy, PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT, true, TCOD_BKGND_ALPHA(70), "menu");
        
        // Offset slightly
        menux += 2;
        menuy += 3;
    }
    else 
    {
        static TCODImage img("menu_background1.png");
        img.blit2x(TCODConsole::root, 0, 0);
        menux = 10;
        menuy = TCODConsole::root->getHeight()/3;
    }
   
    // Start a main loop
    while( !TCODConsole::isWindowClosed() ) 
    {   
        // Light up selected item
        int currentItem = 0;
        for (MenuItem** iterator = items.begin(); iterator!=items.end(); iterator++) 
        {
            if (currentItem == selectedItem)
                TCODConsole::root->setDefaultForeground(TCODColor::lighterOrange);
            else
                TCODConsole::root->setDefaultForeground(TCODColor::lightGrey);

            TCODConsole::root->print(menux, menuy + currentItem*3, (*iterator)->label);
            
            currentItem++;
        }
        
        TCODConsole::flush();
 
        // Check for key press
        TCOD_key_t key;
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
        
        switch (key.vk) 
        {
            case TCODK_UP: // Get previous item
            {
                selectedItem--; 
                if (selectedItem < 0)
                    selectedItem = items.size() - 1;
            }
            break;
            case TCODK_DOWN: // Modulo to get item
            {
                selectedItem = (selectedItem + 1) % items.size();
            }
            break;
            case TCODK_ENTER: // Return item index
            {
                return items.get(selectedItem)->code;
            }
            default : break;
        }
    }
    
    // Window was closed
    return NONE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


