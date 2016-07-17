#ifndef PORT_H
#define PORT_H

#include "IncludeLibraries.h"

using namespace std;

class Actor; //forward declaration
class Channel;

class Port
{
private:
    string type, name, pid;
    int rate, id, initial;
    double score, value;
    Actor* ownerActor;
    Actor* otherEnd;
    Channel* channel;

public:
    Port();
    Port(string t, string n, string p, int r, double s, int i, Actor* a, Actor* o);
    Actor* getOwner();
    Actor* getOtherEnd();
    int getRate();
    double getScore();
    double getValue();
    int getId();
    int getInitial();
    string getType();
    string getName();
    string getPid();
    Channel* getChannel();
    void setOtherEnd(Actor* a);
    void setScore(double s);
    void setValue(double v);
    void setInitial(int i);
    void setName(string n);
    void setPid(string p);
    void setChannel(Channel* c);
};

#endif // PORT_H
