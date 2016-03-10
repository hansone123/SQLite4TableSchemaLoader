include config.mk

SRC := $(wildcard $(TOP)/src/*.cc)
HDR := $(wildcard $(TOP)/src/*.h)

OBJ := $(patsubst %.cc,%.o,$(wildcard $(TOP)/src/*.cc))
OBJ := $(patsubst $(TOP)/src/%, %,$(OBJ))
	
TEST := $(patsubst %.cc,%$(EXE),$(wildcard $(TOP)/test/*.cc))
TEST := $(patsubst $(TOP)/test/%, %,$(TEST))

all: check libSchemaLoader.a test 
	
check:
	echo 'HDR: $(HDR)'
	echo 'SRC: $(SRC)'
	echo 'OBJ: $(OBJ)'
	echo 'TEST: $(TEST)'
	
libSchemaLoader.a: $(OBJ)
	$(AR) libSchemaLoader.a $(TOP)/obj/*.o
	$(RANLIB) libSchemaLoader.a
%.o: $(TOP)/src/%.cc
	g++ -o $(TOP)/obj/$@ -c $<


test: $(TEST)
	
TestLoader$(EXE):
	g++ -o $(TOP)/test/testLoader$(EXE) $(TOP)/test/TestLoader.cc $(TOP)/include/* -lleveldb -lpthread
	
clean:
	rm $(TOP)/test/*$(EXE)
	rm $(TOP)/obj/*
testcmd:
	echo $(OBJ)