#include "channel.h"

Channel::Channel()
{
    name = "none";
    srcActor = NULL;
    dstActor = NULL;
    srcPort = NULL;
    dstPort = NULL;
    initialToken = 0;
    id = -1;
}

Channel::Channel(string n, Actor *sA, Actor *dA, Port *sP, Port *dP, int iT, int i)
{
    name = n;
    srcActor = sA;
    dstActor = dA;
    srcPort = sP;
    dstPort = dP;
    initialToken = iT;
    id =i;
}

int Channel::getInitialToken()
{
    return initialToken;
}

int Channel::getId()
{
    return id;
}

string Channel::getName()
{
    return name;
}

Actor *Channel::getSrcActor()
{
    return srcActor;
}

Actor *Channel::getDstActor()
{
    return dstActor;
}

Port *Channel::getSrcPort()
{
    return srcPort;
}

Port *Channel::getDstPort()
{
    return dstPort;
}
