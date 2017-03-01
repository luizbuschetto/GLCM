CC = g++

CFLAGS = -std=c++0x -lpthread

all: glcm_parameters

glcm_parameters: glcm_parameters.cpp
	$(CC) `pkg-config --cflags opencv` glcm_parameters.cpp -o glcm `pkg-config --libs opencv` $(CFLAGS)

clean:
	rm -f glcm_parameters
