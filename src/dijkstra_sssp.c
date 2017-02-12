/*
 *  dykstra_sssp.c
 *
 *  Jason Graalum
 *  November 15, 2016
 *  New Beginnings - Theory II
 *  Programming Assignment #4
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "Algorithms.h"
#include "Utilities.h"
#include "DirectedGraph.h"
#include "Heap.h"

void dijkstra_sssp(struct DirectedGraph *dg, unsigned int source)
{
   struct Heap *minHeap;

//   struct Queue *ssspQueue;
//   struct queueNode *qNode;
//   struct queueNode *current;

   unsigned int *distances;
   unsigned int *prev;
   int *visited;

   int inf = ~0x0;

   int v_cnt = dg->vertex_cnt;

   minHeap = malloc(sizeof(struct Heap));
   minHeap->size = 0;
   minHeap->heap_index = malloc(sizeof(int)*v_cnt);
   minHeap->key = malloc(sizeof(unsigned int)*v_cnt);
   minHeap->data = malloc(sizeof(heapNode*)*v_cnt);

   //ssspQueue = malloc(sizeof(struct Queue));
   //ssspQueue->head = NULL;
   //ssspQueue->tail = NULL;

   distances = malloc(sizeof(unsigned int)*v_cnt);
   prev = malloc(sizeof(int)*v_cnt);
   visited = malloc(sizeof(int)*v_cnt);

   // Test Heap
/*   for(int i= 0; i < v_cnt; i++)
   {
      minHeap->data[i] = dg->vertex[i];
      minHeap->heap_index[i] = i;
      visited[i] = 0;
      prev[i] = 0;
      minHeap->key[i] = rand() % 100; 
      minHeap->size++;
   }

   heapify(v_cnt,minHeap);
   printHeap(minHeap);
   */

   //
   //
   for(int i= 0; i < v_cnt; i++)
   {
      minHeap->data[i] = dg->vertex[i];
      minHeap->heap_index[i] = i;
      visited[i] = 0;
      prev[i] = 0;
      if(i == (source-1))
         minHeap->key[i] = 0;
      else
         minHeap->key[i] = inf;
      minHeap->size++;
      prev[i] = source-1;
   }

   heapify(minHeap->size,minHeap);

   while((minHeap->size > 0) && (minHeap->key[minHeap->heap_index[0]] != inf))
   {
      int min_index = minHeap->heap_index[0];
      unsigned int base_dist = minHeap->key[min_index];
      //printf("Checking min_index %d at %u\n", min_index+1, minHeap->key[min_index]);

      // iterate through neighbors of the node on the top of the minHeap
      // Use the adjMatrix of the graph
      // Get the edges from the adjMatrix
      for(int i = 0; i < v_cnt; i++)
      {
         unsigned int edge_value = dg->adj->v[min_index*v_cnt + i];

         if((edge_value > 0) && (visited[i] == 0))
         {
            //printf("Found a connected node at %d\n", i+1);
            unsigned int new_dist = base_dist + edge_value;
            if(new_dist < minHeap->key[i])
            {
               //printf("Updating distance to node %d from %u(%u) to %u\n", i+1,distances[i],minHeap->key[minHeap->heap_index[i]],new_dist);
               minHeap->key[i] = new_dist;
               distances[i] = new_dist;
               prev[i] = min_index;
            }
         }

      }
      visited[min_index] = 1;
      minHeap->heap_index[0] = minHeap->heap_index[(minHeap->size)-1];
      minHeap->size--;
      heapify(minHeap->size,minHeap);
   }

   printf("Distances from node %d:\n", source);
   printf("=======================\n");
   for(int i = 0; i < v_cnt; i++)
   {
      if((i+1) != source)
      {
         if(minHeap->key[i] == inf)
            printf("Node:%d\tDistance = INF\tNo Path to %u\n",i+1, i+1);
         else
         {
            printf("Node:%d\tDistance = %u\tPath = %u",i+1,minHeap->key[i],i+1);
            int j = i;
            while((prev[j]+1) != source)
            {
               printf("<-%u",prev[j]+1);
               j = prev[j];
            }
            printf("<-%u\n", source);
         }
      }
   }

}
