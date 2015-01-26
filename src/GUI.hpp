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

 // CLASS: GUI
class GUI 
{
    public:
        static const int PANEL_HEIGHT = 7;
        static const int BAR_WIDTH = 20;
        static const int MSG_X = BAR_WIDTH + 2;
        static const int MSG_HEIGHT = PANEL_HEIGHT - 1;
        static const int INVENTORY_WIDTH = 50;
        static const int INVENTORY_HEIGHT = 28;
        
        GUI();
        ~GUI();
        void render();
        void message(const TCODColor& color, const char* text, ...);
        void renderInventory(Actor* owner);
        void clearInventoryConsole();

    protected:
        // STRUCT: MESSAGE
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

