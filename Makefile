SHELL    = /bin/sh
CC       = gcc
FLAGS    = -I. -fPIC -pedantic -Wall -Wextra -march=native
CFLAGS   = 
HEADERS  = qjson.h

LIB      = libqjson.a
LIB_OBJ  = qjson.o
SLIB     = libqjson.so
SLIB_OBJ = qjson.o
PGM      = qjson
PGM_OBJ  = qjson.o main.o

all: $(PGM) $(LIB) $(SLIB)

%.o: %.c $(HEADERS)
	@echo "compiling $<"
	@$(CC) -c -o $@ $< $(FLAGS)

$(PGM): $(PGM_OBJ)
	@echo "making $@ with $^"
	@$(CC) -o $@ $^ $(CFLAGS) 

$(LIB): $(LIB_OBJ)
	@echo "making $@ with $^"
	@ar -cr $(LIB) $^

$(SLIB): $(SLIB_OBJ)
	@echo "making $@ with $^"
	@$(CC) --shared -o $@ $^ $(CFLAGS)

test: 
	@cd tests && ./test

.PHONY: clean

clean:
	@echo "cleaning up" 
	@rm -f $(PGM) $(PGM_OBJ) $(LIB) $(LIB_OBJ) $(SLIB) $(SLIB_OBJ)

