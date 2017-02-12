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
        DebugLog->DebugMsg(Logger::DebugLevel::LOW, "Ant create to look for %u from %u\n", end_node, start_node);
    }

    void Ant::findDestination(int max_steps)
    {
        int steps = 0;
        while((isDestination() == 0) && (steps < max_steps))
        {
            moveForward();
        }
    }

    void Ant::moveForward()
    {
        next_node = Graph->chooseNextNode(this->current_node, this->prev_node);
        prev_node = current_node; 
        current_node = next_node;

        path_nodes.push_back(current_node);
        path_distances.push_back(Graph->getDistance(current_node, prev_node));

        DebugLog->DebugMsg(Logger::DebugLevel::LOW, "Ant moving to %u from %u\n", current_node, prev_node);
    }

    int Ant::isDestination()
    {
        return(current_node == end_node);
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
