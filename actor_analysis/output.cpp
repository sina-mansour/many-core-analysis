#include "output.h"

#define DELIMETER "\""



Output::Output()

{

}



void Output::parseXml1(ifstream* inFile, Graph* inGraph, ofstream& outFile)

{

    string line, word;

    int actorId = 0;

    int channelId = 0;



    if (!(outFile.is_open()));

    else

    {



        while (!(inFile->eof()))

        {

            getline(*inFile, line);

            if(line.find("<actor ") != string::npos)

            {

                Actor* tmpA;

                word = "noname";

                if(line.find("name=") != string::npos)

                {

                    word = line.substr(line.find("name=")+6);

                    word = word.substr(0 ,  word.find(DELIMETER));

                }

                outFile<<line<<endl;

                getline(*inFile, line);

                tmpA=inGraph->getActor(word);

                int pCount = tmpA->getPortCount();

                int wCount = 0, rCount = 0;

                int id;

                for(int j=0; j<pCount; j++)

                {

                    Port* tmpP;

                    tmpP = tmpA->getIthPort(j);

                    if (tmpP->getPid()=="pIn" || tmpP->getPid()=="pOut"){

                        id=-1;

                    }

                    else if (tmpP->getType()=="out"){

                        id=wCount;

                        wCount++;

                    }

                   else if (tmpP->getType()=="in"){

                        id=rCount;

                        rCount++;

                    }

                    outFile << "            <port type=\""<<tmpP->getType()<<"\" name=\""<<tmpP->getPid()

                    <<"\" rate=\""<<tmpP->getRate()<<"\" id=\""<<id<<"\" score=\""<<tmpP->getScore()<<"\"/>"<< endl;
                    // <<"\" rate=\""<<tmpP->getRate()<<"\" id=\""<<id<<"\"/>"<< endl;

                }

            }

            else if(line.find("<port ") != string::npos)

            {
/*
                if(line.find("pOut") != string::npos || line.find("pIn") != string::npos)
                {
                    line.erase (line.end()-2,line.end());

                    outFile<<line<<" id=\"-1\"/>\n";;
                }
*/
            }

            else

            {

                outFile<<line<<endl;

            }

        }

    }

}





void Output::generate(Graph *graph, ofstream& outFile)

{

    if (!(outFile.is_open()));

    else

    {

        int cCount = graph->getChannelCount();

        outFile << cCount << endl;



        for (int i=0; i<cCount; i++)

        {

            Channel* tmpC;

            tmpC = graph->getIthChannel(i);

            outFile << BUFFER_SIZE << " " << tmpC->getInitialToken() << endl;

        }



        int aCount = graph->getActorCount();

        outFile << aCount << endl;



        for (int i=0; i<aCount; i++)

        {

            outFile << 1 << endl;

            Actor* tmpA;

            tmpA = graph->getIthActor(i);

            int accessCount = 0;



            int pCount = tmpA->getPortCount();

            outFile << pCount << endl;

    outFile<<"\n"<<tmpA->getName()<<"\n";

            for (int j=0; j<pCount; j++)

            {

                Port* tmpP;

                tmpP = tmpA->getIthPort(j);

                string cName;

                cName = tmpP->getChannel()->getName();

                accessCount += tmpP->getRate();



                int index = 0;



                while ((graph->getIthChannel(index)->getName().compare(cName))!=0)

                {

                    index++;

                }



                outFile << tmpP->getType() << " " << index << " " << (PORT_DELAY) << endl;

            }



            outFile << accessCount << endl;





            for (int j=0; j<tmpA->getPortCount(); j++)

            {

                Port* tmpP;

                tmpP = tmpA->getIthPort(j);

                int rateCount;

                rateCount = tmpP->getRate();

                bool isOut;

                isOut = !(tmpP->getType().compare("out"));

                int accessTime;

                accessTime = 0;

                if (isOut)

                {

                    accessTime = (tmpA->getExecTime());

                }



                for (int k=0; k<rateCount; k++)

                {

                    outFile << 0 << " " << j << " "  << accessTime << " " /*<< tmpP->getScore()*/ << endl;

                }



            }



            outFile << 0 << endl;

        }

    }

}

