# Makefile for Rapid Binary Star Evolution program
CC      := gcc
LD      := gcc
PROGRAM := binary_c_api
MAKE    := /usr/bin/make
LIBS 	  := -lbinary_c -lm -lc
C_SRC   := binary_c_api.c
OBJECTS := $(C_SRC:.c=.o)
OBJ_FLAGS := -c
OPTIONS := -fpic
SO_FLAGS := -shared -o
SO_NAME := libbinary_c_api.so

# To create python shared library
PY_EXEC := python
PY_SETUP := setup.py
PY_OPTIONS := build_ext --inplace

all: $(OBJECTS)
	$(CC) $(C_SRC) $(OBJ_FLAGS) $(INCDIRS) $(LIBS) $(OPTIONS)
	$(CC) $(SO_FLAGS) $(SO_NAME) $(OBJECTS)
	$(PY_EXEC) $(PY_SETUP) $(PY_OPTIONS)

clean:
	rm -f *.o *.so
