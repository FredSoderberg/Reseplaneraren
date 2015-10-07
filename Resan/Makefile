CCOPTS=-g -ggdb -Wall -Werror
COMPIL= clang

CODEFILES=$(shell ls *.c *.h)

all: doc test

main: main.c network.o util.o
	$(COMPIL) $(CCOPTS) network.o graph.o util.o list.o time.o main.c -o main

doc: $(CODEFILES)
	doxygen travel_planner.doxy

network.o: graph.o network.c network.h
	$(COMPIL) $(CCOPTS) -c network.c

graph.o: graph.c graph.h list.o util.o
	$(COMPIL) $(CCOPTS) -c graph.c

list.o: list.c list.h util.o graph.h time.o network.h
	$(COMPIL) $(CCOPTS) -c list.c

time.o: time.c time.h
	$(COMPIL) $(CCOPTS) -c time.c

util.o: util.h util.c
	$(COMPIL) $(CCOPTS) -c util.c

test: main list_test graph_test network_test
.PHONY: test

list_test: list_test.c list.o util.o
	$(COMPIL) $(CCOPTS) -lcunit list_test.c list.o util.o -o list_test
	./list_test
.PHONY: list_test

graph_test: graph_test.c graph.o list.o
	$(COMPIL) $(CCOPTS) -lcunit graph_test.c graph.o list.o util.o -o graph_test
	./graph_test
.PHONY: graph_test

network_test: network_test.c network.o graph.o list.o
	$(COMPIL) $(CCOPTS) -lcunit network_test.c network.o graph.o list.o util.o -o network_test
	./network_test
.PHONY: network_test

clean:
	rm -f *.o
	rm -f main
	rm -f list_test
	rm -f graph_test
	rm -f network_test

.PHONY: clean
