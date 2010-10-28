CXXFLAGS=-std=c++0x -pedantic -O2 -g -Wall -Wextra $(shell ncurses5-config --cflags)
LDFLAGS=$(shell ncurses5-config --libs) -lstdc++
PREFIX=/usr/local

OBJECTS= sytycdomg.o \
         armor.o \
         combat.o \
         corridor.o \
         cursesview.o \
         game.o \
         item.o \
         monster.o \
         player.o \
         potion.o \
         room.o \
         shield.o \
         tile.o \
         treasure.o \
         weapon.o \
         world.o
      
sytycdomg: $(OBJECTS)

install: sytycdomg
	install -oroot -groot -m0755 -s  sytycdomg $(PREFIX)/games

clean:
	-rm sytycdomg $(OBJECTS)
