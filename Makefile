LWD = $(shell pwd)
OBJ_DIR = $(LWD)/obj
SRC_DIR = $(LWD)/src
DATA_DIR = $(LWD)/data
BIN_DIR = $(LWD)/bin
INC_DIR = $(LWD)/inc
COMMON_DIR = $(LWD)/common
TEST_DIR = $(LWD)/test


CC = g++
#CFLAGS = -std=c++11 -stdlib=libstdc++ -g -Wall -I$(INC_DIR)
CFLAGS = -std=c++0x -stdlib=libstdc++ -g -Wall -I$(INC_DIR)

COMMON_OBJS = 
ANT_SRC = $(SRC_DIR)/Ant.cpp
PGRAPH_SRC = $(SRC_DIR)/AntGraph.cpp $(SRC_DIR)/Heap.cpp $(SRC_DIR)/Logger.cpp $(SRC_DIR)/Ant.cpp
PGRAPH_INC = $(INC_DIR)/AntGraph.h $(INC_DIR)/Logger.h $(INC_DIR)/Ant.h

all: testPGraph

testPGraph: $(BIN_DIR)/testPGraph

$(BIN_DIR)/testPGraph: $(TEST_DIR)/testPGraph.cpp $(PGRAPH_SRC) $(PGRAPH_INC) 
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(TEST_DIR)/testPGraph.cpp $(PGRAPH_SRC) -o $(BIN_DIR)/testPGraph 

clean:
	rm -f $(BIN_DIR)/* $(OBJ_DIR)/*.o $(OBJ_DIR)/*.s $(OBJ_DIR)/*.i core
