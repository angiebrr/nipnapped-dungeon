#ifndef AI_HPP
#define	AI_HPP

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ===================================================================================================================================
// AI.hpp
// -----------------------------------------------------------------------------------------------------------------------------------
// Header for a class that helps define the behavior of an object.
// -----------------------------------------------------------------------------------------------------------------------------------
// Angela Gross
// NipNapped Dungeon
// ===================================================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: AI
class AI : IPersistent
{
    public:
        virtual void update(Actor* owner) = 0;
        static AI* create (TCODZip& zip);
        virtual void load(TCODZip& zip) = 0;
        virtual void save(TCODZip& zip) = 0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: MONSTERAI
class MonsterAI: public AI 
{
    public:
        void update(Actor* owner);
        void load(TCODZip& zip);
        void save(TCODZip& zip);

    protected:
        int moveCount; // Number of turns player has gone out of sight
        
        void moveOrAttack(Actor* owner, int targetx, int targety);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: PLAYERAI
class PlayerAI: public AI 
{
    public:
        int xpLevel;
        
        PlayerAI();
        void update(Actor* owner);
        int getNextLevelXP();
        void load(TCODZip& zip);
        void save(TCODZip& zip);

    protected:
        bool moveOrAttack(Actor* owner, int targetx, int targety);
        void handleActionKey(Actor* owner, int code);
        Actor* chooseFromInventory(Actor* owner);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CLASS: CONFUSEDMONSTERAI
class ConfusedMonsterAI : public AI 
{
    public:
        ConfusedMonsterAI(int numTurns, AI* oldAI);
        void update(Actor* owner);
        void load(TCODZip& zip);
        void save(TCODZip& zip);

    protected:
        int numTurns;
        AI* oldAI;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif	// AI_HPP

 