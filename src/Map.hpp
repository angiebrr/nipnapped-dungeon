#ifndef MAP_HPP
#define	MAP_HPP

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// Map.hpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Header for a class and struct that helps define a given map and its coloring for a level.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// STRUCT: TILE

struct Tile 
{
    bool explored;
    Tile() : explored(false) {}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: MAP
 
class Map : IPersistent
{
    public:
        static const int ROOM_MAX_SIZE = 12;
        static const int ROOM_MIN_SIZE = 6;
        static const int MAX_ROOM_MONSTERS = 3;
        static const int MAX_ROOM_ITEMS = 2;
        int width, height;

        Map(int width, int height);
        ~Map();
        void init(bool withActors);
        void render() const;
        void computeFov();
        bool canWalk(int x, int y) const;
        bool isInFov(int x, int y) const;    
        bool isExplored(int x, int y) const;
        bool isWall(int x, int y) const;
        void load(TCODZip& zip);
        void save(TCODZip& zip);

    protected:
        Tile* tiles; // Helps keep track of explored areas
        TCODMap* walkMap; // Helps keep track of wall coordinates and player field of view
        long seed; // Random map seed
        TCODRandom* myRand; // Random generator
        friend class BspListener; // Helps build rooms

        void dig(int x1, int y1, int x2, int y2);
        void createRoom(bool first, int x1, int y1, int x2, int y2, bool withActors);
        void addMonster(int x, int y);
        void addItem(int x, int y);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif  // MAP_HPP

