#include "StudentWorld.h"
#include <string>
#include <iostream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}


StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{
    m_ticks=0;
    m_winner_so_far=0;
}

Actor* StudentWorld::getEdibleAt(int x, int y) const
{
    for (std::list<Actor*>::const_iterator it=a[y][x].begin(); it != a[y][x].end(); ++it)
    {
        
        Actor* c = *it;
        if(c->isEdible())
            return c;
    }
    return nullptr;
}

Actor* StudentWorld::getPheromoneAt(int x, int y, int colony) const
{
    for (std::list<Actor*>::const_iterator it=a[y][x].begin(); it != a[y][x].end(); ++it)
    {
        Actor* c = *it;
        if(c->isPheromone(colony))
            return c;
    }
    return nullptr;
}

bool StudentWorld::isAntHillAt(int x, int y, int colony) const
{
    for (std::list<Actor*>::const_iterator it=a[y][x].begin(); it != a[y][x].end(); ++it)
    {
        Actor* c = *it;
        if(c->isAntHill(colony))
        {
            
            return true;
        }
    }
    return false;
}

void StudentWorld::increaseScore(int colony)
{
    m_score[colony]++;
}

void StudentWorld::poisonAllPoisonableAt(int x, int y)
{
    for(std::list<Actor*>::iterator it =a[y][x].begin(); it != a[y][x].end(); it++)
    {
        Actor* a = *it;
        a->getPoisoned();
    }
    
}

void StudentWorld::stunAllStunnableAt(int x, int y)
{
    for (std::list<Actor*>::iterator it=a[y][x].begin(); it != a[y][x].end(); ++it)
    {
        Actor* a = *it;
        a->getStunned();
    }
}


bool StudentWorld::biteEnemyAt(Actor* me, int colony, int biteDamage)
{
    std::vector<Actor*> l;
    for(std::list<Actor*>::iterator it =a[me->getY()][me->getX()].begin(); it != a[me->getY()][me->getX()].end(); it++)
    {
        Actor* a = *it;
        if (a!= me && !a->isDead()&&a->isEnemy(colony))
        {
            l.push_back(a);
        }
    }
    if (l.empty())
        return false;
    int i = randInt(0,l.size()-1);
    Actor*  a = l[i];
    a->getBitten(biteDamage);
    return true;
}

bool StudentWorld::canMoveTo(int x, int y) const
{
    if(x<0||x>=64||y<0||y>=64)
        return false;
    for(std::list<Actor*>::const_iterator it=a[y][x].begin(); it != a[y][x].end(); ++it)
    {
        Actor* c = *it;
        if (c->blocksMovement())
            return false;
    }
    return true;
}

bool StudentWorld::isDangerAt(int x, int y, int colony) const
{
    for(std::list<Actor*>::const_iterator it=a[y][x].begin(); it != a[y][x].end(); ++it)
    {
        Actor* c = *it;
        if (c->isDangerous(colony))
            return true;
    }
    return false;
}

bool StudentWorld::isEnemyAt(int x, int y, int colony) const
{
    for(std::list<Actor*>::const_iterator it=a[y][x].begin(); it != a[y][x].end(); ++it)
    {
        Actor* c = *it;
        if (c->isEnemy(colony))
            return true;
    }
    return false;
}

void StudentWorld::add(Actor* ac, int x, int y)
{
    a[y][x].push_back(ac);
}

void StudentWorld::setDisplayText() {
   
    ostringstream s;
    s<<"Ticks:"<<right<<setw(5)<<2000-m_ticks<<" - ";
    for(int i=0;i<m_antnames.size();i++)
    {
        string t;
        if(m_score[i]<10)
            t="0";
        s<<m_antnames[i]<<": "<<t<<m_score[i]<<"  ";
    }
    setGameStatText(s.str());
}

int StudentWorld::init()
{
    Compiler* c[4];
    std::vector<std::string> fileNames = getFilenamesOfAntPrograms();
    int numPrograms = fileNames.size();
    for(int i =0; i<numPrograms; i++)
    {
        c[i] = new Compiler;
        std::string error;
        if (! c[i]->compile(fileNames[i], error))
        {
            setError(fileNames[i] + " " + error);
            return GWSTATUS_LEVEL_ERROR;
        }
        m_antnames.push_back(fileNames[i].substr(0,fileNames[i].size()-4));
        m_score.push_back(0);
    }

    Field f;
    string fieldFileName;
    std::string fieldFile = getFieldFilename();
    
    if (f.loadField(fieldFile) != Field::LoadResult::load_success)
    {
        return false;
    }
    
    for(int i = 0; i<64;i++)
    {
        for(int y = 0; y<64; y++)
        {
            Field::FieldItem item = f.getContentsOf(i,y);
            switch(item)
            {
                case Field::empty:
                    break;
                case Field::anthill0:
                    if(numPrograms>=1)
                        a[y][i].push_back(new AntHill(this,i,y,0,c[0]));
                    break;
                case Field::anthill1:
                    if(numPrograms>=2)
                        a[y][i].push_back(new AntHill(this,i,y,1,c[1]));
                    break;
                case Field::anthill2:
                    if(numPrograms>=3)
                        a[y][i].push_back(new AntHill(this,i,y,2,c[2]));
                    break;
                case Field::anthill3:
                    if(numPrograms>=4)
                        a[y][i].push_back(new AntHill(this,i,y,3,c[3]));
                    break;
                case Field::rock:
                    a[y][i].push_back(new Pebble(this,i,y));
                    break;
                case Field::water:
                    a[y][i].push_back(new Pool(this,i,y));
                    break;
                case Field::poison:
                    a[y][i].push_back(new Poison(this,i,y));
                    break;
                case Field::grasshopper:
                    a[y][i].push_back(new BabyGrassHopper(this,i,y));
                    break;
                case Field::food:
                    a[y][i].push_back(new Food(this,i,y,6000));
                    break;
                default:
                    break;
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_ticks++;
    
    
    for(int i = 0; i < 64; i++)
    {
        for(int y = 0; y < 64; y++)
        {
            for (std::list<Actor*>::iterator it=a[y][i].begin(); it != a[y][i].end(); it++)
            {
                Actor* c = *it;
           
                if (!c->moved())
                {
                    c->doSomething();
                
                    if (c->moved())
                    {
                        it--;
                        a[c->getY()][c->getX()].push_back(c);
                        a[y][i].remove(c);
                    }
                }
            }
        }
    }
    
    for(int i = 0; i < 64; i++)
    {
        for(int y = 0; y < 64; y++)
        {
            for (std::list<Actor*>::iterator it=a[y][i].begin(); it != a[y][i].end(); ++it)
            {
                Actor* c = *it;
                if (c->isDead())
                {
                    EnergyHolder* e =  dynamic_cast<EnergyHolder*>(c);
                    if (e && e->becomesFoodUponDeath())
                        add(new Food(this, i ,y, 100),i,y);
                    --it;
                    a[y][i].remove(c);
                    
                    delete c;
                }
                else
                {
                    c->setMoved(false);
                }
            }
        }
    }
    setDisplayText();
    
    for(int i = 0; i<m_score.size();i++)
    {
        if (m_score[i]>m_score[m_winner_so_far])
            m_winner_so_far=i;
    }
    
    if (m_ticks<2000)
        return GWSTATUS_CONTINUE_GAME;
    else
    {
        if (m_score.size()>0 && m_score[m_winner_so_far]>=5)
        {
            setWinner(m_antnames[m_winner_so_far]);
            return GWSTATUS_PLAYER_WON;
        }

        return GWSTATUS_NO_WINNER;
    }
    

}


void StudentWorld::cleanUp()
{
    for(int i = 0; i < 64; i++)
    {
        for(int y = 0; y < 64; y++)
        {
            for (std::list<Actor*>::iterator it=a[y][i].begin(); it != a[y][i].end(); ++it)
            {
                Actor* c = *it;
                --it;
                a[y][i].remove(c);
                delete c;
            }
        }
    }
}


StudentWorld::~StudentWorld()
{
    cleanUp();
}






