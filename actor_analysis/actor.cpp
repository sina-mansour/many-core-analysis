#include "actor.h"

Actor::Actor()
{
    name = "none";
    id = -1;
}

Actor::Actor(string n, int i)
{
    name = n;
    id = i;
}

string Actor::getName()
{
    return name;
}

int Actor::getId()
{
    return id;
}

int Actor::getPortCount()
{
    return ports.size();
}

int Actor::getExecTime()
{
    return execTime;
}

Port *Actor::getPort(int id)
{
    if (ports.size() != 0)
    {
        for (int i=0; i<ports.size(); i++)
        {
            if ((ports[i])->getId() == id)
            {
                return ports[i];
            }
        }
    }
    return NULL;
}

Port *Actor::getIthPort(int index)
{
    if (ports.size() > index)
    {
        return ports[index];
    }
    return NULL;
}

Port *Actor::getPort(string name)
{
    if (ports.size() != 0)
    {
        for (int i=0; i<ports.size(); i++)
        {
            if (((ports[i])->getName()).compare(name) == 0)
            {
                return ports[i];
            }
        }
    }
    return NULL;
}

Port *Actor::getPidPort(string pid)
{
    if (ports.size() != 0)
    {
        for (int i=0; i<ports.size(); i++)
        {
            if (((ports[i])->getPid()).compare(pid) == 0)
            {
                return ports[i];
            }
        }
    }
    return NULL;
}

void Actor::addPort(string t, string n, string p, int r, double s, int i, Actor *o)
{
    Port* temp;
    temp = new Port(t, n, p, r, s, i, this, o);
    ports.push_back(temp);
}

void Actor::setExecTime(int eTime)
{
    execTime = eTime;
}

void Actor::scoreSort()
{
    int portCount;
    portCount = ports.size();

    for (int i=0; i<portCount; i++)
    {
        for (int j=i+1; j<portCount; j++)
        {
            if ((((ports[i])->getType()).compare("out") == 0) &&
                    (((ports[j])->getType()).compare("in") == 0))
            {
                Port* temp;
                temp = ports[i];
                ports[i] = ports[j];
                ports[j] = temp;
            }
        }
    }

    for (int i=0; i<portCount; i++)
    {
        for (int j=i+1; j<portCount; j++)
        {
            if ((((ports[i])->getScore())>((ports[j])->getScore())) &&
                    (((ports[i])->getType()).compare((ports[j])->getType()) == 0))
            {
                Port* temp;
                temp = ports[i];
                ports[i] = ports[j];
                ports[j] = temp;
            }
        }
    }
}

void Actor::valueSort()
{
    int portCount;
    portCount = ports.size();

    for (int i=0; i<portCount; i++)
    {
        for (int j=i+1; j<portCount; j++)
        {
            if ((((ports[i])->getType()).compare("out") == 0) &&
                    (((ports[j])->getType()).compare("in") == 0))
            {
                Port* temp;
                temp = ports[i];
                ports[i] = ports[j];
                ports[j] = temp;
            }
        }
    }

    for (int i=0; i<portCount; i++)
    {
        for (int j=i+1; j<portCount; j++)
        {
            if ((((ports[i])->getValue())<((ports[j])->getValue())) &&
                    (((ports[i])->getType()).compare("in") == 0) &&
                    (((ports[j])->getType()).compare("in") == 0))
            {
                Port* temp;
                temp = ports[i];
                ports[i] = ports[j];
                ports[j] = temp;
            }
        }
    }

    for (int i=0; i<portCount; i++)
    {
        for (int j=i+1; j<portCount; j++)
        {
            if ((((ports[i])->getValue())>((ports[j])->getValue())) &&
                    (((ports[i])->getType()).compare("out") == 0) &&
                    (((ports[j])->getType()).compare("out") == 0))
            {
                Port* temp;
                temp = ports[i];
                ports[i] = ports[j];
                ports[j] = temp;
            }
        }
    }
}

bool Actor::sendsTo(Actor *other)
{
    if (ports.size() != 0)
    {
        for (int i=0; i<ports.size(); i++)
        {
            if ((((ports[i])->getOtherEnd()->getName()).compare(other->getName()) == 0)
                    && (((ports[i])->getType()).compare("out") == 0))
            {
                return true;
            }
        }
    }
    return false;
}

bool Actor::recievesFrom(Actor *other)
{
    if (ports.size() != 0)
    {
        for (int i=0; i<ports.size(); i++)
        {
            if ((((ports[i])->getOtherEnd()->getName()).compare(other->getName()) == 0)
                    && (((ports[i])->getType()).compare("in") == 0))
            {
                return true;
            }
        }
    }
    return false;
}

bool Actor::isStart()
{
    if (ports.size() != 0)
    {
        for (int i=0; i<ports.size(); i++)
        {
            if (((ports[i])->getType()).compare("in") == 0)
            {
                return false;
            }
        }
        return true;
    }
    return true;
}

bool Actor::isFinish()
{
    if (ports.size() != 0)
    {
        for (int i=0; i<ports.size(); i++)
        {
            if (((ports[i])->getType()).compare("out") == 0)
            {
                return false;
            }
        }
        return true;
    }
    return true;
}
