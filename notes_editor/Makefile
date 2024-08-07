CXX=g++
CXXFLAGS = -Wall -pedantic -std=c++14
HEADERS = $(wildcard src/*.h)
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

.PHONY: all
all: compile doc

.PHONY: run
run: compile
	./biarkvia

.PHONY: compile
compile: biarkvia

biarkvia: build/main
	cp $< $@

build/main: $(OBJECTS)
	mkdir -p $(@D)
	g++ $^ -o $@

build/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(CXXFLAGS) $< -c -o $@

doc: Doxyfile $(HEADERS)
	doxygen Doxyfile

.PHONY: clean
clean:
	rm -rf biarkvia build/ doc/ 2>/dev/null

deps:
	$(CXX) -MM src/*cpp > Makefile.d

-include Makefile.d

directory.o: src/directory.cpp src/text.h src/helpers.h src/directory.h \
 src/note.h src/todolist.h src/shoppinglist.h
display.o: src/display.cpp src/display.h src/note.h src/text.h \
 src/helpers.h src/directory.h src/todolist.h src/shoppinglist.h
helpers.o: src/helpers.cpp src/helpers.h src/directory.h src/note.h
main.o: src/main.cpp src/notepadapp.h src/text.h src/helpers.h \
 src/directory.h src/note.h src/todolist.h src/shoppinglist.h \
 src/display.h
note.o: src/note.cpp src/note.h
notepadapp.o: src/notepadapp.cpp src/notepadapp.h src/text.h \
 src/helpers.h src/directory.h src/note.h src/todolist.h \
 src/shoppinglist.h src/display.h
shoppinglist.o: src/shoppinglist.cpp src/shoppinglist.h src/note.h \
 src/helpers.h src/directory.h src/display.h
text.o: src/text.cpp src/text.h src/helpers.h src/directory.h src/note.h \
 src/display.h
todolist.o: src/todolist.cpp src/todolist.h src/note.h src/display.h \
 src/notepadapp.h src/text.h src/helpers.h src/directory.h \
 src/shoppinglist.h
