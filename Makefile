CC = g++

CFLAGS = -g -Wall

all: renderer.o
	$(CC) $(CFLAGS) -o rendererP renderer.cpp matrix.cpp matrix.h vec3.h

clean:
	$(RM) renderer