// 
// Heap Class
//
//  

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

namespace AntGraph {


    class Heap {
        private:
            std::vector<unsigned int> value;
            std::vector<unsigned int> key;

            int direction;  // 0 = min, 1 = max

        public:
            Heap();
            void setDirection(unsigned int);
            void HeapSort();
            void addValue(int);
            int removeKey(unsigned int);
            unsigned int getHeapSize();
            unsigned int getHeapTopValue();
            void printHeap();
    };

}
