// 
// Graph Class
//
// Weighted Graph with pheromone values
// // Methods //  Create random graph of n nodes with a distribution of node connection #'s 
//  Print a graph
//  Save a graph
//  Load a graph
//  Get shortest path from node a to node b
//  Get highest pheromone path
//  


//class pGraph;
//class Heap;
//

#include "Logger.h"

#ifndef ANTHEAP_H
#define ANTHEAP_H

#define AG_VERSION 1
#define MAX_LINE_LEN 256

namespace AntGraph {

    class Heap {
        private:
            Logger::Logger *DebugLog;

        public:
            std::vector<int> value;
            std::vector<unsigned int> key;
            int direction;  // 0 = min, 1 = max

            Heap();
            Heap(Logger::Logger*);
            void setDirection(unsigned int);
            void HeapSort();
            void addValue(int);
            void updateValue(unsigned int, int);
            int removeNode(unsigned int);
            unsigned int getHeapSize();
            int getHeapTopValue();
            int getHeapValue(unsigned int);
            int getHeapMinValue();
            void setHeapValue(unsigned int, int);
            unsigned int getHeapTopIndex();
            void printHeap();
    };

    class pGraph {
        private:
            std::vector<int> distAdjMatrix;
            std::vector<int> pherAdjMatrix;

            unsigned int nodeCount;
            unsigned int trailCount;

            unsigned int maxDistance;

            Heap *distHeap;
            Logger::Logger *DebugLog;

            unsigned int seed;
            std::default_random_engine generator;

            unsigned int fileVersion;
        public:
            pGraph();
            pGraph(unsigned int, int, unsigned int, Logger::Logger*);

            unsigned int getMatrixIndex(unsigned int, unsigned int);

            void genRandomGraph();

            void addPheromone(unsigned int, unsigned int, int);
            int getPheromone(unsigned int, unsigned int);
            std::vector<int>* getPheromones(unsigned int, std::vector<unsigned int>*);

            void setDistance(unsigned int, unsigned int, int);
            int getDistance(unsigned int, unsigned int);

            void DikstraSSSP(int, unsigned int);

            unsigned int chooseNextNode(unsigned int, unsigned int);
            std::vector<unsigned int> *getAdjNodes(unsigned int);

            void saveGraph(const std::string &);
            void loadGraph(const std::string &);

            void printDistMatrix();
            void printPherMatrix();

            int getRandomNumber(int, int);
    };

}
#endif
