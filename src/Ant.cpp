// 
// Ant Class
//
// Automata Class - 
//  
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>

#include "AntGraph.h"
#include "Logger.h"
#include "Ant.h"

namespace Ant
{
    Ant::Ant(AntGraph::pGraph *graph, unsigned int src, unsigned int dest, Logger::Logger *Log)
    {
        this->Graph = graph;
        this->DebugLog = Log;

        start_node = src;
        end_node = dest;
        current_node = src;
        next_node = 0;
        prev_node = src;
        DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "Ant create to look for %u from %u\n", end_node, start_node);
    }

    int Ant::findDestination(int max_steps)
    {
        int steps = 0;
        current_node = start_node;
        prev_node = start_node;
        next_node = start_node;

        if(this->getNumAdjNodes(start_node) == 0)
            return(0);

        while((isDestination() == 0) && (steps < max_steps))
        {
            steps++;
            DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "Step %d\n", steps);
            moveForward();
        }
        if(steps == max_steps)
            DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "Didn't find destination in %d steps.\n", steps);

        for(unsigned int index = 0; index < path_nodes.size(); index++)
            DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "%u(%u):", path_nodes.at(index), path_distances.at(index));
        DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "\n");
        return(isDestination());

    }

    void Ant::moveForward()
    {
        do {
            next_node = Graph->chooseNextNode(this->current_node, this->prev_node);
        } while(next_node == current_node);

        prev_node = current_node; 
        current_node = next_node;

        DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "Ant moving to %u from %u\t Distance = %u\n", current_node, prev_node, this->Graph->getDistance(current_node, prev_node));

        path_nodes.push_back(current_node);
        path_distances.push_back(Graph->getDistance(current_node, prev_node));

    }

    int Ant::isDestination()
    {
        if(current_node == end_node)
        {
            DebugLog->InfoMsg("Found destination node %u from %u\n", end_node, start_node);
            DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "\n");
            return(1);
        }
        return(0);
    }

    void Ant::setDestination(unsigned int node)
    {
        end_node = node;
    }

    void Ant::setStart(unsigned int node)
    {
        start_node = node;
    }

}
