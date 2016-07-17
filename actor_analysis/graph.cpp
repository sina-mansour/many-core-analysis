#include "graph.h"

Graph::Graph()
{
    name = "none";
}

Graph::Graph(string n)
{
    name = n;
}

void Graph::setGraphName(string n)
{
    name = n;
}

Actor *Graph::getActor(int id)
{
    if (actors.size() != 0)
    {
        for (int i=0; i<actors.size(); i++)
        {
            if ((actors[i])->getId() == id)
            {
                return actors[i];
            }
        }
    }
    return NULL;
}

Actor *Graph::getIthActor(int index)
{
    if (actors.size() > index)
    {
        return actors[index];
    }
    return NULL;
}

Actor *Graph::getActor(string name)
{
    if (actors.size() != 0)
    {
        for (int i=0; i<actors.size(); i++)
        {
            if (((actors[i])->getName()).compare(name) == 0)
            {
                return actors[i];
            }
        }
    }
    return NULL;
}

void Graph::addActor(string n, int i)
{
    Actor* temp;
    temp = new Actor(n, i);
    actors.push_back(temp);
}

Channel *Graph::getChannel(int id)
{
    if (channels.size() != 0)
    {
        for (int i=0; i<channels.size(); i++)
        {
            if ((channels[i])->getId() == id)
            {
                return channels[i];
            }
        }
    }
    return NULL;
}

Channel *Graph::getIthChannel(int index)
{
    if (channels.size() > index)
    {
        return channels[index];
    }
    return NULL;
}

Channel *Graph::getChannel(string name)
{
    if (channels.size() != 0)
    {
        for (int i=0; i<channels.size(); i++)
        {
            if (((channels[i])->getName()).compare(name) == 0)
            {
                return channels[i];
            }
        }
    }
    return NULL;
}

void Graph::addChannel(string n, Actor* sA, Actor* dA, Port* sP, Port* dP, int iT, int i)
{
    Channel* temp;
    temp = new Channel(n, sA, dA, sP, dP, iT, i);
    channels.push_back(temp);
}

int Graph::getChannelCount()
{
    return channels.size();
}

int Graph::getActorCount()
{
    return actors.size();
}
