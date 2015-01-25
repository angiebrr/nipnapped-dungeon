#ifndef BSPLISTENER_HPP
#define	BSPLISTENER_HPP

class Map;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// BspListener.hpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Utility class for the game's map to create/dig out rooms using a binary space partition tree.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

class BspListener : public ITCODBspCallback 
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    private:
        Map& map; // A map to dig
        int roomNum; // Room number (so we know where to put the player)
        int lastx, lasty; // Center of the last room
        
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    public:
        BspListener(Map& map) : map(map), roomNum(0) {}
        
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // ===========================================================================================================================
        // VISITNODE()
        // ---------------------------------------------------------------------------------------------------------------------------
        // Using the binary space partition tree and random number generator, divide and dig out the rooms from the leaves.
        // ===========================================================================================================================
        bool visitNode(TCODBsp* node, void* userData) 
        {
            // Only creating rooms with leafs from the tree
            if ( node->isLeaf() ) 
            {    
                int w, h, x, y;
                
                // Get a random number generator
                TCODRandom* myRand = TCODRandom::getInstance();

                // Get a random number for the width, height, x and y coordinates from the divisions made
                w = myRand->getInt(Map::ROOM_MIN_SIZE, node->w - 2);
                h = myRand->getInt(Map::ROOM_MIN_SIZE, node->h - 2);
                x = myRand->getInt(node->x + 1, node->x + node->w - w - 1);
                y = myRand->getInt(node->y + 1, node->y + node->h - h - 1);

                // Create the first room for the player to start in
                map.createRoom(roomNum == 0, x, y, x + w - 1, y + h - 1);

                // Dig a corridor from the last room (Shouldn't do it for first room.)
                if (roomNum != 0) 
                {
                    map.dig(lastx, lasty, x + w/2, lasty);
                    map.dig(x + w/2, lasty, x + w/2, y + h/2);
                }
                
                // Center of the room
                lastx = x + w/2;
                lasty = y + h/2;
                
                roomNum++;
            }

            return true;
        }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif	// BSPLISTENER_H

