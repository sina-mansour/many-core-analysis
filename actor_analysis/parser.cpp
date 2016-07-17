#include "parser.h"

#define DELIMETER "\""

Parser::Parser()
{
    inFile = NULL;
}

Parser::Parser(ifstream *in)
{
    inFile = in;
}

void Parser::parseXml(Graph *graph)
{
    string line, word;
    int actorId = 0;
    int channelId = 0;

    while (!(inFile->eof()))
    {
        getline(*inFile, line);

        if(line.find("<applicationGraph") != string::npos)
        {
            word = "noname";
            if(line.find("name=") != string::npos)
            {
                word = line.substr(line.find("name=")+6);
                word = word.substr(0 ,  word.find(DELIMETER));
            }
            graph->setGraphName(word);
            cout << "graph name:\t" << word << endl;
        }

        else if(line.find("<actor ") != string::npos)
        {
            word = line.substr(line.find("name=")+6);
            word = word.substr(0, word.find(DELIMETER));
            actorId++;
            graph->addActor(word,actorId);

            Actor* tempA;
            tempA = graph->getActor(actorId);
            int portId = 0;

            getline(*inFile, line);

            while (line.find("/actor") == string::npos)
            {
                // Escape ports related to self-loop
                if((line.find("pIn") != string::npos) || (line.find("pOut") != string::npos))
                {
                    getline(*inFile, line);
                    continue;
                }

                if(line.find("in") != string::npos) // Inport
                {
                    word = line.substr(line.find("name=")+6);
                    word = word.substr(0, word.find(DELIMETER));
                    string pid = word;
                    word = line.substr(line.find("rate=")+6);
                    word = word.substr(0, word.find(DELIMETER));
                    int rate = stoi(word);
                    portId++;
                    tempA->addPort("in", "none", pid, rate, 0, portId, NULL);
                }
                else	// Outport
                {
                    word = line.substr(line.find("name=")+6);
                    word = word.substr(0, word.find(DELIMETER));
                    string pid = word;
                    word = line.substr(line.find("rate=")+6);
                    word = word.substr(0, word.find(DELIMETER));
                    int rate = stoi(word);
                    portId++;
                    tempA->addPort("out", "none", pid, rate, 0, portId, NULL);
                }

                getline(*inFile, line);
            }
        }

        else if (line.find("<channel ") != string::npos)
        {
            // Escape ports related to self-loop
            if((line.find("pIn") != string::npos) || (line.find("pOut") != string::npos));
            else
            {
                Actor* tmpSrcA;
                Port* tmpSrcP;
                Actor* tmpDstA;
                Port* tmpDstP;

                word = line.substr(line.find("name=")+6);
                word = word.substr(0, word.find(DELIMETER));
                string name = word;
                word = line.substr(line.find("srcActor=")+10);
                word = word.substr(0, word.find(DELIMETER));
                string srcA = word;
                word = line.substr(line.find("dstActor=")+10);
                word = word.substr(0, word.find(DELIMETER));
                string dstA = word;
                word = line.substr(line.find("srcPort=")+9);
                word = word.substr(0, word.find(DELIMETER));
                string srcP = word;
                word = line.substr(line.find("dstPort=")+9);
                word = word.substr(0, word.find(DELIMETER));
                string dstP = word;

                int initial = 0;

                if (line.find("initialTokens=") != string::npos)
                {
                    word = line.substr(line.find("initialTokens=")+15);
                    word = word.substr(0, word.find(DELIMETER));
                    initial = stoi(word);
                }

                tmpSrcA = graph->getActor(srcA);
                if (tmpSrcA == NULL)
                    cout << srcA << "\tnull src actor.\n";
                tmpSrcP = tmpSrcA->getPidPort(srcP);
                if (tmpSrcP == NULL)
                    cout << srcP << "\tnull src port.\n";
                tmpSrcP->setName(name);
                tmpSrcP->setInitial(initial);

                tmpDstA = graph->getActor(dstA);
                if (tmpDstA == NULL)
                    cout << dstA << "\tnull dst actor.\n";
                tmpDstP = tmpDstA->getPidPort(dstP);
                if (tmpDstP == NULL)
                    cout << dstP << "\tnull dst port.\n";
                tmpDstP->setName(name);
                tmpDstP->setInitial(initial);

                channelId++;
                graph->addChannel(name, tmpSrcA, tmpDstA, tmpSrcP, tmpDstP, initial, channelId);

                Channel* tmpC;
                tmpC = graph->getChannel(name);

                tmpSrcP->setChannel(tmpC);
                tmpSrcP->setOtherEnd(tmpDstA);

                tmpDstP->setChannel(tmpC);
                tmpDstP->setOtherEnd(tmpSrcA);
            }
        }

        else if (line.find("<actorProperties ") != string::npos)
        {
            word = line.substr(line.find("actor=")+7);
            word = word.substr(0, word.find(DELIMETER));
            string name;
            name = word;
            while (line.find("<executionTime ") == string::npos)
            {
                getline(*inFile, line);
            }
            word = line.substr(line.find("time=")+6);
            word = word.substr(0, word.find(DELIMETER));
            int eTime;
            eTime = stoi(word);

            Actor* tmpA;
            tmpA = graph->getActor(name);
            tmpA->setExecTime(eTime);
        }
    }
}
