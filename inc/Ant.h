// 
// Ant Class
//
// Parameters
//  Current Path from Source
//    Node to Node with Distances
//  Direction - Forward or Backward
//
// Methods
//  Choose Path
//  Deposit Pheromone
//
//  

#include "AntGraph.h"

#ifndef ANT_H
#define ANT_H
namespace Ant{

    class Ant {
        private:
            unsigned int start_node, end_node;
            unsigned int prev_node;
            unsigned int current_node;
            unsigned int next_node;

            std::vector<unsigned int> path_nodes;
            std::vector<int> path_distances;
            AntGraph::pGraph *Graph;
            Logger::Logger *DebugLog;

        public:
            Ant(AntGraph::pGraph *,unsigned int, unsigned int, Logger::Logger *);
            void findDestination(int);
            void backTrack();
            void moveForward();
            int isDestination();
            int removePathLoops();
            void setDestination(unsigned int);
            void setStart(unsigned int);
    };
}
#endif
