#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "AntGraph.h"
#include "Logger.h"
#include "Ant.h"

int main()
{


    Logger::Logger *DebugLog = new Logger::Logger();
    Logger::DebugLevel debugLevel = Logger::DebugLevel::LOW;
    DebugLog->setDebugLevel(debugLevel);
    DebugLog->disableDebugLogging();

    AntGraph::pGraph *trails = new AntGraph::pGraph(20, 40, 100, DebugLog);
    AntGraph::pGraph *trails2 = new AntGraph::pGraph(20, 40, 100, DebugLog);

    trails->genRandomGraph();
    trails->printDistMatrix();

    trails->saveGraph("graphfile.out");

    trails2->loadGraph("graphfile.out");
    trails2->printDistMatrix();

    //trails->DikstraSSSP(0,0);
    //
    //unsigned int nextNode = trails->chooseNextNode(0,0);
    //DebugLog->InfoMsg("Next Node is: %u\n", nextNode);

    /*
    Ant::Ant *Bob = new Ant::Ant(trails,0,50, DebugLog);

    Bob->findDestination(50);
    */
}


