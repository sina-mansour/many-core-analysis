#ifndef PARSER_H
#define PARSER_H

#include "IncludeLibraries.h"

using namespace std;

class Graph;

class Parser
{
private:
    ifstream* inFile;
public:
    Parser();
    Parser(ifstream* in);
    void parseXml(Graph* graph);
};

#endif // PARSER_H
