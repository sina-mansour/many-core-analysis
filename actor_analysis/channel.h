#ifndef CHANNEL_H
#define CHANNEL_H

#include "IncludeLibraries.h"

using namespace std;

class Port;

class Channel
{
private:
    int initialToken, id;
    string name;
    Actor* srcActor;
    Actor* dstActor;
    Port* srcPort;
    Port* dstPort;
public:
    Channel();
    Channel(string n, Actor* sA, Actor* dA, Port* sP, Port* dP, int iT, int i);
    int getInitialToken();
    int getId();
    string getName();
    Actor* getSrcActor();
    Actor* getDstActor();
    Port* getSrcPort();
    Port* getDstPort();
};

#endif // CHANNEL_H
