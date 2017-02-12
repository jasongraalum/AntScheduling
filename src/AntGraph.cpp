// 
// Graph Class
//
// Weighted Graph with pheromone values
//
// Methods
//  Create random graph of n nodes with a distribution of node connection #'s 
//  Print a graph
//  Save a graph
//  Load a graph
//  Get shortest path from node a to node b
//  Get highest pheromone path
//  

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <chrono>
#include <random>

#include "AntGraph.h"
#include "Logger.h"

namespace AntGraph
{
    void pGraph::saveGraph(const std::string &filename)
    {

        // Elements of a Graph
        std::size_t distAdjMatrixSize = this->distAdjMatrix.size();
        std::size_t pherAdjMatrixSize = this->pherAdjMatrix.size();
        std::size_t nodeCountSize = sizeof(unsigned int);
        std::size_t trailCountSize = sizeof(unsigned int);
        std::size_t maxDistanceSize = sizeof(int);
        std::size_t distHeapPtrSize = sizeof(AntGraph::Heap*);
        std::size_t DebugLogPtrSize = sizeof(Logger::Logger*);
        std::size_t seedSize = sizeof(int);
        std::size_t generatorSize = sizeof(std::mt19937);

        std::ofstream out(filename);
        // Write the size of each element to the file

        out.write((char *) distAdjMatrixSize, sizeof(std::size_t));
        out.write((char *) pherAdjMatrixSize, sizeof(std::size_t));
        out.write((char *) nodeCountSize, sizeof(std::size_t));
        out.write((char *) trailCountSize, sizeof(std::size_t));
        out.write((char *) maxDistanceSize, sizeof(std::size_t));
        out.write((char *) distHeapPtrSize, sizeof(std::size_t));
        out.write((char *) DebugLogPtrSize, sizeof(std::size_t));
        out.write((char *) seedSize, sizeof(std::size_t));
        out.write((char *) generatorSize, sizeof(std::size_t));

        out.write((char *) this->distAdjMatrix.size() , distAdjMatrixSize);
        out.write((char *) this->pherAdjMatrix.size() , pherAdjMatrixSize);
        out.write((char *) nodeCount, nodeCountSize);
        out.write((char *) trailCount, trailCountSize);
        out.write((char *) maxDistanceSize, maxDistance);
        out.write((char *) &distHeap, distHeapPtrSize);
        out.write((char *) &DebugLog, DebugLogPtrSize);
        out.write((char *) seed, seedSize);
        out.write((char *) generator, generatorSize);
    }
    void pGraph::loadGraph(const std::string &filename)
    {
        // Elements of a Graph
        std::size_t distAdjMatrixSize = this->distAdjMatrix.size();
        std::size_t pherAdjMatrixSize = this->pherAdjMatrix.size();
        std::size_t nodeCountSize = sizeof(unsigned int);
        std::size_t trailCountSize = sizeof(unsigned int);
        std::size_t maxDistanceSize = sizeof(int);
        std::size_t distHeapPtrSize = sizeof(AntGraph::Heap*);
        std::size_t DebugLogPtrSize = sizeof(Logger::Logger*);
        std::size_t seedSize = sizeof(int);
        std::size_t generatorSize = sizeof(std::mt19937);

        std::ifstream in(filename);
        // Write the size of each element to the file

        in.read((char *) distAdjMatrixSize, sizeof(std::size_t));
        in.read((char *) pherAdjMatrixSize, sizeof(std::size_t));
        in.read((char *) nodeCountSize, sizeof(std::size_t));
        in.read((char *) trailCountSize, sizeof(std::size_t));
        in.read((char *) maxDistanceSize, sizeof(std::size_t));
        in.read((char *) distHeapPtrSize, sizeof(std::size_t));
        in.read((char *) DebugLogPtrSize, sizeof(std::size_t));
        in.read((char *) seedSize, sizeof(std::size_t));
        in.read((char *) generatorSize, sizeof(std::size_t));

        in.read((char *) distAdjMatrix , distAdjMatrixSize);
        in.read((char *) pherAdjMatrix , pherAdjMatrixSize);
        in.read((char *) nodeCount, nodeCountSize);
        in.read((char *) trailCount, trailCountSize);
        in.read((char *) maxDistanceSize, maxDistance);
        in.read((char *) &distHeap, distHeapPtrSize);
        in.read((char *) &DebugLog, DebugLogPtrSize);
        in.read((char *) seed, seedSize);
        in.read((char *) generator, generatorSize);
    }

    void pGraph::loadGraph(const std::string &filename)
    {
        std::ifstream in(filename);
        in.seekg (0,in.end);
        long size = in.tellg();
        std::cout << "Reading " << size << "bytes" << std::endl;
        in.seekg(0);
        in.read((char *) this, size);
    }

    unsigned int pGraph::chooseNextNode(unsigned int srcNode, unsigned int prevNode)
    {
        std::vector<unsigned int> nodes;

        DebugLog->InfoMsg("Finding next node from %u\n", srcNode);
        
        // Get the adjancent nodes
        nodes = this->getAdjNodes(srcNode);

        // Remove previous node
        DebugLog->InfoMsg("Nodes(%d) to %u:\n", nodes.size(), srcNode);
        for(std::vector<unsigned int>::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            DebugLog->InfoMsg("Checking node: %u\n", *it);
            if(*it == prevNode)
                nodes.erase(it);
        }

        // Get pheromone numbers to generate distribution
        std::vector<int> *pheromones = this->getPheromones(srcNode, nodes);

        // Normalize to 100 and gen random number between 0 and 100
        int total_pheromone = 0;
        //for(std::vector<int>::iterator it = pheromones->begin(); it != pheromones->end(); ++it)
        for(unsigned int it = 0; it < pheromones->size(); ++it)
        {
            DebugLog->InfoMsg("%u\tAdding %d\tTotal = %d\n", nodes.at(it), pheromones->at(it), total_pheromone);
            total_pheromone += pheromones->at(it);
        }
        DebugLog->InfoMsg("Total pheromone: %d\n", total_pheromone);

        int weighted_pheromone = this->getRandomNumber(0, total_pheromone);

        DebugLog->InfoMsg("Weighted Pheromone = %d\n", weighted_pheromone);

        total_pheromone = 0;
        unsigned int index = 0;
        std::vector<int>::iterator it = pheromones->begin(); 
        do {
            total_pheromone += pheromones->at(index);
            index++;
        } while(total_pheromone < weighted_pheromone);

        return((unsigned int) nodes.at(index-1));
    }

    std::vector<unsigned int> pGraph::getAdjNodes(unsigned int node)
    {
        std::vector<unsigned int> nodes;
        for(unsigned int nodeB = 0; nodeB < this->nodeCount; nodeB++)
        {
            if(this->getDistance(node, nodeB) > 0)
            {
                nodes.push_back(nodeB);
            }
        }
        return(nodes);
    }

    std::vector<int>* pGraph::getPheromones(unsigned int src_node, std::vector<unsigned  int> node_list)
    {
        std::vector<int> *pheromone_list =new std::vector<int>();
        for(std::vector<unsigned int>::iterator it = node_list.begin(); it != node_list.end(); ++it)
        {
            pheromone_list->push_back(getPheromone(src_node,*it));
        }
        return(pheromone_list);

    }

    unsigned int pGraph::getMatrixIndex(unsigned int A, unsigned int B)
    {
        return(this->nodeCount*A + B);
    }

    int pGraph::getPheromone(unsigned int nodeA, unsigned int nodeB)
    {
        return(this->pherAdjMatrix.at(this->getMatrixIndex(nodeA,nodeB)));
    }

    void pGraph::addPheromone(unsigned int nodeA, unsigned int nodeB, int pLevel)
    {
        this->pherAdjMatrix.at(this->getMatrixIndex(nodeA, nodeB)) += pLevel;
    }

    void pGraph::setDistance(unsigned int nodeA, unsigned int nodeB, int distance)
    {
        this->distAdjMatrix.at(this->getMatrixIndex(nodeA, nodeB)) = distance;
    }

    int pGraph::getDistance(unsigned int nodeA, unsigned int nodeB)
    {
        return(this->distAdjMatrix.at(this->getMatrixIndex(nodeA, nodeB)));
    }

    //
    // Constructor Method
    // Arg: number of nodes and max distance
    //
    // 

    pGraph::pGraph()
    {
    }

    pGraph::pGraph(unsigned int nodeCount, int maxDist, unsigned int trailCount, Logger::Logger *Log)
    {

        this->nodeCount = nodeCount;
        this->maxDistance = maxDist;
        this->trailCount = trailCount;

        this->DebugLog = Log;

        // 
        // Resize the matrixes used to store the graph
        //
        // Distance and Pherome levels....maybe create struct for both?
        //
        DebugLog->DebugMsg(Logger::DebugLevel::LOW,"Trying to resize the distAdjMatrix\n");
        this->distAdjMatrix.resize(nodeCount*nodeCount);
        std::fill(this->distAdjMatrix.begin(), this->distAdjMatrix.end(), 0);

        DebugLog->DebugMsg(Logger::DebugLevel::LOW, "Trying to resize the pherAdjMatrix\n");
        this->pherAdjMatrix.resize(nodeCount*nodeCount);
        std::fill(this->pherAdjMatrix.begin(), this->pherAdjMatrix.end(), 0);

        DebugLog->DebugMsg(Logger::DebugLevel::LOW, "Distance Adjaceny Matrix size: %d \n", this->distAdjMatrix.size());
        DebugLog->DebugMsg(Logger::DebugLevel::LOW, "Pheromone Adjaceny Matrix size: %d \n", this->pherAdjMatrix.size());

        this->distHeap = new Heap(this->DebugLog);

        //
        // Set random number seed
        //
        typedef std::chrono::high_resolution_clock myclock;
        myclock::time_point beginning = myclock::now();

        myclock::duration d = myclock::now() - beginning;
        this->seed = d.count();
        std::mt19937 gen (this->seed);
        this->generator = gen;
    }

    //
    // Generate trails between random nodes with random distances
    //
    void pGraph::genRandomGraph()
    {
        int dups = 0;
        int loops = 0;
        DebugLog->DebugMsg(Logger::DebugLevel::MED, "Generating %d trails between %d nodes\n", this->trailCount, this->nodeCount);

        unsigned int nodeA, nodeB; 
        int distance, pher; 

        for(unsigned int i = 0; i < this->trailCount; ++i)
        {
            DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "Adding trail # %d\n", i);

            nodeA = this->getRandomNumber(0, (this->nodeCount-1));
            do {
                nodeB = this->getRandomNumber(0, (this->nodeCount-1));
                loops++;
            } while(nodeB == nodeA);
            loops--;

            DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "Node 1: %d/n", nodeA); 
            DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "Node s: %d/n", nodeB); 

            if(this->distAdjMatrix.at(this->getMatrixIndex(nodeA, nodeB)) == 0)
            {
                pher = this->getRandomNumber(0, 100);

                this->pherAdjMatrix.at(this->getMatrixIndex(nodeA,nodeB)) = pher;
                this->pherAdjMatrix.at(this->getMatrixIndex(nodeB,nodeA)) = pher;

                distance = this->getRandomNumber(1, this->maxDistance);
                this->distAdjMatrix.at(this->getMatrixIndex(nodeA,nodeB)) = distance;
                this->distAdjMatrix.at(this->getMatrixIndex(nodeB,nodeA)) = distance;


                DebugLog->DebugMsg(Logger::DebugLevel::MED, "Adding trail #%d of length %d between %d and %d\n",i, distance,nodeA, nodeB);
            }
            else
            {
                DebugLog->DebugMsg(Logger::DebugLevel::MED, "Duplicate trail\n");
                dups++;
            }
        }

        DebugLog->DebugMsg(Logger::DebugLevel::MED, "%d duplicate trails\n", dups);
        DebugLog->DebugMsg(Logger::DebugLevel::MED, "%d loops\n", loops);

    }

    void pGraph::DikstraSSSP(int graph, unsigned int node0)
    {
        // std::vector<unsigned int> *SSSP;
        std::vector<int> distances;
        std::vector<unsigned int> path;
        std::vector<unsigned int> nodeVisited;

        /*
           if(graph == 0) // Distance
           SSSP = &distSSSP;
           if(graph == 1) // Pheromone
           SSSP = &pherSSSP;
           */

        distances.resize(nodeCount);

        //
        // Assign -1(inf) to each node value
        //
        //for(std::vector<unsigned int>::iterator it = (this->distHeap->key).begin(); it != (this->distHeap->key).end(); ++it)
        for(unsigned int i = 0; i < this->nodeCount; i++)
        {
            this->distHeap->addValue(std::numeric_limits<int>::max());
            nodeVisited.push_back(0);
        }

        // 
        // Assign the source node a distance of 0 and sortHeap.
        //
        this->distHeap->updateValue(node0, 0);
        this->distHeap->HeapSort();
        //this->distHeap->printHeap();

        // Loop should start here
        //
        //
        // Put distance update here...before the heapSort
        //
        //
        //Loop starts - heap is sorted, min_index is set
        int update = 1;

        while(update)
        {

            //
            // Get the index of the minimum value(which should be the top of the Heap)
            // min_index is the index to the value vector in the Heap instance
            //
            unsigned int min_index = this->distHeap->getHeapTopIndex();

            //
            // The baseDistance is the distance from the source node to the current min node
            //
            int baseDistance = this->distHeap->getHeapMinValue();
            DebugLog->DebugMsg(Logger::DebugLevel::MED, "Node with shortest distance is: %d at %d\n", min_index, baseDistance);

            //
            // Set the final distance from the source node to the min node to the baseDistance
            // Added min_index to the back of the path vector
            // Set the node as vistied
            //
            path.push_back(min_index);
            nodeVisited.at(min_index) = 1;
            DebugLog->DebugMsg(Logger::DebugLevel::MED, "Setting distance of node %d to: %d\n", min_index, baseDistance);
            distances.at(min_index) = baseDistance;

            //
            // Search through each node with a path to the min node and add the base distance to there distances.
            // Only search those in the adjMatric row of the min node and that have not been visited.
            //
            update = 0;
            for(unsigned int nodeBIndex = 0; nodeBIndex < this->distHeap->value.size(); ++nodeBIndex)
            {
                DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "nodeBIndex = %d\n", nodeBIndex);
                int nodeDistance = this->getDistance(min_index, nodeBIndex);
                if(nodeVisited.at(nodeBIndex) == 1)
                {
                    DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "Node %d has already been visited\n", nodeBIndex);
                }
                else if (nodeDistance <= 0)
                {
                    DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "Node %d doesn't have a path to %d\n", nodeBIndex, min_index);
                }
                else
                { 
                    update = 1;
                    if(this->distHeap->getHeapValue(nodeBIndex) > baseDistance + nodeDistance)
                    {
                        DebugLog->DebugMsg(Logger::DebugLevel::HIGH,"Updating heap node %d with %d + %d\n", nodeBIndex, baseDistance, nodeDistance);
                        this->distHeap->setHeapValue(nodeBIndex, baseDistance + nodeDistance);
                        //this->distHeap->printHeap();
                    }
                }
            }

            DebugLog->DebugMsg(Logger::DebugLevel::LOW, "Pre Sort\n");
            //this->distHeap->printHeap();
            // Remove top node(min_index)
            this->distHeap->removeNode(0);
            this->distHeap->HeapSort();
            DebugLog->DebugMsg(Logger::DebugLevel::LOW, "Post Sort\n");
            //this->distHeap->printHeap();

        }
        for(unsigned int i = 0; i < distances.size(); i++)
            DebugLog->InfoMsg("Distance to %d is: %d\n", i, distances.at(i));



    }

    void pGraph::printDistMatrix()
    {
        for(unsigned int nodeA = 0; nodeA < this->nodeCount; nodeA++)
        {
            std::cout << nodeA << ": ";
            for(unsigned int nodeB = 0; nodeB < this->nodeCount; nodeB++)
                std::cout << this->getDistance(nodeA, nodeB) << " ";
            std::cout << std::endl;
        }

    }
    void pGraph::printPherMatrix()
    {
        for(unsigned int nodeA = 0; nodeA < this->nodeCount; nodeA++)
        {
            std::cout << nodeA << ": ";
            for(unsigned int nodeB = 0; nodeB < this->nodeCount; nodeB++)
                std::cout << this->getPheromone(nodeA, nodeB) << " ";
            std::cout << std::endl;
        }

    }

    int pGraph::getRandomNumber(int lower, int upper)
    {
        std::uniform_int_distribution<int> pher_distribution(lower, upper);

        return(pher_distribution(this->generator));
    }
}
