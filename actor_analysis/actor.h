#ifndef ACTOR_H
#define ACTOR_H

#include "IncludeLibraries.h"

using namespace std;

class Port; //forward declaration

class Actor
{
private:
    string name;
    int id, execTime;
    vector <Port*> ports;

public:
    Actor();
    Actor(string n, int i);
    string getName();
    int getId();
    int getPortCount();
    int getExecTime();
    Port* getPort(int id);
    Port* getIthPort(int index);
    Port* getPort(string name);
    Port* getPidPort(string pid);
    void addPort(string t, string n, string p, int r, double s, int i, Actor* o);
    void setExecTime(int eTime);
    void scoreSort();
    void valueSort();
    bool sendsTo(Actor* other);
    bool recievesFrom(Actor* other);
    bool isStart();
    bool isFinish();
};

#endif // ACTOR_H
