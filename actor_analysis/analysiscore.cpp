#include "analysiscore.h"

AnalysisCore::AnalysisCore(string fName, string inp, string outp)
{
    fileName = fName;
    outPath = outp;
    inPath = inp;
    graph = new Graph();

    ifstream input;
    input.open((inPath + "/" + fileName + ".xml"), ios::in);

    bool token = false;

    while (!(input.is_open()))
    {
        if (!token)
        {
            cout << "failed to open " << fileName << ".xml, resolve problems.\n";
            token = true;
        }
    }

    cout << fileName << ".xml opened successfully.\n";

    Parser* parser;
    parser = new Parser(&input);
    parser->parseXml(graph);
    delete parser;
    cout << "parsing input finished successfully.\n";
    input.close();
}

AnalysisCore::~AnalysisCore()
{
    delete graph;
}

bool AnalysisCore::generateUnsortedOutput(string fName, string inp, string outp)
{
    cout << "unsorted output generation:\n";

    // string unsortedOutFileName = outPath + "/" + fileName + "_unsorted.txt";

    // ofstream outputGraph;
    // outputGraph.open(unsortedOutFileName, ios::out);

    // bool token = false;
    // while (!(outputGraph.is_open()))
    // {
    //     if (!token)
    //     {
    //         cout << "failed to open " << unsortedOutFileName << ", resolve problems.\n";
    //         token = true;
    //     }
    // }
    // cout << unsortedOutFileName << " opened successfully.\n";

    // Output* unsortedOutput;
    // unsortedOutput = new Output();
    // unsortedOutput->generate(graph, outputGraph);
    // delete unsortedOutput;
    // cout << "writing to " << unsortedOutFileName << " finished successfully.\n";
    // outputGraph.close();
    // cout << unsortedOutFileName << " generated successfully.\n";


//***

    Output* unsortedSortedOutput1;


    ifstream input;
    input.open((inPath + "/" + fileName + ".xml"), ios::in);

//    string unsortedSortedOutFileName1 = outPath + "/" + fileName + "_unsorted_sorted.xml";
    string unsortedSortedOutFileName1 = outPath + "/" + outp + "_" + fileName + ".xml";

    ofstream unsortedSortedGraph1;
    unsortedSortedGraph1.open(unsortedSortedOutFileName1, ios::out);

    bool token1 = false;
    while (!(unsortedSortedGraph1.is_open()))
    {
        if (!token1)
        {
            cout << "failed to open " << unsortedSortedOutFileName1 << ", resolve problems.\n";
            token1 = true;
        }
    }
    cout << unsortedSortedOutFileName1 << " opened successfully.\n";

	unsortedSortedOutput1 = new Output();
    unsortedSortedOutput1->parseXml1(&input, graph, unsortedSortedGraph1);
    delete unsortedSortedOutput1;
    cout << "writing finished successfully.\n";
    unsortedSortedGraph1.close();
    cout << unsortedSortedOutFileName1 << " generated successfully.\n";


    return true;
}

bool AnalysisCore::generateSimpleSortedOutput(string fName, string inp, string outp)
{

    cout << "simple sorting algorithm:\n";

    scorer = new Scorer();
    scorer->simpleGraphScore(graph);
    delete scorer;
    cout << "scoring finished successfully.\n";

    sorter = new Sorter();
    sorter->portSorter(graph);
    delete sorter;
    cout << "sorting finished successfully.\n";


    // string simpleSortedOutFileName = outPath + "/" + fileName + "_simple_sorted.txt";

    // ofstream simpleSortedGraph;
    // simpleSortedGraph.open(simpleSortedOutFileName, ios::out);

    // bool token = false;
    // while (!(simpleSortedGraph.is_open()))
    // {
    //     if (!token)
    //     {
    //         cout << "failed to open " << simpleSortedOutFileName << ", resolve problems.\n";
    //         token = true;
    //     }
    // }
    // cout << simpleSortedOutFileName << " opened successfully.\n";

    // Output* simpleSortedOutput;
    // simpleSortedOutput = new Output();
    // simpleSortedOutput->generate(graph, simpleSortedGraph);
    // delete simpleSortedOutput;
    // cout << "writing finished successfully.\n";
    // simpleSortedGraph.close();
    // cout << simpleSortedOutFileName << " generated successfully.\n";

//***

    Output* simpleSortedOutput1;

    ifstream input;
    input.open((inPath + "/" + fileName + ".xml"), ios::in);

//    string simpleSortedOutFileName1 = outPath + "/" + fileName + "_simple_sorted.xml";
    string simpleSortedOutFileName1 = outPath + "/" + outp + "_" + fileName + ".xml";

    ofstream simpleSortedGraph1;
    simpleSortedGraph1.open(simpleSortedOutFileName1, ios::out);

    bool token1 = false;
    while (!(simpleSortedGraph1.is_open()))
    {
        if (!token1)
        {
            cout << "failed to open " << simpleSortedOutFileName1 << ", resolve problems.\n";
            token1 = true;
        }
    }
    cout << simpleSortedOutFileName1 << " opened successfully.\n";

	simpleSortedOutput1 = new Output();
    simpleSortedOutput1->parseXml1(&input, graph, simpleSortedGraph1);
    delete simpleSortedOutput1;
    cout << "writing finished successfully.\n";
    simpleSortedGraph1.close();
    cout << simpleSortedOutFileName1 << " generated successfully.\n";

    return true;
}

bool AnalysisCore::generateMatrixSortedOutput(string fName, string inp, string outp)
{
    cout << "matrix sorting algorithm:\n";

    scorer = new Scorer();
    scorer->matrixGraphScore(graph);
    delete scorer;
    cout << "scoring finished successfully.\n";

    sorter = new Sorter();
    sorter->portSorter(graph);
    delete sorter;
    cout << "sorting finished successfully.\n";


   //  string matrixSortedOutFileName = outPath + "/" + fileName + "_matrix_sorted.txt";

   //  ofstream matrixSortedGraph;
   //  matrixSortedGraph.open(matrixSortedOutFileName, ios::out);

   //  bool token = false;
   //  while (!(matrixSortedGraph.is_open()))
   //  {
   //      if (!token)
   //      {
   //          cout << "failed to open " << matrixSortedOutFileName << ", resolve problems.\n";
   //          token = true;
   //      }
   //  }
   //  cout << matrixSortedOutFileName << " opened successfully.\n";

   //  Output* matrixSortedOutput;
   //  matrixSortedOutput = new Output();
   //  matrixSortedOutput->generate(graph, matrixSortedGraph);
   //  delete matrixSortedOutput;
   //  cout << "writing finished successfully.\n";
   //  matrixSortedGraph.close();
   //  cout << matrixSortedOutFileName << " generated successfully.\n";
//***

    Output* matrixSortedOutput1;

    ifstream input;
    input.open((inPath + "/" + fileName + ".xml"), ios::in);

//    string matrixSortedOutFileName1 = outPath + "/" + fileName + "_matrix_sorted.xml";
    string matrixSortedOutFileName1 = outPath + "/" + outp + "_" + fileName + ".xml";

    ofstream matrixSortedGraph1;
    matrixSortedGraph1.open(matrixSortedOutFileName1, ios::out);

    bool token1 = false;
    while (!(matrixSortedGraph1.is_open()))
    {
        if (!token1)
        {
            cout << "failed to open " << matrixSortedOutFileName1 << ", resolve problems.\n";
            token1 = true;
        }
    }
    cout << matrixSortedOutFileName1 << " opened successfully.\n";

	matrixSortedOutput1 = new Output();
    matrixSortedOutput1->parseXml1(&input, graph, matrixSortedGraph1);
    delete matrixSortedOutput1;
    cout << "writing finished successfully.\n";
    matrixSortedGraph1.close();
    cout << matrixSortedOutFileName1 << " generated successfully.\n";

    return true;
}

//**
bool AnalysisCore::generateCombinedSortedOutput(string fName, string inp, string outp)
{
    cout << "combined sorting algorithm:\n";

    scorer = new Scorer();
    scorer->combinedGraphScore(graph);
    delete scorer;
    cout << "scoring finished successfully.\n";

    sorter = new Sorter();
    sorter->portSorter(graph);
    delete sorter;
    cout << "sorting finished successfully.\n";


    // string combinedSortedOutFileName = outPath + "/" + fileName + "_combined_sorted.txt";

    // ofstream combinedSortedGraph;
    // combinedSortedGraph.open(combinedSortedOutFileName, ios::out);

    // bool token = false;
    // while (!(combinedSortedGraph.is_open()))
    // {
    //     if (!token)
    //     {
    //         cout << "failed to open " << combinedSortedOutFileName << ", resolve problems.\n";
    //         token = true;
    //     }
    // }
    // cout << combinedSortedOutFileName << " opened successfully.\n";

    // Output* combinedSortedOutput;
    // combinedSortedOutput = new Output();
    // combinedSortedOutput->generate(graph, combinedSortedGraph);
    // delete combinedSortedOutput;
    // cout << "writing finished successfully.\n";
    // combinedSortedGraph.close();
    // cout << combinedSortedOutFileName << " generated successfully.\n";
//***

    Output* combinedSortedOutput1;

    ifstream input;
    input.open((inPath + "/" + fileName + ".xml"), ios::in);

//    string combinedSortedOutFileName1 = outPath + "/" + fileName + "_combined_sorted.xml";
    string combinedSortedOutFileName1 = outPath + "/" + outp + "_" + fileName + ".xml";

    ofstream combinedSortedGraph1;
    combinedSortedGraph1.open(combinedSortedOutFileName1, ios::out);

    bool token1 = false;
    while (!(combinedSortedGraph1.is_open()))
    {
        if (!token1)
        {
            cout << "failed to open " << combinedSortedOutFileName1 << ", resolve problems.\n";
            token1 = true;
        }
    }
    cout << combinedSortedOutFileName1 << " opened successfully.\n";

	combinedSortedOutput1 = new Output();
    combinedSortedOutput1->parseXml1(&input, graph, combinedSortedGraph1);
    delete combinedSortedOutput1;
    cout << "writing finished successfully.\n";
    combinedSortedGraph1.close();
    cout << combinedSortedOutFileName1 << " generated successfully.\n";


    return true;
}

bool AnalysisCore::generateRandomSortedOutput(int randCount, string fName, string inp, string outp)
{
    cout << "random sorting algorithm: take #" << randCount << "\n";

    scorer = new Scorer();
    scorer->randomGraphScore(graph);
    delete scorer;
    cout << "scoring finished successfully.\n";

    sorter = new Sorter();
    sorter->portSorter(graph);
    delete sorter;
    cout << "sorting finished successfully.\n";


    // string randomSortedOutFileName = outPath + "/" + fileName + "_random_sorted_" + to_string(randCount) + ".txt";

    // ofstream randomSortedGraph;
    // randomSortedGraph.open(randomSortedOutFileName, ios::out);

    // bool token = false;
    // while (!(randomSortedGraph.is_open()))
    // {
    //     if (!token)
    //     {
    //         cout << "failed to open " << randomSortedOutFileName << ", resolve problems.\n";
    //         token = true;
    //     }
    // }
    // cout << randomSortedOutFileName << " opened successfully.\n";

    // Output* randomSortedOutput;
    // randomSortedOutput = new Output();
    // randomSortedOutput->generate(graph, randomSortedGraph);
    // delete randomSortedOutput;
    // cout << "writing finished successfully.\n";
    // randomSortedGraph.close();
    // cout << randomSortedOutFileName << " generated successfully.\n";

//***

    Output* randomSortedOutput1;

    ifstream input;
    input.open((inPath + "/" + fileName + ".xml"), ios::in);

//    string randomSortedOutFileName1 = outPath + "/" + fileName + "_random_sorted_" + to_string(randCount) + ".xml";
    string randomSortedOutFileName1 = outPath + "/" + outp + "_" + fileName + ".xml";

    ofstream randomSortedGraph1;
    randomSortedGraph1.open(randomSortedOutFileName1, ios::out);

    bool token1 = false;
    while (!(randomSortedGraph1.is_open()))
    {
        if (!token1)
        {
            cout << "failed to open " << randomSortedOutFileName1 << ", resolve problems.\n";
            token1 = true;
        }
    }
    cout << randomSortedOutFileName1 << " opened successfully.\n";

	randomSortedOutput1 = new Output();
    randomSortedOutput1->parseXml1(&input, graph, randomSortedGraph1);
    delete randomSortedOutput1;
    cout << "writing finished successfully.\n";
    randomSortedGraph1.close();
    cout << randomSortedOutFileName1 << " generated successfully.\n";


    return true;
}
