#ifndef GRAPH_H
#define GRAPH_H

#include "IncludeLibraries.h"

using namespace std;

class Actor;
class Channel;
class Port;

class Graph
{
private:
    string name;
    vector <Actor*> actors;
    vector <Channel*> channels;
public:
    Graph();
    Graph(string n);
    void setGraphName(string n);
    Actor* getActor(int id);
    Actor* getIthActor(int index);
    Actor* getActor(string name);
    void addActor(string n, int i);
    Channel* getChannel(int id);
    Channel* getIthChannel(int index);
    Channel* getChannel(string name);
    void addChannel(string n, Actor *sA, Actor *dA, Port *sP, Port *dP, int iT, int i);
    int getChannelCount();
    int getActorCount();
};

#endif // GRAPH_H
