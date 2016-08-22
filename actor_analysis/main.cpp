#include "IncludeLibraries.h"

using namespace std;

string getFileName(const string s) {

   size_t i = s.rfind('/', s.length());
   if (i != string::npos) {
      return(s.substr(i+1, s.length() - i - 5));
   }

   return("");
}

string getFilePath(const string s) {

   size_t i = s.rfind('/', s.length());
   if (i != string::npos) {
      return(s.substr(0,i));
   }

   return(s);
}

void runBenchmark(string fileDir, string outP, int random_times)
{
    string fileName = getFileName(fileDir);
    string inP = getFilePath(fileDir);

    AnalysisCore* analyser;

    analyser = new AnalysisCore(fileName, inP, outP);

    analyser->generateUnsortedOutput(fileName, inP, "unsorted");

    analyser->generateSimpleSortedOutput(fileName, inP, "simple_a_a", "a_a");
    analyser->generateSimpleSortedOutput(fileName, inP, "simple_a_d", "a_d");
    analyser->generateSimpleSortedOutput(fileName, inP, "simple_d_a", "d_a");
    analyser->generateSimpleSortedOutput(fileName, inP, "simple_d_d", "d_d");

    analyser->generateMatrixSortedOutput(fileName, inP, "matrix_a_a", "a_a");
    analyser->generateMatrixSortedOutput(fileName, inP, "matrix_a_d", "a_d");
    analyser->generateMatrixSortedOutput(fileName, inP, "matrix_d_a", "d_a");
    analyser->generateMatrixSortedOutput(fileName, inP, "matrix_d_d", "d_d");

    analyser->generateCombinedSortedOutput(fileName, inP, "combined_a_a", "a_a");
    analyser->generateCombinedSortedOutput(fileName, inP, "combined_a_d", "a_d");
    analyser->generateCombinedSortedOutput(fileName, inP, "combined_d_a", "d_a");
    analyser->generateCombinedSortedOutput(fileName, inP, "combined_d_d", "d_d");
        
    for (int i=1; i<=random_times; i++)
    {
        srand(time(NULL));
        analyser->generateRandomSortedOutput(i, fileName, inP, "random_" + to_string(i));
    }

    delete analyser;
}

int main(int argc, char *argv[])
{

    char cwd[1024];
    string directory;

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        directory = string(cwd);
    else {
        cerr << "failed to get working directory" << endl;
        return 1;
    }

    // cout << directory << endl;

    // Check the number of parameters
    if (argc < 4) {
        // Tell the user how to run the program
        cerr << "Usage: " << argv[0] << " <path to output directory> <number of random tries> <path to input xml files (space seperated)>" << endl;
        return 1;
    }

    string outPath = string(argv[1]); // "results/buffer" + to_string(BUFFER_SIZE);

    int random_times = atoi((argv[2]));
    // cout << string(argv[2]) << " : " << random_times << endl;

    vector<string> fileDir;

    for (int i = 3; i < argc; ++i) {
        if (string(argv[i]).find(".xml") == string(argv[i]).length()-4) {
            fileDir.push_back(string(argv[i]));
        }
        else {
            cout << "please insert only xml files, <"+ string(argv[i]) +"> is not a valid file path" << endl;
        }
    }

    // cout << getFileName(fileDir[0]) << endl;
    // cout << getFilePath(fileDir[0]) << endl;

    for (int i=0; i<fileDir.size(); i++)
    {
        runBenchmark(fileDir[i], outPath, random_times);
    }

    //    runBenchmark("sample","inputs","results");

    cout << "program finished.\n";
}
