I_FLAG := -I./include/ -I./lib/stack/include/ -I./lib/ -I./lib/calc/ #-I./inc/

CC:= g++
CFLAGS := $(I_FLAG) -g -fsanitize=address # -Wall

SRC_DIR  := ./src/
LIB_DIR  := ./lib/
SRC_STK  := ./lib/stack/src/
READ_DIR := ./lib/calc/

SOURCES 	:= $(SRC_DIR)matan_killer.cpp $(SRC_DIR)matan_killer_debug.cpp $(SRC_DIR)matan_killer_f.cpp $(SRC_DIR)matan_killer_modes.cpp $(LIB_DIR)tree_funcs.cpp  $(LIB_DIR)text_funcs.cpp $(READ_DIR)calc_f.cpp  #$(SRC_STK)stack_funcs.cpp $(SRC_STK)debug_funcs.cpp 
EXECUTABLE  := matan_killer

all:
	mkdir -p graph_dumps
	touch expression.input
	$(CC) $(SOURCES) $(CFLAGS) -o $(EXECUTABLE)