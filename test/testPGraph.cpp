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

    unsigned int nodeCnt = 50;
    unsigned int trailCnt = 100;
    unsigned int maxD = 100;
    unsigned int maxSteps = 40;

    Logger::Logger *DebugLog = new Logger::Logger();
    Logger::DebugLevel debugLevel = Logger::DebugLevel::LOW;
    DebugLog->setDebugLevel(debugLevel);
    DebugLog->enableDebugLogging();

    AntGraph::pGraph *newTrails = new AntGraph::pGraph(nodeCnt, trailCnt, maxD, DebugLog);
    //AntGraph::pGraph *trails = new AntGraph::pGraph(nodeCnt, trailCnt, maxD, DebugLog);

    newTrails->genRandomGraph();
    //newTrails->printDistMatrix();
    newTrails->saveGraph("graphfile.out");

    //trails->loadGraph("graphfile.out");
    //trails->printDistMatrix();
    //trails->printPherMatrix();
    //trails->saveGraph("graphfile2.out");

    //trails->DikstraSSSP(0,0);
    //
    //unsigned int nextNode = trails->chooseNextNode(0,0);
    //DebugLog->InfoMsg("Next Node is: %u\n", nextNode);

    Ant::Ant *Bob = new Ant::Ant(newTrails,0,nodeCnt-10, DebugLog);

    if(Bob->findDestination(maxSteps))
        std::cout << "Success!!" << std::endl;
    else
        std::cout << "Failure!!" << std::endl;
}


