#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Field.h"
#include <list>
#include <iostream>
#include "Actor.h"
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    Actor* getEdibleAt(int x, int y) const;
    Actor* getPheromoneAt(int x, int y, int colony) const;
    bool isAntHillAt(int x, int y, int colony) const;
    void increaseScore(int colony);
    void poisonAllPoisonableAt(int x, int y);
    void stunAllStunnableAt(int x, int y);
    bool biteEnemyAt(Actor* me, int colony, int biteDamage);
    bool canMoveTo(int x, int y) const;
    bool isDangerAt(int x, int y, int colony) const;
    bool isEnemyAt(int x, int y, int colony) const;
    void add(Actor* ac, int x, int y);
    void setDisplayText();
private:
    int m_ticks;
    int m_winner_so_far;
    std::vector<int> m_score;
    std::vector<string> m_antnames;
    std::list<Actor*> a[64][64];
};

#endif // STUDENTWORLD_H_
