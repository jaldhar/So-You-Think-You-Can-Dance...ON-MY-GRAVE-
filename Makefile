TARGET=sytycdomg
CXXFLAGS=-std=c++0x -pedantic -O2 -g -Wall -Wextra -Wcast-qual -Wformat=2 $(shell ncurses5-config --cflags)
LDFLAGS=$(shell ncurses5-config --libs)
PREFIX=/usr/local

OBJECTS= sytycdomg.o \
         armament.o \
         armor.o \
         combat.o \
         door.o \
         game.o \
         item.o \
         monster.o \
         player.o \
         potion.o \
         room.o \
         shield.o \
         tile.o \
         treasure.o \
         view.o \
         weapon.o \
         world.o
      
$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS) $(LDFLAGS)
      
install: $(TARGET)
	install -oroot -groot -m0755 -s $(TARGET) $(PREFIX)/games

memcheck: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

clean:
	-rm sytycdomg $(OBJECTS)
