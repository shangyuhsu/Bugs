#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"
#include <list>
class StudentWorld;

class Actor:public GraphObject {
public:
    Actor(StudentWorld* s, int imageID, int x, int y, unsigned int depth = 0, Direction dir = right);
    virtual ~Actor();
    virtual void doSomething()=0;
    StudentWorld* getStudentWorld() const;
    bool moved() const;
    void setMoved(bool b);
    virtual void getBitten(int amount);
    virtual bool blocksMovement() const;
    virtual void getPoisoned();
    virtual bool isEdible() const;
    virtual bool isPheromone(int colony) const;
    virtual void getStunned();
    virtual bool isAntHill(int colony) const;
    virtual bool isEnemy(int colony) const;
    virtual bool isDead() const;
    virtual bool isDangerous(int colony) const;
private:
    StudentWorld* m_world;
    bool m_moved;
};

class Pebble:public Actor{
public:
    Pebble(StudentWorld* s, int x, int y);
    virtual void doSomething();
    virtual bool blocksMovement() const;
};

class EnergyHolder:public Actor{
public:
    EnergyHolder(StudentWorld* s, int imageID, int startX, int startY, int energy, unsigned int depth = 0, Direction dir = right);
    virtual ~EnergyHolder();
    virtual bool isDead() const;
    virtual void doSomething();
    int getEnergy() const;
    void updateEnergy(int amount);
    int pickupAndEatFood(int amt);
    virtual bool becomesFoodUponDeath() const;
private:
    int m_energy;
};

class Food:public EnergyHolder{
public:
    Food(StudentWorld* s, int x, int y, int units);
    virtual void doSomething();
    virtual bool isEdible() const;
};

class Pheromone:public EnergyHolder{
public:
    Pheromone(StudentWorld* s, int colony, int x, int y);
    virtual bool isPheromone(int colony) const;
    void increaseStrength();
private:
    int m_colony;
};

class TriggerableActor:public Actor
{
public:
    TriggerableActor(StudentWorld* s, int imageID, int x, int y);
    virtual ~TriggerableActor();
    virtual bool isDangerous(int colony) const;
};

class Poison:public TriggerableActor{
public:
    Poison(StudentWorld* s, int x, int y);
    virtual void doSomething();
};

class Pool:public TriggerableActor{
public:
    Pool(StudentWorld* s, int x, int y);
    virtual void doSomething();
};

class AntHill:public EnergyHolder{
public:
    AntHill(StudentWorld* s, int x, int y, int colony, Compiler* c);
    virtual void doSomething();
    void giveBirth();
    virtual bool isAntHill(int colony) const;
private:
    Compiler* m_c;
    int m_colony;
};

class Insect:public EnergyHolder{
public:
    Insect(StudentWorld* s, int imageID, int x, int y, int energy);
    virtual ~Insect();
    void inFront(int& x, int& y) const;
    int getSleep() const;
    virtual bool isEnemy(int colony) const;
    bool moveInDirection();
    void incrementSleepBy(int turns);
    virtual bool isDangerous(int colony) const;
    virtual void getPoisoned();
    virtual void getStunned();
    virtual void setTriggerStatus(bool b);
    virtual void getBitten(int amount);
    virtual bool becomesFoodUponDeath() const;
private:
    bool m_triggered;
    int m_sleep;
};

class Ant:public Insect{
public:
    Ant(StudentWorld* s, Compiler* c, int colonyNumber, int x, int y);
    virtual void doSomething();
    bool conditionTriggered(Compiler::Command cmd) const;
    bool runCommand(const Compiler::Command cmd);
    virtual bool isEnemy(int colony) const;
    void addFood(int amount);
    virtual bool isDangerous(int colony) const;
    virtual void getBitten(int amount);
    int pickupFood(int amt);
    
private:
    int m_last_rand;
    bool m_prev_bitten;
    bool m_prev_blocked;
    int m_held_food;
    int m_colony;
    int m_counter;
    Compiler* m_c;
};


class GrassHopper:public Insect{
public:
    GrassHopper(StudentWorld* s, int imageID, int x, int y, int energy);
    virtual ~GrassHopper();
    virtual void doSomething();
    virtual bool doMore()=0;
private:
    int m_distance;
};


class BabyGrassHopper:public GrassHopper{
public:
    BabyGrassHopper(StudentWorld* s, int x, int y);
    virtual bool doMore();
};


class AdultGrassHopper:public GrassHopper{
public:
    AdultGrassHopper(StudentWorld* s, int x, int y);
    virtual void getPoisoned();
    virtual void getStunned();
    virtual void getBitten(int amount);
    virtual bool doMore();
    bool jump();
};


#endif // ACTOR_H_
