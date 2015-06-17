CC = g++
CFLAGS = -std=c++11 `sdl2-config --cflags`
LIBS = `sdl2-config --libs` -lSDL2_image -lSDL2_mixer

EXE_NAME = e
EXE = -o $(EXE_NAME)

SOURCES  = main.cpp
SOURCES += lib/data.cpp
SOURCES += lib/graphics.cpp
SOURCES += lib/keyboard.cpp
SOURCES += src/game.cpp

SOURCES += src/base/base.cpp
SOURCES += src/base/character.cpp

SOURCES += src/screen/screen.cpp

SOURCES += src/terminals/components/display.cpp
SOURCES += src/terminals/components/shell.cpp
SOURCES += src/terminals/components/program.cpp
SOURCES += src/terminals/components/memory.cpp
SOURCES += src/terminals/terminal.cpp

SOURCES += src/states/state.cpp
SOURCES += src/states/play.cpp

OBJECTS = $(wildcard *.o)

test:
	make build;
	make run;

build:
	g++ -c $(SOURCES) $(CFLAGS);
	g++ $(OBJECTS) $(LIBS) $(EXE);

run:
	./$(EXE_NAME);

clean:
	-rm $(OBJECTS);
	-rm $(EXE_NAME);
