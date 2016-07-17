#ifndef OUTPUT_H
#define OUTPUT_H

#include "IncludeLibraries.h"

#define BUFFER_SIZE 50
#define PORT_DELAY 2

using namespace std;

class Output
{
private:
public:
    Output();
    void generate(Graph* graph, ofstream &outFile);
	void parseXml1(ifstream* in, Graph *graph, ofstream& outFile);
};

#endif // OUTPUT_H
