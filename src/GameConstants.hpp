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
    LEVEL_ONE = 1, 
    LEVEL_TWO = 2, 
    LEVEL_THREE = 3, 
    LEVEL_FOUR = 4, 
    LEVEL_FIVE = 5,
    FIRST_LEVEL = LEVEL_ONE,
    LAST_LEVEL = LEVEL_FIVE
};

enum AttributeType
{
    HEALTH,
    ATTACK,
    DEFENSE,
    XP
};

enum MonsterType
{
    MOUSE, 
    PUPPY, 
    DOG, 
    VACUUM, 
    KITTY_THE_GRAY,
    FIRST_MONSTER = MOUSE,
    LAST_MONSTER = KITTY_THE_GRAY
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
extern const int PANEL_HEIGHT;
extern const int BAR_WIDTH;
extern const int MSG_X;
extern const int MSG_HEIGHT;
extern const int INVENTORY_WIDTH;
extern const int INVENTORY_HEIGHT;
extern const int PAUSE_MENU_WIDTH;
extern const int PAUSE_MENU_HEIGHT;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern bool IS_FULLSCREEN;
extern const char* WINDOW_TITLE;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// LEVEL CONSTANTS
// ==================================================================================================================================

// Map colors
extern const TCODColor darkWall;
extern const TCODColor darkGround;
extern const TCODColor lightWall;
extern const TCODColor lightGround;

// Camera
extern int FOV_RADIUS;

// Room constants
extern const int ROOM_MAX_SIZE;
extern const int ROOM_MIN_SIZE;
extern const int MAX_ROOM_MONSTERS;
extern const int MAX_ROOM_ITEMS;
extern const int RECURSION_DEPTH;
extern const float MAX_H_RATIO;
extern const float MAX_V_RATIO;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// ACTOR CONSTANTS
// ==================================================================================================================================

// Slots for each monster
const int NUM_MONSTER_SLOTS = 5;

// Num attributes
const int NUM_ATTRIBUTES = 4;

// Types of monsters that will spawn in each level
// 5th level will be boss battle with KITTY
extern const MonsterType LEVEL_MONSTERS[LAST_LEVEL][NUM_MONSTER_SLOTS];
    
// Bool for boss
extern const bool IS_BOSS;

// Experience-related constants
extern const int LEVEL_UP_BASE;
extern const int LEVEL_UP_FACTOR;

// Player attributes
extern const int PLAYER_DEFAULT_X;
extern const int PLAYER_DEFAULT_Y;
extern const char* PLAYER_NAME;
extern const char* PLAYER_CORPSE_NAME;
extern const float PLAYER_MAX_HEALTH;
extern const float PLAYER_DEFENSE;
extern const float PLAYER_ATTACK;
extern const char PLAYER_CHAR;
extern const TCODColor PLAYER_COLOR;
extern const int PLAYER_BASE_XP_DROP;

// Mouse atrributes
extern const char* MOUSE_NAME;
extern const char* MOUSE_CORPSE_NAME;
extern float MOUSE_MAX_HEALTH;
extern float MOUSE_DEFENSE;
extern float MOUSE_ATTACK;
extern const char MOUSE_CHAR;
extern const TCODColor MOUSE_COLOR;
extern int MOUSE_BASE_XP_DROP;

// Puppy attributes
extern const char* PUPPY_NAME;
extern const char* PUPPY_CORPSE_NAME;
extern float PUPPY_MAX_HEALTH;
extern float PUPPY_DEFENSE;
extern float PUPPY_ATTACK;
extern const char PUPPY_CHAR;
extern const TCODColor PUPPY_COLOR;
extern int PUPPY_BASE_XP_DROP;

// Dog attributes
extern const char* DOG_NAME;
extern const char* DOG_CORPSE_NAME;
extern float DOG_MAX_HEALTH;
extern float DOG_DEFENSE;
extern float DOG_ATTACK;
extern const char DOG_CHAR;
extern const TCODColor DOG_COLOR;
extern int DOG_BASE_XP_DROP;

// Vacuum attributes
extern const char* VACUUM_NAME;
extern const char* VACUUM_CORPSE_NAME;
extern float VACUUM_MAX_HEALTH;
extern float VACUUM_DEFENSE;
extern float VACUUM_ATTACK;
extern const char VACUUM_CHAR;
extern const TCODColor VACUUM_COLOR;
extern int VACUUM_BASE_XP_DROP;

// Kitty the gray attributes
extern const char* KITTY_THE_GRAY_NAME;
extern const char* KITTY_THE_GRAY_CORPSE_NAME;
extern float KITTY_THE_GRAY_MAX_HEALTH;
extern float KITTY_THE_GRAY_DEFENSE;
extern float KITTY_THE_GRAY_ATTACK;
extern const char KITTY_THE_GRAY_CHAR;
extern const TCODColor KITTY_THE_GRAY_COLOR;
extern int KITTY_THE_GRAY_BASE_XP_DROP;

// How monsters level up with you after every iteration of the levels.
// Starts with base so we can get the correct attributes when we reload the game
extern const float LEVEL_UP_MONSTERS[LAST_MONSTER + 1][NUM_ATTRIBUTES];

// Default symbols and colors for items on map
extern const char POTION_CHAR;
extern const char SCROLL_CHAR;
extern const char STAIRS_CHAR;
extern const TCODColor POTION_COLOR;
extern const TCODColor SCROLL_COLOR;
extern const TCODColor STAIRS_COLOR;

// Names for items
extern const char* HEALER_NAME;
extern const char* LIGHTNING_NAME;
extern const char* FIREBALL_NAME;
extern const char* CONFUSER_NAME;
extern const char* STAIRS_NAME;

// Colors for items in inventory
extern const TCODColor HEALER_COLOR;
extern const TCODColor LIGHTNING_COLOR;
extern const TCODColor FIREBALL_COLOR;
extern const TCODColor CONFUSER_COLOR;

// Numbers associated with items
extern const float HEALER_AMOUNT;
extern const float LIGHTNING_RANGE;
extern const float LIGHTNING_DAMAGE;
extern const float FIREBALL_RANGE;
extern const float FIREBALL_DAMAGE;
extern const float CONFUSER_RANGE;
extern const int CONFUSER_NUM_TURNS;

// Inventory items
extern const bool IS_STACKABLE;
extern const int ONE_ITEM_COUNT;

// How many turns the monster chases the player after losing his sight
extern const int TRACKING_TURNS;

// Player inventory size
extern const int INVENTORY_SIZE;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif	// GAMECONSTANTS_HPP
