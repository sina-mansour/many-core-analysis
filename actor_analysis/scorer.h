#ifndef SCORER_H
#define SCORER_H

#include "IncludeLibraries.h"

using namespace std;

class Scorer
{
private:
    int consUD, prodUD;
public:
    Scorer();
    Scorer(string mode);
    void simpleGraphScore(Graph* graph);
    void simpleActorScore(Actor* actor);
    void simplePortScore(Port* port);
    void randomGraphScore(Graph* graph);
    void randomActorScore(Actor* actor);
    void randomPortScore(Port* port);
    void combinedGraphScore(Graph* graph);
    void matrixGraphScore(Graph* graph);
    double calculateDeterminant(double **matrix, const int n);
    double calculateValueResult(double** inverse,double** matrix, double det, int node, int point, int index, int matSize);
    double calculateCombinedValueResult(double** inverse,double** matrix, double det, int node, int point, int index, int matSize);
    double calculateCofactorDeterminant(double** matrix, const int n, int row, int column);
};

#endif // SCORER_H
