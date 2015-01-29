#ifndef GAMECONSTANTS_HPP
#define	GAMECONSTANTS_HPP

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// GameConstants.hpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Constants for the game to try to avoid magic numbers.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNappedDungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// ENUMS
// ==================================================================================================================================

enum GameStatus 
{
    STARTUP, // First frame of the game.
    IDLE, // No new turn. Redraw the same screen.
    NEW_TURN, // Update the monster's position.
    VICTORY, // The player won.
    DEFEAT // The player was killed.
 };
 
enum MenuItemCode 
{
    NONE, // Close game window
    NEW_GAME, // Load a new game
    CONTINUE, // Load a previous game
    EXIT, // Exit game
    CONSTITUTION, // Level up constitution
    STRENGTH, // Level up strength
    AGILITY // Level up agility
};

enum DisplayMode
{
    MAIN, // Main menu
    PAUSE // Pause menu
};

enum LevelCode
{
    LEVEL_ONE, LEVEL_TWO, LEVEL_THREE, LEVEL_FOUR, LEVEL_FIVE
};

enum MonsterType
{
    MOUSE, PUPPY, DOG, VACUUM, KITTY_THE_GRAY
};

enum AIType 
{
    MONSTER_AI, CONFUSED_MONSTER_AI, PLAYER_AI
};

enum DestructibleType 
{
    MONSTER, PLAYER
};

enum PickableType 
{
    HEALER, LIGHTNING_BOLT, CONFUSER, FIREBALL
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// GUI CONSTANTS
// ==================================================================================================================================
namespace GUIConstants
{
    static const int PANEL_HEIGHT = 7;
    static const int BAR_WIDTH = 20;
    static const int MSG_X = BAR_WIDTH + 2;
    static const int MSG_HEIGHT = PANEL_HEIGHT - 1;
    static const int INVENTORY_WIDTH = 50;
    static const int INVENTORY_HEIGHT = 28;
    static const int PAUSE_MENU_WIDTH = 30;
    static const int PAUSE_MENU_HEIGHT = 15;

    static int SCREEN_WIDTH = 80;
    static int SCREEN_HEIGHT = 50;
    static bool IS_FULLSCREEN = false;
    static const char* WINDOW_TITLE = "NipNapped Dungeon";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// LEVEL CONSTANTS
// ==================================================================================================================================

namespace LevelConstants
{
    // Map colors
    static const TCODColor darkWall(0, 0, 100);
    static const TCODColor darkGround(50, 50, 150);
    static const TCODColor lightWall(130, 110, 50);
    static const TCODColor lightGround(200, 180, 50);

    // Camera
    static int FOV_RADIUS = 10;

    // Room constants
    static const int ROOM_MAX_SIZE = 12;
    static const int ROOM_MIN_SIZE = 6;
    static const int MAX_ROOM_MONSTERS = 3;
    static const int MAX_ROOM_ITEMS = 2;
    static const int RECURSION_DEPTH = 8;
    static const float MAX_H_RATIO = 1.5f;
    static const float MAX_V_RATIO = 1.5f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// ACTOR CONSTANTS
// ==================================================================================================================================

namespace ActorConstants
{
    // Types of monsters that will spawn in each level
    // 5th level will be boss battle with KITTY
    static const int LEVEL_MONSTERS[4][5] =
    { 
        { MOUSE, MOUSE, MOUSE, MOUSE, MOUSE },
        { MOUSE, MOUSE, PUPPY, PUPPY, PUPPY },
        { MOUSE, MOUSE, PUPPY, PUPPY, DOG },
        { MOUSE, PUPPY, DOG, DOG, VACUUM }
    };
    
    // Experience-related constants
    static const int LEVEL_UP_BASE = 200;
    static const int LEVEL_UP_FACTOR = 150;
    
    // Player attributes
    static const int PLAYER_DEFAULT_X = 40;
    static const int PLAYER_DEFAULT_Y = 25;
    static const char* PLAYER_NAME = "Lucky";
    static const char* PLAYER_CORPSE_NAME = "Lucky's corpse";
    static const float PLAYER_MAX_HEALTH = 50;
    static const float PLAYER_DEFENSE = 2;
    static const float PLAYER_ATTACK = 5;
    static const char PLAYER_CHAR = '@';
    static const TCODColor PLAYER_COLOR = TCODColor::white;
    static const int PLAYER_BASE_XP_DROP = 0;

    // Mouse atrributes
    static const char* MOUSE_NAME = "Infected Mouse";
    static const char* MOUSE_CORPSE_NAME = "Perished Mouse";
    static const float MOUSE_MAX_HEALTH = 5;
    static const float MOUSE_DEFENSE = 0;
    static const float MOUSE_ATTACK = 1;
    static const char MOUSE_CHAR = 'm';
    static const TCODColor MOUSE_COLOR = TCODColor::desaturatedSky;
    static const int MOUSE_BASE_XP_DROP = 50;

    // Puppy attributes
    static const char* PUPPY_NAME = "Evil Puppy";
    static const char* PUPPY_CORPSE_NAME = "Dead Puppy";
    static const float PUPPY_MAX_HEALTH = 7;
    static const float PUPPY_DEFENSE = 0;
    static const float PUPPY_ATTACK = 1;
    static const char PUPPY_CHAR = 'p';
    static const TCODColor PUPPY_COLOR = TCODColor::violet;
    static const int PUPPY_BASE_XP_DROP = 75;

    // Dog attributes
    static const char* DOG_NAME = "Rabid Dog";
    static const char* DOG_CORPSE_NAME = "Slain Dog";
    static const float DOG_MAX_HEALTH = 10;
    static const float DOG_DEFENSE = 1;
    static const float DOG_ATTACK = 3;
    static const char DOG_CHAR = 'D';
    static const TCODColor DOG_COLOR = TCODColor::magenta;
    static const int DOG_BASE_XP_DROP = 95;

    // Vacuum attributes
    static const char* VACUUM_NAME = "Vicious Vacuum";
    static const char* VACUUM_CORPSE_NAME = "Dead Vacuum";
    static const float VACUUM_MAX_HEALTH = 15;
    static const float VACUUM_DEFENSE = 2;
    static const float VACUUM_ATTACK = 4;
    static const char VACUUM_CHAR = 'V';
    static const TCODColor VACUUM_COLOR = TCODColor::magenta;
    static const int VACUUM_BASE_XP_DROP = 125;

    // Kitty the gray attributes
    static const char* KITTY_THE_GRAY_NAME = "Kitty The Gray";
    static const char* KITTY_THE_GRAY_CORPSE_NAME = "Dead as door nails Kitty The Gray.";
    static const float KITTY_THE_GRAY_HEALTH = 75;
    static const float KITTY_THE_GRAY_DEFENSE = 3;
    static const float KITTY_THE_GRAY_ATTACK = 8;
    static const char KITTY_THE_GRAY_CHAR = 'K';
    static const TCODColor KITTY_THE_GRAY_COLOR = TCODColor::crimson;
    static const int KITTY_THE_GRAY_BASE_XP_DROP = 500;

    // Default symbols and colors for items on map
    static const char POTION_CHAR = '!';
    static const char SCROLL_CHAR = '#';
    static const char STAIRS_CHAR = '>';
    static const TCODColor POTION_COLOR = TCODColor::lightGreen;
    static const TCODColor SCROLL_COLOR = TCODColor::lightYellow;
    static const TCODColor STAIRS_COLOR = TCODColor::white;

    // Names for items
    static const char* HEALER_NAME = "health potion";
    static const char* LIGHTNING_NAME = "scroll of lightning bolt";
    static const char* FIREBALL_NAME = "scroll of fireball";
    static const char* CONFUSER_NAME = "scroll of confusion";
    static const char* STAIRS_NAME = "stairs";

    // Colors for items in inventory
    static const TCODColor HEALER_COLOR = TCODColor::green;
    static const TCODColor LIGHTNING_COLOR = TCODColor::yellow;
    static const TCODColor FIREBALL_COLOR = TCODColor::amber;
    static const TCODColor CONFUSER_COLOR = TCODColor::sky;

    // Numbers associated with items
    static const float HEALER_AMOUNT = 4;
    static const float LIGHTNING_RANGE = 5;
    static const float LIGHTNING_DAMAGE = 20;
    static const float FIREBALL_RANGE = 3;
    static const float FIREBALL_DAMAGE = 12;
    static const float CONFUSER_RANGE = 10;
    static const int CONFUSER_NUM_TURNS = 3;

    // Inventory items
    static const bool IS_STACKABLE = true;
    static const int ONE_ITEM_COUNT = 1;

    // How many turns the monster chases the player after losing his sight
    static const int TRACKING_TURNS = 3;

    // Player inventory size
    static const int INVENTORY_SIZE = 26;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// MISC CONSTANTS
// ==================================================================================================================================


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif	// GAMECONSTANTS_HPP
