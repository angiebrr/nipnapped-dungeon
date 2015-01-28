#ifndef GUI_HPP
#define	GUI_HPP

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// GUI.hpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Header for a class that maintains and builds the game's GUI.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GUI : IPersistent
{
    public:
        Menu menu;
        
        GUI();
        ~GUI();
        void clear();
        void render();
        void message(const TCODColor& color, const char* text, ...);
        void renderInventory(Actor* owner);
        void clearInventoryConsole();
        void load(TCODZip& zip);
        void save(TCODZip& zip);

    protected:
        struct Message 
        {
            char* text;
            TCODColor color;
            Message(const char* text, const TCODColor& color);
            ~Message();
        };
        
        TCODList<Message*> log;
        TCODConsole* inventoryConsole;
        TCODConsole* mainConsole;

        void renderBar(int x, int y, int width, const char* name, float value, float maxValue, const TCODColor& barColor,
            const TCODColor& backColor);
        void renderMouseLook();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif	// GUI_HPP

