SRCS = gifenc.c main.c particle.c simulation_gif.c simulator.c vector.c event.c
OBJS = $(SRCS:%.c=%.o)
TARGET = simulate
CC = gcc
CFLAGS = -std=c99 --pedantic -Wall -Wextra -Wmissing-prototypes -g
LDFLAGS = -lm

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

maxwell: maxwell.o particle.o simulator.o vector.o event.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET) maxwell maxwell.o

run:
	./simulate

gifenc.o: gifenc.c gifenc.h
main.o: main.c color.h vector.h particle.h simulator.h simulation_gif.h \
  gifenc.h
particle.o: particle.c color.h vector.h particle.h
simulation_gif.o: simulation_gif.c vector.h particle.h color.h \
  simulator.h gifenc.h simulation_gif.h
simulator.o: simulator.c particle.h color.h vector.h simulator.h
vector.o: vector.c vector.h
event.o: event.c event.h
maxwell.o: maxwell.c vector.h particle.h color.h simulator.h event.h