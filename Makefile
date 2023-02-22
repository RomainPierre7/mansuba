WIDTH ?= 8
HEIGHT ?= 8
MAX_PLAYERS ?= 2
HEADER_DIR = src
MANSUBA_FLAGS = -DWIDTH=$(WIDTH) -DHEIGHT=$(HEIGHT) -DMAX_PLAYERS=$(MAX_PLAYERS)
CFLAGS = -Wall -Wextra -std=c99 -g3 $(MANSUBA_FLAGS) -I $(HEADER_DIR)
LINKFLAGS = -lm
SOURCES = src/geometry.c src/world.c src/neighbors.c src/play.c src/set.c src/view.c src/game.c
OBJS = $(patsubst src/%.c, %.o, $(SOURCES))
MAINS_SRC = src/main.c tst/test.c

all: project
	
%.o: 
	gcc -c $(CFLAGS) $<

project: main.o $(OBJS) .depend
	gcc $(CFLAGS) $< $(OBJS) -o $@ $(LINKFLAGS)

test: test.o $(TEST_OBJ) $(OBJS) .depend
	gcc $(CFLAGS) $< $(TEST_OBJ) $(OBJS) -o $@ $(LINKFLAGS)
	./test
	rm test

pdf:
	pdflatex ./tex/rapport.tex
	evince rapport.pdf

.depend: $(MAINS_SRC) $(SOURCES) $(TEST_SRC) 
	gcc -MM $(CFLAGS) $(MAINS_SRC) $(SOURCES) $(TEST_SRC) > .depend

cleantmp:
	rm -f *~ *.aux *.log *.dvi *.toc *.out

clean: cleantmp
	rm -f *.o ./*/*.o ./*/*.h.gch project test rapport.[lta]*

-include .depend
