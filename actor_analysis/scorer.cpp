#include "scorer.h"

#define NORMALIZE 1.0e4

// 'a_d' is:
// Ascending sorter for consumption
// Descending sorter for production


Scorer::Scorer()
{
    consUD = 1;
    prodUD = 1;
}

Scorer::Scorer(string mode)
{
    if (mode == "a_d") {
        consUD = 1;
        prodUD = -1;
    }
    if (mode == "a_a") {
        consUD = 1;
        prodUD = 1;
    }
    if (mode == "d_d") {
        consUD = -1;
        prodUD = -1;
    }
    if (mode == "d_a") {
        consUD = -1;
        prodUD = 1;
    }
}

void Scorer::simpleGraphScore(Graph *graph)
{
    Actor* tmpA;
    int actorCount;

    actorCount = graph->getActorCount();

    for (int i=0; i<actorCount; i++)
    {
        tmpA = graph->getIthActor(i);
        simpleActorScore(tmpA);
    }
}

void Scorer::simpleActorScore(Actor *actor)
{
    Port* tmpP;
    int portCount;

    portCount = actor->getPortCount();

    for (int i=0; i<portCount; i++)
    {
        tmpP = actor->getIthPort(i);
        simplePortScore(tmpP);
    }
}

void Scorer::simplePortScore(Port *port)
{
    double score;
    int inRate, outRate;
    int inExec, outExec;
    Channel* tmpC;
    tmpC = port->getChannel();

    inRate = tmpC->getSrcPort()->getRate();
    outRate = tmpC->getDstPort()->getRate();

    inExec = tmpC->getSrcPort()->getOwner()->getExecTime();
    outExec = tmpC->getDstPort()->getOwner()->getExecTime();

    if (port->getType().compare("in") == 0 && outRate!=0)
    {
        score = (double) inRate / outRate * consUD; //consumption score
        // score = (double) (outExec * inRate) / (inExec * outRate) * consUD; // exec devision
        // score = (double) (inExec * inRate) / (outExec * outRate) * consUD; // exec multiply
    }
    else if (port->getType().compare("out") == 0 && inRate!=0)
    {
        score = (double) outRate / inRate * prodUD; //production score
        // score = (double) (inExec * outRate) / (outExec * inRate) * prodUD; // exec devision
        // score = (double) (outExec * outRate) / (inExec * inRate) * prodUD; // exec multiply
    }
    else
    {
        score = 0;
    }

    port->setScore(score);
}

void Scorer::randomGraphScore(Graph *graph)
{
    Actor* tmpA;
    int actorCount;

    actorCount = graph->getActorCount();

    for (int i=0; i<actorCount; i++)
    {
        tmpA = graph->getIthActor(i);
        randomActorScore(tmpA);
    }
}

void Scorer::randomActorScore(Actor *actor)
{
    Port* tmpP;
    int portCount;

    portCount = actor->getPortCount();

    for (int i=0; i<portCount; i++)
    {
        tmpP = actor->getIthPort(i);
        randomPortScore(tmpP);
    }
}

void Scorer::randomPortScore(Port *port)
{
    double score;
    score = (double)rand() / RAND_MAX;
    port->setScore(score);
    port->setValue(score);
}

void Scorer::matrixGraphScore(Graph *graph)
{
    int actorCount;

    actorCount = graph->getActorCount();

    double** inPortsMatrix;
    inPortsMatrix = new double* [actorCount];
    for (int i=0; i<actorCount; i++)
    {
        *(inPortsMatrix + i) = new double [actorCount];
    }

    for (int i=0; i<actorCount; i++)
    {
        for (int j=0; j<actorCount; j++)
        {
            if (graph->getIthActor(i)->recievesFrom(graph->getIthActor(j)))
            {
                double value, outCost, inCost;
                int outCount, inCount;
                inCount = 0;
                outCount = 0;
                value = graph->getIthActor(j)->getExecTime();
                outCost = 0;
                inCost = 0;
                int portsCount = graph->getIthActor(j)->getPortCount();



                for (int k=0; k<portsCount; k++)
                {
                    if ((graph->getIthActor(j)->getIthPort(k)->getType()).compare("in") == 0)
                    {
                        inCount++;
                        inCost += graph->getIthActor(j)->getIthPort(k)->getRate();
                    }
                    else
                    {
                        outCount++;
                        outCost += graph->getIthActor(j)->getIthPort(k)->getRate();
                    }
                }

                if (inCount != 0)
                {
                    inCost /= inCount;
                }

                if (outCount != 0)
                {
                    outCost /= outCount;
                }

                value += inCost + outCost;

                value *= -1;

                value = exp(value/NORMALIZE);

                inPortsMatrix[i][j] = value;
            }
            else
            {
                inPortsMatrix[i][j] = 0;
                //*(*(inPortsMatrix + i) + j) = 0;
            }

        }
    }

    // M matrix produced, now turning to (I - M) matrix:

    double** inverseInPortsMatrix;
    inverseInPortsMatrix = new double* [actorCount];
    for (int i=0; i<actorCount; i++)
    {
        *(inverseInPortsMatrix + i) = new double [actorCount];
    }

    for (int i=0; i<actorCount; i++)
    {
        for (int j=0; j<actorCount; j++)
        {
            inverseInPortsMatrix[i][j] =  -inPortsMatrix[i][j];
            if (i == j)
            {
                inverseInPortsMatrix[i][i] ++;
            }
        }
    }

    double** outPortsMatrix;
    outPortsMatrix = new double* [actorCount];
    for (int i=0; i<actorCount; i++)
    {
        *(outPortsMatrix + i) = new double [actorCount];
    }

    for (int i=0; i<actorCount; i++)
    {
        for (int j=0; j<actorCount; j++)
        {
            if (graph->getIthActor(i)->sendsTo(graph->getIthActor(j)))
            {
                double value, outCost, inCost;
                int outCount, inCount;
                inCount = 0;
                outCount = 0;
                value = graph->getIthActor(j)->getExecTime();
                outCost = 0;
                inCost = 0;
                int portsCount = graph->getIthActor(j)->getPortCount();

                for (int k=0; k<portsCount; k++)
                {
                    if ((graph->getIthActor(j)->getIthPort(k)->getType()).compare("in") == 0)
                    {
                        inCount++;
                        inCost += graph->getIthActor(j)->getIthPort(k)->getRate();
                    }
                    else
                    {
                        outCount++;
                        outCost += graph->getIthActor(j)->getIthPort(k)->getRate();
                    }
                }

                if (inCount != 0)
                {
                    inCost /= inCount;
                }

                if (outCount != 0)
                {
                    outCost /= outCount;
                }

                value += inCost + outCost;

                value *= -1;

                value = exp(value/NORMALIZE);

                outPortsMatrix[i][j] = value;
            }
            else
            {
                outPortsMatrix[i][j] = 0;
                //*(*(outPortsMatrix + i) + j) = 0;
            }
        }
    }

    // M matrix produced, now turning to (I - M) matrix:

    double** inverseOutPortsMatrix;
    inverseOutPortsMatrix = new double* [actorCount];
    for (int i=0; i<actorCount; i++)
    {
        *(inverseOutPortsMatrix + i) = new double [actorCount];
    }
    for (int i=0; i<actorCount; i++)
    {
        for (int j=0; j<actorCount; j++)
        {
            inverseOutPortsMatrix[i][j] = -outPortsMatrix[i][j];
            if (i == j)
            {
                inverseOutPortsMatrix[i][i] ++;
            }
        }
    }

    //calculate determinants and create inverse matrix

    double inMatrixDet, outMatrixDet;
    inMatrixDet = calculateDeterminant(inverseInPortsMatrix,actorCount);
    outMatrixDet = calculateDeterminant(inverseOutPortsMatrix,actorCount);

//    set start and finish node

    // startNode = graph->getIthActor(startNodeIndex);
    // finishNode = graph->getIthActor(finishNodeIndex);


    for (int i=0; i<actorCount; i++)
    {
        Actor* tmpA;
        tmpA = graph->getIthActor(i);
        int portCount = tmpA->getPortCount();

        for (int portId=0; portId<portCount; portId++)
        {
            Actor* startNode = NULL;
            Actor* finishNode = NULL;
            int startNodeIndex = 0;
            int finishNodeIndex = graph->getActorCount()-1;

            if (tmpA->getIthPort(portId)->getType().compare("in")==0)
            {
                Port* inPort = tmpA->getIthPort(portId);
                Actor* otherEnd = inPort->getOtherEnd();
                int otherEndIndex = 0;
                for (int j=0; j<actorCount; j++)
                {
                    if (otherEnd == graph->getIthActor(j))
                    {
                        otherEndIndex = j;
                        break;
                    }
                }
        double valueResult = 0 ;
        for (int j=0; j<actorCount; j++)
        {
           Actor* tmpB;
           tmpB = graph->getIthActor(j);
           if (tmpB->isStart())
           {
               startNode = tmpB;
               startNodeIndex = j;
            valueResult += calculateValueResult(inverseInPortsMatrix,inPortsMatrix,inMatrixDet,otherEndIndex,startNodeIndex,i,actorCount);
          }
           if(j==actorCount-1 && startNode == NULL)
           {
            startNodeIndex = 0;
            startNode = graph->getIthActor(startNodeIndex);
            valueResult = calculateValueResult(inverseInPortsMatrix,inPortsMatrix,inMatrixDet,otherEndIndex,startNodeIndex,i,actorCount);
           }
         }
                 inPort->setScore(valueResult * consUD); //consumption score
            }
            else
            {
                Port* outPort = tmpA->getIthPort(portId);
                Actor* otherEnd = outPort->getOtherEnd();
                int otherEndIndex = 0;
                for (int j=0; j<actorCount; j++)
                {
                    if (otherEnd == graph->getIthActor(j))
                    {
                        otherEndIndex = j;
                        break;
                    }
                }
                double valueResult = 0 ;
           for (int j=0; j<actorCount; j++)
           {
           Actor* tmpB;
           tmpB = graph->getIthActor(j);
           if (tmpB->isFinish())
           {
               finishNode = tmpB;
               finishNodeIndex = j;
               valueResult += calculateValueResult(inverseOutPortsMatrix,outPortsMatrix,outMatrixDet,otherEndIndex,finishNodeIndex,i,actorCount);
           }
           if(j==actorCount-1 && finishNode == NULL)
           {
                finishNodeIndex = graph->getActorCount()-1;
            finishNode = graph->getIthActor(finishNodeIndex);
               valueResult = calculateValueResult(inverseOutPortsMatrix,outPortsMatrix,outMatrixDet,otherEndIndex,finishNodeIndex,i,actorCount);
           }
           }
                outPort->setScore(valueResult * prodUD); //production score
            }
        }
    }

    for (int i=0; i<actorCount; i++)
    {
        delete[] *(inPortsMatrix + i);
    }
    delete[] inPortsMatrix;

    for (int i=0; i<actorCount; i++)
    {
        delete[] *(outPortsMatrix + i);
    }
    delete[] outPortsMatrix;
}

double Scorer::calculateDeterminant(double **matrix, const int n)
{
    double ** M;
    M = new double* [n];
    for (int i=0; i<n; i++)
    {
        M[i] = new double[n];
    }
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            M[i][j] = matrix[i][j];
        }
    }
    double det;
    double *temp;
    int i, j, k;

    det = 1;
    for(i = 0; i < n; i++)
    {
        //---------------
        if(M[i][i] == 0)
        {
            for (k = i + 1; k < n; k++)
            {
                if (M[k][i] == 0)
                {
                    continue;
                }
                break;
            }
            if (k == n)
            {
                return 0;
            }
            temp = M[i];
            M[i] = M[k];
            M[k] = temp;
            det *= -1;
        }
        //--------------
        for (j = i + 1; j < n; j++)
        {
            M[i][j] /= M[i][i]; // div
        }
        //---------------
        for (k = i + 1; k < n; k++)
        {
            if (M[k][i] != 0)
            {
                for(j = i + 1; j < n; j++)
                {
                    M[k][j] = M[k][j] /M[k][i] - M[i][j];
                }
                det *= M[k][i];
            }
        }
        det *= M[i][i];
    }

    for (int i=0; i<n; i++)
    {
        delete[] M[i];
    }
    delete[] M;
    return det;
}

double Scorer::calculateValueResult(double** inverse ,double **matrix, double det, int node, int endPoint, int index, int matSize)
{
    double valueResult = 0;
    valueResult += matrix[index][node];
    double cofac = calculateCofactorDeterminant(inverse,matSize,endPoint,node);
    valueResult /= det;
    valueResult *= cofac;
    if ((node+endPoint)%2==1)
    {
        valueResult *=-1;
    }
    return valueResult;
}

double Scorer::calculateCofactorDeterminant(double **matrix, const int n, int row, int column)
{
    double ** M;
    M = new double* [n-1];
    for (int i=0; i<n-1; i++)
    {
        M[i] = new double[n-1];
    }
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++)
        {
            if(i<row)
            {
                if(j<column)
                {
                    M[i][j] = matrix[i][j];
                }
                if(j>column)
                {
                    M[i][j-1] = matrix[i][j];
                }
            }
            if(i>row)
            {
                if(j<column)
                {
                    M[i-1][j] = matrix[i][j];
                }
                if(j>column)
                {
                    M[i-1][j-1] = matrix[i][j];
                }
            }
        }
    }

    double det;
    double *temp;
    int i, j, k;

    det = 1;
    for(i = 0; i < n-1; i++)
    {
        //---------------
        if(M[i][i] == 0)
        {
            for (k = i + 1; k < n-1; k++)
            {
                if (M[k][i] == 0)
                {
                    continue;
                }
                break;
            }
            if (k == n-1)
            {
                return 0;
            }
            temp = M[i];
            M[i] = M[k];
            M[k] = temp;
            det *= -1;
        }
        //--------------
        for (j = i + 1; j < n-1; j++)
        {
            M[i][j] /= M[i][i]; // div
        }
        //---------------
        for (k = i + 1; k < n-1; k++)
        {
            if (M[k][i] != 0)
            {
                for(j = i + 1; j < n-1; j++)
                {
                    M[k][j] = M[k][j] /M[k][i] - M[i][j];
                }
                det *= M[k][i];
            }
        }
        det *= M[i][i];
    }

    for (int i=0; i<n-1; i++)
    {
        delete[] M[i];
    }
    delete[] M;
    return det;
}

void Scorer::combinedGraphScore(Graph *graph)
{
    int actorCount;

    actorCount = graph->getActorCount();

    double** inPortsMatrix;
    inPortsMatrix = new double* [actorCount];
    for (int i=0; i<actorCount; i++)
    {
        *(inPortsMatrix + i) = new double [actorCount];
    }

    for (int i=0; i<actorCount; i++)
    {
        for (int j=0; j<actorCount; j++)
        {
            if (graph->getIthActor(i)->recievesFrom(graph->getIthActor(j)))
            {
                double value, outCost, inCost;
                int outCount, inCount;
                inCount = 0;
                outCount = 0;
                value = graph->getIthActor(j)->getExecTime();
                outCost = 0;
                inCost = 0;
                int portsCount = graph->getIthActor(j)->getPortCount();



                for (int k=0; k<portsCount; k++)
                {
                    if ((graph->getIthActor(j)->getIthPort(k)->getType()).compare("in") == 0)
                    {
                        inCount++;
                        inCost += graph->getIthActor(j)->getIthPort(k)->getRate();
                    }
                    else
                    {
                        outCount++;
                        outCost += graph->getIthActor(j)->getIthPort(k)->getRate();
                    }
                }

                if (inCount != 0)
                {
                    inCost /= inCount;
                }

                if (outCount != 0)
                {
                    outCost /= outCount;
                }

                value += inCost + outCost;

                value *= -1;

                value = exp(value/NORMALIZE);

                inPortsMatrix[i][j] = value;
            }
            else
            {
                inPortsMatrix[i][j] = 0;
                //*(*(inPortsMatrix + i) + j) = 0;
            }

        }
    }

    // M matrix produced, now turning to (I - M) matrix:

    double** inverseInPortsMatrix;
    inverseInPortsMatrix = new double* [actorCount];
    for (int i=0; i<actorCount; i++)
    {
        *(inverseInPortsMatrix + i) = new double [actorCount];
    }

    for (int i=0; i<actorCount; i++)
    {
        for (int j=0; j<actorCount; j++)
        {
            inverseInPortsMatrix[i][j] =  -inPortsMatrix[i][j];
            if (i == j)
            {
                inverseInPortsMatrix[i][i] ++;
            }
        }
    }

    double** outPortsMatrix;
    outPortsMatrix = new double* [actorCount];
    for (int i=0; i<actorCount; i++)
    {
        *(outPortsMatrix + i) = new double [actorCount];
    }

    for (int i=0; i<actorCount; i++)
    {
        for (int j=0; j<actorCount; j++)
        {
            if (graph->getIthActor(i)->sendsTo(graph->getIthActor(j)))
            {
                double value, outCost, inCost;
                int outCount, inCount;
                inCount = 0;
                outCount = 0;
                value = graph->getIthActor(j)->getExecTime();
                outCost = 0;
                inCost = 0;
                int portsCount = graph->getIthActor(j)->getPortCount();

                for (int k=0; k<portsCount; k++)
                {
                    if ((graph->getIthActor(j)->getIthPort(k)->getType()).compare("in") == 0)
                    {
                        inCount++;
                        inCost += graph->getIthActor(j)->getIthPort(k)->getRate();
                    }
                    else
                    {
                        outCount++;
                        outCost += graph->getIthActor(j)->getIthPort(k)->getRate();
                    }
                }

                if (inCount != 0)
                {
                    inCost /= inCount;
                }

                if (outCount != 0)
                {
                    outCost /= outCount;
                }

                value += inCost + outCost;

                value *= -1;

                value = exp(value/NORMALIZE);

                outPortsMatrix[i][j] = value;
            }
            else
            {
                outPortsMatrix[i][j] = 0;
                //*(*(outPortsMatrix + i) + j) = 0;
            }
        }
    }

    // M matrix produced, now turning to (I - M) matrix:

    double** inverseOutPortsMatrix;
    inverseOutPortsMatrix = new double* [actorCount];
    for (int i=0; i<actorCount; i++)
    {
        *(inverseOutPortsMatrix + i) = new double [actorCount];
    }
    for (int i=0; i<actorCount; i++)
    {
        for (int j=0; j<actorCount; j++)
        {
            inverseOutPortsMatrix[i][j] = -outPortsMatrix[i][j];
            if (i == j)
            {
                inverseOutPortsMatrix[i][i] ++;
            }
        }
    }

    //calculate determinants and create inverse matrix

    double inMatrixDet, outMatrixDet;
    inMatrixDet = calculateDeterminant(inverseInPortsMatrix,actorCount);
    outMatrixDet = calculateDeterminant(inverseOutPortsMatrix,actorCount);


//    set start and finish node

//    startNode = graph->getIthActor(startNodeIndex);
//    finishNode = graph->getIthActor(finishNodeIndex);

//    for (int i=0; i<actorCount; i++)
//    {
//        Actor* tmpA;
//        tmpA = graph->getIthActor(i);
//        if (tmpA->isStart())
//        {
//            startNode = tmpA;
//            startNodeIndex = i;
//            break;
//        }
//    }

//    for (int i=0; i<actorCount; i++)
//    {
//        Actor* tmpA;
//        tmpA = graph->getIthActor(i);
//        if (tmpA->isFinish())
//        {
//            finishNode = tmpA;
//            finishNodeIndex = i;
//            break;
//        }
//    }



    for (int i=0; i<actorCount; i++)
    {
        Actor* tmpA;
        tmpA = graph->getIthActor(i);
        int portCount = tmpA->getPortCount();

        for (int portId=0; portId<portCount; portId++)
        {
            Actor* startNode = NULL;
            Actor* finishNode = NULL;
            int startNodeIndex = 0;
            int finishNodeIndex = graph->getActorCount()-1;

            if (tmpA->getIthPort(portId)->getType().compare("in")==0)
            {
                Port* inPort = tmpA->getIthPort(portId);
                Actor* otherEnd = inPort->getOtherEnd();
                int otherEndIndex = 0;
                for (int j=0; j<actorCount; j++)
                {
                    if (otherEnd == graph->getIthActor(j))
                    {
                        otherEndIndex = j;
                        break;
                    }
                }
        double valueResult = 0 ;
        for (int j=0; j<actorCount; j++)
        {
           Actor* tmpB;
           tmpB = graph->getIthActor(j);
           if (tmpB->isStart())
           {
               startNode = tmpB;
               startNodeIndex = j;
            valueResult += calculateCombinedValueResult(inverseInPortsMatrix,inPortsMatrix,inMatrixDet,otherEndIndex,startNodeIndex,i,actorCount);
          }
           if(j==actorCount-1 && startNode == NULL)
           {
            startNodeIndex = 0;
            startNode = graph->getIthActor(startNodeIndex);
            valueResult = calculateCombinedValueResult(inverseInPortsMatrix,inPortsMatrix,inMatrixDet,otherEndIndex,startNodeIndex,i,actorCount);
           }
         }
            int inRate, outRate;
            Channel* tmpC;
            tmpC = inPort->getChannel();
            inRate = tmpC->getSrcPort()->getRate();
            outRate = tmpC->getDstPort()->getRate();
         valueResult = inRate / outRate * valueResult;
                 inPort->setScore(valueResult * consUD); //consumption score
            }
            else
            {
                Port* outPort = tmpA->getIthPort(portId);
                Actor* otherEnd = outPort->getOtherEnd();
                int otherEndIndex = 0;
                for (int j=0; j<actorCount; j++)
                {
                    if (otherEnd == graph->getIthActor(j))
                    {
                        otherEndIndex = j;
                        break;
                    }
                }
                double valueResult = 0 ;
           for (int j=0; j<actorCount; j++)
           {
           Actor* tmpB;
           tmpB = graph->getIthActor(j);
           if (tmpB->isFinish())
           {
               finishNode = tmpB;
               finishNodeIndex = j;
               valueResult += calculateCombinedValueResult(inverseOutPortsMatrix,outPortsMatrix,outMatrixDet,otherEndIndex,finishNodeIndex,i,actorCount);
           }
           if(j==actorCount-1 && finishNode == NULL)
           {
                finishNodeIndex = graph->getActorCount()-1;
            finishNode = graph->getIthActor(finishNodeIndex);
               valueResult = calculateCombinedValueResult(inverseOutPortsMatrix,outPortsMatrix,outMatrixDet,otherEndIndex,finishNodeIndex,i,actorCount);
           }
           }
            int inRate, outRate;
            Channel* tmpC;
            tmpC = outPort->getChannel();
            inRate = tmpC->getSrcPort()->getRate();
            outRate = tmpC->getDstPort()->getRate();
        valueResult = outRate / inRate * valueResult;
                outPort->setScore(valueResult * prodUD); //production score
            }
        }
    }

    for (int i=0; i<actorCount; i++)
    {
        delete[] *(inPortsMatrix + i);
    }
    delete[] inPortsMatrix;

    for (int i=0; i<actorCount; i++)
    {
        delete[] *(outPortsMatrix + i);
    }
    delete[] outPortsMatrix;
}

double Scorer::calculateCombinedValueResult(double** inverse ,double **matrix, double det, int node, int endPoint, int index, int matSize)
{
    double valueResult = 0;
    valueResult += matrix[index][node];
    double cofac = calculateCofactorDeterminant(inverse,matSize,endPoint,node);
    valueResult /= det;
    valueResult *= cofac;
    if ((node+endPoint)%2==1)
    {
        valueResult *=-1;
    }
    valueResult = log(valueResult) ;

    return valueResult;
}
