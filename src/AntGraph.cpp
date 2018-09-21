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
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
//#include <chrono>
#include <time>
#include <random>

#include "AntGraph.h"
#include "Logger.h"

namespace AntGraph
{
    void pGraph::saveGraph(const std::string &filename)
    {

        // Elements of a Graph

        std::ofstream outf;
        outf.open (filename, std::ofstream::out | std::ofstream::trunc);
        // Write the size of each element to the file

        std::string line;
        DebugLog->InfoMsg("Writing graph to: ");
        DebugLog->InfoMsg(filename);
        DebugLog->InfoMsg("File Version: %u\n", this->fileVersion);
        DebugLog->InfoMsg("Node Count: %u\n", this->nodeCount);
        DebugLog->InfoMsg("Trail Count: %u\n", this->trailCount);
        DebugLog->InfoMsg("Max Distance: %u\n", this->maxDistance);
        DebugLog->InfoMsg("Random Seed: %u\n", this->seed);

        outf << AG_VERSION << std::endl;
        outf << this->nodeCount << std::endl;
        outf << this->trailCount << std::endl;
        outf << this->maxDistance << std::endl;
        outf << this->seed << std::endl;

        outf << "Distance Matrix" << std::endl;
        for(unsigned int nodeA = 0; nodeA < this->nodeCount; nodeA++)
        {
            for(unsigned int nodeB = 0; nodeB < this->nodeCount; nodeB++)
                outf << " " << this->getDistance(nodeA, nodeB);
            outf << std::endl;
        }

        outf << "Pheromone Matrix" << std::endl;
        for(unsigned int nodeA = 0; nodeA < this->nodeCount; nodeA++)
        {
            for(unsigned int nodeB = 0; nodeB < this->nodeCount; nodeB++)
                outf << " " << this->getPheromone(nodeA, nodeB);
            outf << std::endl;
        }
        outf.close();
            
    }
    void pGraph::loadGraph(const std::string &filename)
    {
        std::ifstream inf;
        std::string line;
        unsigned int val;

        std::vector<unsigned int> us_int_vals;
        inf.open(filename, std::ifstream::in);

        for(int i = 0; i < 5; i++)
        {
            inf >> val;
            us_int_vals.push_back(val);
        }
        std::getline(inf, line);

        DebugLog->InfoMsg("Reading graph from: %s\n", filename);
        this->fileVersion = us_int_vals.at(0);
        DebugLog->InfoMsg("File Version: %u\n", this->fileVersion);
        this->nodeCount = us_int_vals.at(1);
        DebugLog->InfoMsg("Node Count: %u\n", this->nodeCount);
        this->trailCount = us_int_vals.at(2);
        DebugLog->InfoMsg("Trail Count: %u\n", this->trailCount);
        this->maxDistance = us_int_vals.at(3);
        DebugLog->InfoMsg("Max Distance: %u\n", this->maxDistance);
        this->seed = us_int_vals.at(4);
        DebugLog->InfoMsg("Random Seed: %u\n", this->seed);


        // Resize and initialize Pheromone Matrix
        this->distAdjMatrix.resize(nodeCount*nodeCount);
        std::fill(this->distAdjMatrix.begin(), this->distAdjMatrix.end(), 0);
        
        //
        // Get Distance Matrix Header
        //
        std::getline(inf, line);

        for(unsigned int nodeA = 0; nodeA < this->nodeCount; nodeA++)
        {
            std::getline(inf, line);

            std::stringstream lineStream(line);
            unsigned int nodeB = 0;
            while(lineStream >> val)
            {
                this->distAdjMatrix.at(this->getMatrixIndex(nodeA, nodeB)) = val;
                nodeB++;
            }
        }

        // Resize and initialize Pheromone Matrix
        this->pherAdjMatrix.resize(nodeCount*nodeCount);
        std::fill(this->pherAdjMatrix.begin(), this->pherAdjMatrix.end(), 0);

        //
        // Get Pheromone Matrix Header
        //
        std::getline(inf, line);

        for(unsigned int nodeA = 0; nodeA < this->nodeCount; nodeA++)
        {
            std::getline(inf, line);

            std::stringstream lineStream(line);
            unsigned int nodeB = 0;
            while(lineStream >> val)
            {
                this->pherAdjMatrix.at(this->getMatrixIndex(nodeA, nodeB)) = val;
                nodeB++;
            }
        }

        inf.close();
    }

    unsigned int pGraph::chooseNextNode(unsigned int srcNode, unsigned int prevNode)
    {
        std::vector<unsigned int> *nodes;

        DebugLog->DebugMsg(Logger::DebugLevel::MED, "Finding next node from %u\n", srcNode);
        
        // Get the adjancent nodes
        nodes = this->getAdjNodes(srcNode);
        if(nodes->size() == 0)
            return(0);

        // Remove previous node
        DebugLog->DebugMsg(Logger::DebugLevel::MED, "Removing Previous\nNodes(%d) to %u:\n", nodes->size(), srcNode);
        for(unsigned int it = 0; it < nodes->size(); it++)
        {
            unsigned int node = nodes->at(it);
            DebugLog->DebugMsg(Logger::DebugLevel::MED, "Checking node: %u\n", node);
            if(node == prevNode)
                nodes->erase(nodes->begin() + it);
        }
        DebugLog->DebugMsg(Logger::DebugLevel::MED, "Finished checking node\n");

        //
        // If the node has only 1 adjacent node(which must be the previous node),
        // go back to the previous node and try again.
        //
        if(nodes->size() <= 1)
        {
            DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "Dead end found at %u(%u)\n", srcNode, nodes->at(0));
            return(prevNode);
        }

        // Get pheromone numbers to generate distribution
        std::vector<int> *pheromones = this->getPheromones(srcNode, nodes);

        // Normalize to 100 and gen random number between 0 and 100
        int total_pheromone = 0;
        for(unsigned int it = 0; it < pheromones->size(); ++it)
        {
            DebugLog->DebugMsg(Logger::DebugLevel::MED, "%u\tAdding %d\tTotal = %d\n", nodes->at(it), pheromones->at(it), total_pheromone);
            total_pheromone += pheromones->at(it);
        }
        DebugLog->DebugMsg(Logger::DebugLevel::MED, "Total pheromone: %d\n", total_pheromone);

        int weighted_pheromone = this->getRandomNumber(0, total_pheromone);

        DebugLog->DebugMsg(Logger::DebugLevel::MED, "Weighted Pheromone = %d\n", weighted_pheromone);

        total_pheromone = 0;
        unsigned int index = 0;
        std::vector<int>::iterator it = pheromones->begin(); 
        do {
            total_pheromone += pheromones->at(index);
            index++;
        } while(total_pheromone < weighted_pheromone);

        return((unsigned int) nodes->at(index-1));
    }

    std::vector<unsigned int> *pGraph::getAdjNodes(unsigned int node)
    {
        std::vector<unsigned int> *nodes = new std::vector<unsigned int>;
        DebugLog->DebugMsg(Logger::DebugLevel::MED, "Adjacent nodes: ");
        for(unsigned int nodeB = 0; nodeB < this->nodeCount; nodeB++)
        {
            if(this->getDistance(node, nodeB) > 0)
            {
                nodes->push_back(nodeB);
                DebugLog->DebugMsg(Logger::DebugLevel::MED, "%u ", nodeB);
            }
        }
        DebugLog->DebugMsg(Logger::DebugLevel::MED, "\n");
        return(nodes);
    }

    std::vector<int>* pGraph::getPheromones(unsigned int src_node, std::vector<unsigned  int> *node_list)
    {
        std::vector<int> *pheromone_list =new std::vector<int>();
        for(std::vector<unsigned int>::iterator it = node_list->begin(); it != node_list->end(); ++it)
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
        this->fileVersion = AG_VERSION;

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
        std::uniform_int_distribution gen (this->seed);
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
            DebugLog->DebugMsg(Logger::DebugLevel::MED, "Adding trail # %d\n", i);

            nodeA = this->getRandomNumber(0, (this->nodeCount-1));
            do {
                nodeB = this->getRandomNumber(0, (this->nodeCount-1));
                loops++;
            } while(nodeB == nodeA);
            loops--;

            DebugLog->DebugMsg(Logger::DebugLevel::LOW, "Node 1: %d\n", nodeA); 
            DebugLog->DebugMsg(Logger::DebugLevel::LOW, "Node 2: %d\n", nodeB); 

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
        std::cout << "Distance Matrix" << std::endl;
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
        std::cout << "Pheromone Matrix" << std::endl;
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
