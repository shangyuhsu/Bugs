#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//ACTOR STUFF
//
//
Actor::Actor(StudentWorld* s, int imageID, int x, int y, unsigned int depth, Direction dir)
:GraphObject::GraphObject(imageID, x, y, dir, depth)
{
    m_world = s;
    m_moved= false;
}

Actor::~Actor()
{

}

StudentWorld* Actor::getStudentWorld() const
{
    return m_world;
}

bool Actor::moved() const
{
    return m_moved;
}

void Actor::setMoved(bool b)
{
    m_moved = b;
}

bool Actor::blocksMovement() const
{
    return false;
}

void Actor::getBitten(int amount)
{
}

void Actor::getPoisoned()
{
}

void Actor::getStunned()
{
}

bool Actor::isEdible() const
{
    return false;
}

bool Actor::isPheromone(int colony) const
{
    return false;
}

bool Actor::isAntHill(int colony) const
{
    return false;
}

bool Actor::isEnemy(int colony) const
{
    return false;
}

bool Actor::isDead() const
{
    return false;
}

bool Actor::isDangerous(int colony) const
{
    return false;
}


//PEBBLE STUFF
//
//
Pebble::Pebble(StudentWorld* s, int x, int y): Actor(s, IID_ROCK, x, y, 1)
{
}

void Pebble::doSomething()
{
}

bool Pebble::blocksMovement() const
{
    return true;
}


//ENERGYHOLDER STUFF
//
//
EnergyHolder::EnergyHolder(StudentWorld* s, int imageID, int startX, int startY, int energy, unsigned int depth, Direction dir)
:Actor(s, imageID, startX, startY, depth, dir)
{
    m_energy=energy;
}

EnergyHolder::~EnergyHolder()
{
    
}

void EnergyHolder::doSomething()
{
    m_energy--;
}

bool EnergyHolder::isDead() const
{
    return m_energy<=0;
}

int EnergyHolder::getEnergy() const
{
    return m_energy;
}

void EnergyHolder::updateEnergy(int amount)
{
    m_energy+=amount;
}

bool EnergyHolder::becomesFoodUponDeath() const
{
    return false;
}

int EnergyHolder::pickupAndEatFood(int amount)
{
    
    Food* f = dynamic_cast<Food*>(getStudentWorld()->getEdibleAt(getX(),getY()));
    if (f)
    {
        
        int a = std::min(amount,f->getEnergy());
        f->updateEnergy(-a);
        updateEnergy(a);
        return a;
    }
    return 0;
}


//FOOD STUFF
//
//
Food::Food(StudentWorld* s, int x, int y, int units): EnergyHolder(s, IID_FOOD, x, y, units, 2)
{
}

void Food::doSomething()
{
}

bool Food::isEdible() const
{
    return true;
}



//PHEROMONE
//
//
Pheromone::Pheromone(StudentWorld* s, int colony, int x, int y): EnergyHolder(s, 11+colony, x, y, 256, 1)
{
    m_colony=colony;
}

void Pheromone::increaseStrength(){
    updateEnergy((std::min(768-getEnergy(), 256)));
}

bool Pheromone::isPheromone(int colony) const
{
    return m_colony==colony;
}



//TRIGGERABLEACTOR STUFF
//
//
TriggerableActor::TriggerableActor(StudentWorld* s, int imageID, int x, int y): Actor(s, imageID, x, y, 2)
{
}

TriggerableActor::~TriggerableActor()
{
    
}

bool TriggerableActor::isDangerous(int colony) const
{
    return true;
}



//POISON STUFF
//
//
Poison::Poison(StudentWorld* s, int x, int y): TriggerableActor(s, IID_POISON, x, y)
{
}

void Poison::doSomething()
{
    getStudentWorld()->poisonAllPoisonableAt(getX(), getY());
}


//POOL STUFF
//
//
Pool::Pool(StudentWorld* s, int x, int y): TriggerableActor(s, IID_WATER_POOL, x, y)
{
}

void Pool::doSomething()
{
    getStudentWorld()->stunAllStunnableAt(getX(), getY());
}



//ANTHILL STUFF
AntHill::AntHill(StudentWorld* s, int x, int y, int colony, Compiler* c)
: EnergyHolder(s, IID_ANT_HILL, x, y, 8999, 2)
{
    m_c = c;
    m_colony=colony;
}

void AntHill::doSomething()
{
    
    EnergyHolder::doSomething();
    if(isDead())
        return;
    pickupAndEatFood(10000);

    if (getEnergy()>=2000)
    {
        giveBirth();
    }
}

void AntHill::giveBirth()
{
    Ant* a = new Ant(getStudentWorld(), m_c, m_colony,getX(),getY());
    getStudentWorld()->add(a, getX(), getY());
    updateEnergy(-1500);
    getStudentWorld()->increaseScore(m_colony);
}

bool AntHill::isAntHill(int colony) const
{
    
    return m_colony==colony;
}


//INSECT STUFF
//
//
Insect::Insect(StudentWorld* s, int imageID, int x, int y, int energy): EnergyHolder(s, imageID, x, y, energy, 1)
{
    m_sleep=0;
    m_triggered=false;
}

Insect::~Insect()
{
    
}

bool Insect::isEnemy(int colony) const
{
    return true;
}

bool Insect::isDangerous(int colony) const
{
    return true;
}

int Insect::getSleep() const
{
    return m_sleep;
}

void Insect::incrementSleepBy(int turns)
{
    m_sleep+=turns;
}

void Insect::inFront(int& x, int& y) const
{
    GraphObject::Direction d = getDirection();
    
    switch(d)
    {
        case GraphObject::up:
            x=getX();
            y=getY()+1;
            break;
        case GraphObject::down:
            x=getX();
            y=getY()-1;
            break;
        case GraphObject::left:
            x=getX()-1;
            y=getY();
            break;
        case GraphObject::right:
            x=getX()+1;
            y=getY();
            break;
    }
}

bool Insect::moveInDirection()
{
    int x,y;
    inFront(x, y);
    
    if(!getStudentWorld()->canMoveTo(x,y))
        return false;
    else
    {
        moveTo(x,y);
        setMoved(true);
        m_triggered=false;
        return true;
    }
}

void Insect::getBitten(int amount)
{
    
    updateEnergy(-amount);
}

void Insect::getPoisoned()
{
    if(!m_triggered)
    {
        updateEnergy(-150);
        m_triggered=true;
    }
}

void Insect::getStunned()
{
    if(!m_triggered)
    {
        incrementSleepBy(2);
        m_triggered=true;
    }
}

void Insect::setTriggerStatus(bool b)
{
    m_triggered=b;
}

bool Insect::becomesFoodUponDeath() const
{
    return true;
}



//ANT STUFF
//
//
Ant::Ant(StudentWorld* s, Compiler* c, int colony, int x, int y): Insect(s, colony, x, y, 1500)
{
    m_colony = colony;
    m_c = c;
    m_prev_bitten = false;
    m_prev_blocked = false;
    m_last_rand = 0;
    m_counter = 0;
    m_held_food= 0;
}

bool Ant::isEnemy(int colony) const
{
    return m_colony != colony;
}

bool Ant::isDangerous(int colony) const
{
    return m_colony != colony;
}

void Ant::addFood(int amount)
{
    Food* f = dynamic_cast<Food*>(getStudentWorld()->getEdibleAt(getX(),getY()));
    if (f)
        f->updateEnergy(amount);
    else
        getStudentWorld()->add(new Food(getStudentWorld(),getX(),getY(),amount), getX(), getY());
}

int Ant::pickupFood(int amount)
{
    
    Food* f = dynamic_cast<Food*>(getStudentWorld()->getEdibleAt(getX(),getY()));
    if (f)
    {
        int a = std::min(amount,f->getEnergy());
        f->updateEnergy(-a);
        m_held_food+=a;
        return a;
    }
    return 0;
}

bool Ant::conditionTriggered(Compiler::Command cmd) const
{
    
    int op = stoi(cmd.operand1);
    switch(Compiler::Condition(op))
    {
        case Compiler::last_random_number_was_zero:
            return (m_last_rand==0);
        case Compiler::i_am_carrying_food:
            return (m_held_food>0);
        case Compiler::i_am_hungry:
            return (m_held_food<=25);
        case Compiler::i_am_standing_with_an_enemy:
            
            return getStudentWorld()->isEnemyAt(getX(), getY(), m_colony);
        case Compiler::i_am_standing_on_food:
            if(getStudentWorld()->getEdibleAt(getX(), getY()))
                return true;
            else
                return false;
        case Compiler::i_smell_pheromone_in_front_of_me:
        {
            int x,y;
            inFront(x, y);
            
            if(getStudentWorld()->getPheromoneAt(x, y, m_colony))
                return true;
            else
                return false;
        }
        case Compiler::i_am_standing_on_my_anthill:
            
            return getStudentWorld()->isAntHillAt(getX(), getY(), m_colony);
        case Compiler::i_smell_danger_in_front_of_me:
        {
            int x,y;
            inFront(x, y);
            return getStudentWorld()->isDangerAt(x, y, m_colony);
        }
        case Compiler::i_was_bit:
            return m_prev_bitten;
        case Compiler::i_was_blocked_from_moving:
            return m_prev_blocked;
        case Compiler::invalid_if:
            return false;
    }
}


bool Ant::runCommand(Compiler::Command cmd)
{
    
    switch (cmd.opcode) {
        case Compiler::moveForward:
            if (moveInDirection()==false)
                m_prev_blocked=true;
            else
            {
                m_prev_blocked=false;
                m_prev_bitten=false;
            }
            m_counter++;
            return true;
        case Compiler::eatFood:
            if(m_held_food<100)
            {
                updateEnergy(m_held_food);
                m_held_food=0;
            }
            else
            {
                updateEnergy(100);
                m_held_food-=100;
            }
            m_counter++;
            return true;
        case Compiler::dropFood:
            
            addFood(m_held_food);
            m_held_food=0;
            m_counter++;
            return true;
        case Compiler::bite:
            
            getStudentWorld()->biteEnemyAt(this, m_colony, 15);
            m_counter++;
            return true;
        case Compiler::pickupFood:
        {
            pickupFood(400);
            m_counter++;
            return true;
        }
        case Compiler::emitPheromone:
        {
            Pheromone* c = dynamic_cast<Pheromone*>(getStudentWorld()->getPheromoneAt(getX(), getY(), m_colony));
            if (c)
            {
                c->increaseStrength();
            }
            else
                getStudentWorld()->add(new Pheromone(getStudentWorld(), m_colony, getX(),getY()), getX(), getY());
            m_counter++;
            return true;
        }
        case Compiler::faceRandomDirection:
            setDirection(GraphObject::Direction(randInt(1,4)));
            m_counter++;
            return true;
        case Compiler::rotateClockwise:
        {
            Direction d=getDirection();
            if(d==GraphObject::Direction(1))
            {
                setDirection(GraphObject::Direction(2));
            }
            else if(d==GraphObject::Direction(2))
            {
                setDirection(GraphObject::Direction(3));
            }
            else if(d==GraphObject::Direction(3))
            {
                setDirection(GraphObject::Direction(4));
            }
            else if(d==GraphObject::Direction(4))
            {
                setDirection(GraphObject::Direction(1));
            }
            return true;
        }
        case Compiler::rotateCounterClockwise:
        {
             Direction d=getDirection();
            if(d==GraphObject::Direction(1))
            {
                setDirection(GraphObject::Direction(4));
            }
            else if(d==GraphObject::Direction(2))
            {
                setDirection(GraphObject::Direction(1));
            }
            else if(d==GraphObject::Direction(3))
            {
                setDirection(GraphObject::Direction(2));
            }
            else if(d==GraphObject::Direction(4))
            {
                setDirection(GraphObject::Direction(3));
            }
            return true;
        }
        case Compiler::generateRandomNumber:
        {
            int i = stoi(cmd.operand1);
            if(i==0)
                m_last_rand=i;
            else
                m_last_rand=randInt(0, i-1);
            m_counter++;
            break;
        }
        case Compiler::if_command:
            if (conditionTriggered(cmd)){
                
                m_counter = stoi(cmd.operand2);
            }
            else
                m_counter++;
            break;
        case Compiler::goto_command:
            m_counter = stoi(cmd.operand1);
            break;
        default:
            break;
    }
    return false;
}

void Ant::doSomething()
{
    Insect::doSomething();
    if (isDead())
        return;
    if (getSleep()>0)
    {
        incrementSleepBy(-1);
        return;
    }
    
    Compiler::Command cmd;
    for(int i=0; i<10;i++)
    {
        if (!m_c->getCommand(m_counter, cmd))
        {
            updateEnergy(-getEnergy());
            return;
        }
        if(runCommand(cmd))
            return;
    }
}

void Ant::getBitten(int amount)
{
    Insect::getBitten(amount);
    m_prev_bitten=true;
}



//GRASSHOPPER
//
//
GrassHopper::GrassHopper(StudentWorld* s, int imageID, int x, int y, int energy): Insect(s, imageID, x, y, energy)
{
    m_distance = randInt(2,10);
}

GrassHopper::~GrassHopper()
{
    
}

void GrassHopper::doSomething()
{
    
    Insect::doSomething();
    if(isDead())
        return;
    
    if (getSleep()>0)
    {
        incrementSleepBy(-1);
        return;
    }
    
    incrementSleepBy(2);
    
    if(doMore())
        return;

    int f = pickupAndEatFood(200);
    if(f>0 && randInt(1, 2)==2)
        return;
    
    if(m_distance==0)
    {
        m_distance = randInt(2,10);
        setDirection(GraphObject::Direction(randInt(1,4)));
    }
    
    if(!moveInDirection())
    {
        
        m_distance=0;
    }
    else
    {
        
        m_distance--;
    }
}



//BABYGRASSHOPPER STUFF
//
//
BabyGrassHopper::BabyGrassHopper(StudentWorld* s, int x, int y): GrassHopper(s, IID_BABY_GRASSHOPPER, x, y, 500)
{
}

bool BabyGrassHopper::doMore()
{
    
    if (getEnergy()>=1600)
    {
        getStudentWorld()->add(new AdultGrassHopper(getStudentWorld(), getX(), getY()),getX(),getY());
        updateEnergy(-getEnergy());
        return true;
    }
    return false;
}



//ADULTGRASSHOPPER STUFF
AdultGrassHopper::AdultGrassHopper(StudentWorld* s, int x, int y)
:GrassHopper(s, IID_ADULT_GRASSHOPPER, x, y, 1600)
{
}

bool AdultGrassHopper::doMore()
{
   if(randInt(1,3)==1)
    {
        if (getStudentWorld()->biteEnemyAt(this, -1, 50))
        {
            
            return true;
        }
    }
    
    if(randInt(1,10)==1)
    {
       if(jump())
          return true;
    }
    return false;
}

void AdultGrassHopper::getPoisoned()
{
}

void AdultGrassHopper::getStunned()
{
}

bool AdultGrassHopper::jump()
{
 
    std::vector<int> x;
    std::vector<int> y;
    
    for(int i=-10; i<=10;i++)
    {
        for(int j=-10; j<=10; j++)
        {
            if(j==0&&i==0)
            {
                
            }
            else if((i*i+j*j)<=100 && getStudentWorld()->canMoveTo(getX()+i, getY()+j))
            {
                x.push_back(i);
                y.push_back(j);
            }
        }
    }
    
    if (x.empty())
        return false;
    int r = randInt(0, x.size()-1);
    moveTo(getX()+x[r],getY()+y[r]);
    setMoved(true);
    setTriggerStatus(false);
    return true;
}

void AdultGrassHopper::getBitten(int amount)
{
    
    Insect::getBitten(amount);
    if (!isDead() && randInt(1,2)==2)
        getStudentWorld()->biteEnemyAt(this, -1, 50);
}







