#ifndef ANALYSISCORE_H
#define ANALYSISCORE_H

#include "IncludeLibraries.h"

class Scorer;
class Sorter;
class Graph;

class AnalysisCore
{
private:
    string fileName, outPath, inPath;
    Graph* graph;
    Scorer* scorer;
    Sorter* sorter;

public:
    AnalysisCore(string fName, string inp, string outp);
    ~AnalysisCore();

    bool generateUnsortedOutput(string fName, string inp, string outp);
    bool generateSimpleSortedOutput(string fName, string inp, string outp, string mode);
    bool generateMatrixSortedOutput(string fName, string inp, string outp, string mode);
    bool generateCombinedSortedOutput(string fName, string inp, string outp, string mode);
    bool generateRandomSortedOutput(int randCount, string fName, string inp, string outp);

};

#endif // ANALYSISCORE_H
