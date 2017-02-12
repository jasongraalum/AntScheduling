/*
 *  Heap.cpp
 *
 *  Jason Graalum
 *  January 17, 2017
 *  New Beginnings - Theory III
 *
 *  Generic support for standard data structures used in a variety of programs
 *
 *  heap
 *
 *  The same data element(queueNode) is used for all structures. The queueNode structure
 *  include a void ptr to another structure that can be defined within the main program
 *  to hold program specific payloads
 *
 */
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "AntGraph.h"

namespace AntGraph 
{

    Heap::Heap()
    {
        //std::cout << "Default constructor for Heap" << std::endl;
    }

    Heap::Heap(Logger::Logger *Log)
    {
        this->DebugLog = Log;
        //std::cout << "Default constructor for Heap" << std::endl;
    }

    void Heap::addValue(int value)
    {
        this->value.push_back(value);
        unsigned int key = this->value.size();
        this->key.push_back(key-1);
    }

    void Heap::updateValue(unsigned int key, int value)
    {
        DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "Updating %u with %d\n", key, value);
        (this->value).at(this->key.at(key)) = value;

    }

    void Heap::setHeapValue(unsigned int index, int value)
    {
        this->value.at(index) = value;
    }

    int Heap::removeNode(unsigned int key)
    {
        int value = (this->key).at(key);
        DebugLog->DebugMsg(Logger::DebugLevel::HIGH, "Removing key %u\n", key);
        (this->key).erase(this->key.begin() + key);
        return(value);
    }

    unsigned int Heap::getHeapSize()
    {
        return((this->key).size());
    }


    int Heap::getHeapValue(unsigned int index)
    {
        return(this->value.at(index));
    }

    int Heap::getHeapMinValue()
    {
        return(this->value.at(this->key.at(0)));
    }

    unsigned int Heap::getHeapTopIndex()
    {
        return(this->key.at(0));
    }

    void Heap::HeapSort()
    {
        //Psuedo Code
        // Input: n - size of array
        //        data[0:n-1] - unsorted data
        // Output: sorted data[0:n-1]
        // Shift all array indexes down by 1.
        //
        // for i : n/2 to 1
        //   k = i
        //   v = data[k]
        //   heaped = 0
        //   while !heaped && 2*k < n
        //     j = 2 * k
        //     if j < n
        //       if data[j-1] < data[j]
        //         j++
        //     if v >= data[j-1]
        //       heaped = 1
        //     else
        //       data[k-1] = data[j-1]
        //       k = j
        //    data[k-1] = v
        //
        //
        // n, i, j, k are indexed starting at 1.
        // v is indexed starting at 0
        //
        unsigned int n = this->key.size();
        unsigned int i, j, k;
        unsigned int v;
        int heaped;


        for(i = n/2; i > 0; i--)
        {
            k = i;
            v = this->key.at(k-1);
            heaped = 0;
            //std::cout << "i,j,k,n,v = " << i <<  "," << j << "," << k << "," << n << "," << v << std::endl;

            while(!heaped && (2*k <= n))
            {
                j = 2 * k;
                //std::cout << "i,j,k,n,v = " << i <<  "," << j << "," << k << "," << n << "," << v << std::endl;
                if(j < n)
                {
                    if(this->value.at(this->key.at(j-1)) > this->value.at(this->key.at(j)))
                    {
                    //std::cout << "i,j,k,n,v = " << i <<  "," << j << "," << k << "," << n << "," << v << std::endl;
                        j++;
                    }
                }
                if(this->value.at(v) <= this->value.at(this->key.at(j-1)))
                {
                    heaped = 1;
                }
                else
                {
                    this->key.at(k-1) = this->key.at(j-1);
                    k = j;
                }
                //std::cout << "i,j,k,n,v = " << i <<  "," << j << "," << k << "," << n << "," << v << std::endl;
            }
            this->key.at(k-1) = v;
        }
    }

    void Heap::printHeap()
    {
        //for(std::vector<unsigned int>::iterator it = this->key.begin(); it != this->key.end(); it++) 
        std::cout << "Current Heap: " << this->key.size() << " elements." << std::endl;
        for(unsigned int it = 0; it < this->key.size(); it++)
        {
            printf("\tvalue = %d\tkey = %u\n", this->value.at(this->key.at(it)), this->key.at(it));
        }
        std::cout << std::endl;
    }
}

