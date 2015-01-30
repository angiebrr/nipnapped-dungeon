#include "main.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// GameConstants.cpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Constants for the game to try to avoid magic numbers.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNappedDungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// GUI CONSTANTS
// ==================================================================================================================================
const int PANEL_HEIGHT = 7;
const int BAR_WIDTH = 20;
const int MSG_X = BAR_WIDTH + 2;
const int MSG_HEIGHT = PANEL_HEIGHT - 1;
const int INVENTORY_WIDTH = 50;
const int INVENTORY_HEIGHT = 28;
const int PAUSE_MENU_WIDTH = 30;
const int PAUSE_MENU_HEIGHT = 15;

int SCREEN_WIDTH = 80;
int SCREEN_HEIGHT = 50;
bool IS_FULLSCREEN = false;
const char* WINDOW_TITLE = "NipNapped Dungeon";

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// LEVEL CONSTANTS
// ==================================================================================================================================

// Map colors
const TCODColor darkWall(0, 0, 100);
const TCODColor darkGround(50, 50, 150);
const TCODColor lightWall(130, 110, 50);
const TCODColor lightGround(200, 180, 50);

// Camera
int FOV_RADIUS = 10;

// Room constants
const int ROOM_MAX_SIZE = 12;
const int ROOM_MIN_SIZE = 6;
const int MAX_ROOM_MONSTERS = 3;
const int MAX_ROOM_ITEMS = 2;
const int RECURSION_DEPTH = 8;
const float MAX_H_RATIO = 1.5f;
const float MAX_V_RATIO = 1.5f;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ==================================================================================================================================
// ACTOR CONSTANTS
// ==================================================================================================================================

// Types of monsters that will spawn in each level
// 5th level will be boss battle with KITTY
const MonsterType LEVEL_MONSTERS[LAST_LEVEL][NUM_MONSTER_SLOTS] =
{ 
    { MOUSE, MOUSE, MOUSE, MOUSE, MOUSE },
    { MOUSE, MOUSE, PUPPY, PUPPY, PUPPY },
    { MOUSE, MOUSE, PUPPY, PUPPY, DOG },
    { MOUSE, PUPPY, DOG, DOG, VACUUM },
    { KITTY_THE_GRAY, KITTY_THE_GRAY, KITTY_THE_GRAY, KITTY_THE_GRAY, KITTY_THE_GRAY }
};

// Bool for boss
const bool IS_BOSS = true;

// Experience-related constants
const int LEVEL_UP_BASE = 200;
const int LEVEL_UP_FACTOR = 150;

// Player attributes
const int PLAYER_DEFAULT_X = 40;
const int PLAYER_DEFAULT_Y = 25;
const char* PLAYER_NAME = "Lucky";
const char* PLAYER_CORPSE_NAME = "Lucky's corpse";
const float PLAYER_MAX_HEALTH = 30;
const float PLAYER_DEFENSE = 0;
const float PLAYER_ATTACK = 4;
const char PLAYER_CHAR = '@';
const TCODColor PLAYER_COLOR = TCODColor::white;
const int PLAYER_BASE_XP_DROP = 0;

// Mouse atrributes
const char* MOUSE_NAME = "infected mouse";
const char* MOUSE_CORPSE_NAME = "perished mouse";
float MOUSE_MAX_HEALTH = 5;
float MOUSE_DEFENSE = 0;
float MOUSE_ATTACK = 1;
const char MOUSE_CHAR = 'm';
const TCODColor MOUSE_COLOR = TCODColor::desaturatedSky;
int MOUSE_BASE_XP_DROP = 50;

// Puppy attributes
const char* PUPPY_NAME = "evil puppy";
const char* PUPPY_CORPSE_NAME = "dead puppy";
float PUPPY_MAX_HEALTH = 7;
float PUPPY_DEFENSE = 0;
float PUPPY_ATTACK = 2.5;
const char PUPPY_CHAR = 'p';
const TCODColor PUPPY_COLOR = TCODColor::violet;
int PUPPY_BASE_XP_DROP = 75;

// Dog attributes
const char* DOG_NAME = "rabid dog";
const char* DOG_CORPSE_NAME = "slain dog";
float DOG_MAX_HEALTH = 10;
float DOG_DEFENSE = 1;
float DOG_ATTACK = 3.5;
const char DOG_CHAR = 'D';
const TCODColor DOG_COLOR = TCODColor::magenta;
int DOG_BASE_XP_DROP = 95;

// Vacuum attributes
const char* VACUUM_NAME = "vicious vacuum";
const char* VACUUM_CORPSE_NAME = "dead vacuum";
float VACUUM_MAX_HEALTH = 15;
float VACUUM_DEFENSE = 2;
float VACUUM_ATTACK = 4;
const char VACUUM_CHAR = 'V';
const TCODColor VACUUM_COLOR = TCODColor::magenta;
int VACUUM_BASE_XP_DROP = 125;

// Kitty the gray attributes
const char* KITTY_THE_GRAY_NAME = "Kitty The Gray";
const char* KITTY_THE_GRAY_CORPSE_NAME = "dead as door nails Kitty The Gray";
float KITTY_THE_GRAY_MAX_HEALTH = 75;
float KITTY_THE_GRAY_DEFENSE = 3;
float KITTY_THE_GRAY_ATTACK = 6;
const char KITTY_THE_GRAY_CHAR = 'K';
const TCODColor KITTY_THE_GRAY_COLOR = TCODColor::crimson;
int KITTY_THE_GRAY_BASE_XP_DROP = 500;

// How monsters level up with you after every iteration of the levels.
// Starts with base so we can get the correct attributes when we reload the game
const float LEVEL_UP_MONSTERS[LAST_MONSTER + 1][NUM_ATTRIBUTES] =
{
    { MOUSE_MAX_HEALTH + 10.0f, MOUSE_ATTACK + 5.0f, MOUSE_DEFENSE + 0.5f, 1.5f * MOUSE_BASE_XP_DROP },
    { PUPPY_MAX_HEALTH + 10.0f, PUPPY_ATTACK + 5.0f, PUPPY_DEFENSE + 0.5f, 1.5f * PUPPY_BASE_XP_DROP },
    { DOG_MAX_HEALTH + 15.0f, DOG_ATTACK + 10.0f, DOG_DEFENSE +  1.0f, 1.5f * DOG_BASE_XP_DROP },
    { VACUUM_MAX_HEALTH + 20.0f, VACUUM_ATTACK + 8.0f, VACUUM_DEFENSE + 1.0f, 1.5 * VACUUM_BASE_XP_DROP },
    { KITTY_THE_GRAY_MAX_HEALTH + 40.0f, KITTY_THE_GRAY_ATTACK + 10.0f, KITTY_THE_GRAY_DEFENSE + 3.0f, 1.5 * KITTY_THE_GRAY_BASE_XP_DROP  }
};

// Default symbols and colors for items on map
const char POTION_CHAR = '!';
const char SCROLL_CHAR = '#';
const char STAIRS_CHAR = '>';
const TCODColor POTION_COLOR = TCODColor::lightGreen;
const TCODColor SCROLL_COLOR = TCODColor::lightYellow;
const TCODColor STAIRS_COLOR = TCODColor::white;

// Names for items
const char* HEALER_NAME = "catnip juice";
const char* LIGHTNING_NAME = "scroll of lightning claws";
const char* FIREBALL_NAME = "scroll of fire hairball";
const char* CONFUSER_NAME = "scroll of confused cats";
const char* STAIRS_NAME = "stairs";

// Colors for items in inventory
const TCODColor HEALER_COLOR = TCODColor::green;
const TCODColor LIGHTNING_COLOR = TCODColor::yellow;
const TCODColor FIREBALL_COLOR = TCODColor::amber;
const TCODColor CONFUSER_COLOR = TCODColor::sky;

// Numbers associated with items
const float HEALER_AMOUNT = 4;
const float LIGHTNING_RANGE = 5;
const float LIGHTNING_DAMAGE = 20;
const float FIREBALL_RANGE = 3;
const float FIREBALL_DAMAGE = 12;
const float CONFUSER_RANGE = 10;
const int CONFUSER_NUM_TURNS = 3;

// Inventory items
const bool IS_STACKABLE = true;
const int ONE_ITEM_COUNT = 1;

// How many turns the monster chases the player after losing his sight
const int TRACKING_TURNS = 3;

// Player inventory size
const int INVENTORY_SIZE = 13;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
