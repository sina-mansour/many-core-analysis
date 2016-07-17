#include "port.h"

Port::Port()
{
    type = "none";
    name = "none";
    pid = "none";
    rate = 0;
    score = 0;
    id = -1;
    initial = 0;
    ownerActor = NULL;
    otherEnd = NULL;
    channel = NULL;
}

Port::Port(string t, string n, string p, int r, double s, int i, Actor *a, Actor *o)
{
    type = t;
    name = n;
    pid = p;
    rate = r;
    score = s;
    id = i;
    initial = 0;
    ownerActor = a;
    otherEnd = o;
    channel = NULL;
}

Actor *Port::getOwner()
{
    return ownerActor;
}

Actor *Port::getOtherEnd()
{
    return otherEnd;
}

int Port::getRate()
{
    return rate;
}

double Port::getScore()
{
    return score;
}

double Port::getValue()
{
    return value;
}

int Port::getId()
{
    return id;
}

int Port::getInitial()
{
    return initial;
}

string Port::getType()
{
    return type;
}

string Port::getName()
{
    return name;
}

string Port::getPid()
{
    return pid;
}

Channel *Port::getChannel()
{
    return channel;
}

void Port::setOtherEnd(Actor *a)
{
    otherEnd = a;
}

void Port::setScore(double s)
{
    score = s;
}

void Port::setValue(double v)
{
    value = v;
}

void Port::setInitial(int i)
{
    initial = i;
}

void Port::setName(string n)
{
    name = n;
}

void Port::setPid(string p)
{
    pid = p;
}

void Port::setChannel(Channel *c)
{
    channel = c;
}
