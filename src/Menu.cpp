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
MenuItemCode Menu::pick() 
{
    int selectedItem = 0;
    
    // Load menu image
    static TCODImage img("menu_background1.png");
   
    // Start a main loop
    while( !TCODConsole::isWindowClosed() ) 
    {
        img.blit2x(TCODConsole::root, 0, 0);
        
        // Light up selected item
        int currentItem = 0;
        for (MenuItem** iterator = items.begin(); iterator!=items.end(); iterator++) 
        {
            if (currentItem == selectedItem)
                TCODConsole::root->setDefaultForeground(TCODColor::lighterOrange);
            else
                TCODConsole::root->setDefaultForeground(TCODColor::lightGrey);

            TCODConsole::root->print(10, 10 + currentItem*3, (*iterator)->label);
            
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


