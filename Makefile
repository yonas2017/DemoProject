#
# 'make'        build executable file targets
# 'make clean'  removes all .o and executable files
#

#
# Defines
#
CC = gcc
#CFLAGS = -Wall -g
INC = -Iinclude

PATH_CLIENT  = bin/build/client
PATH_SERVER  = bin/build/server

BIN_CLIENT  = bin/client
BIN_SERVER  = bin/server

#
# Get all client sources from client dir
# Create client objects into client build path
#
SRCS_CLIENT = $(wildcard client/*.c)
OBJS_CLIENT = $(patsubst client/%.c, $(PATH_CLIENT)/%.o, $(SRCS_CLIENT) )

#
# Get all server sources from server dir
# Create server objects into server build path
#
SRCS_SERVER = $(wildcard server/*.c)
OBJS_SERVER = $(patsubst server/%.c, $(PATH_SERVER)/%.o, $(SRCS_SERVER) )

#
# Make All Targets
#
all: echo $(BIN_CLIENT) $(BIN_SERVER)

#
# Echo Srcs and Objs
#
echo:
	@echo SRCS_CLIENT:$(SRCS_CLIENT)
	@echo OBJS_CLIENT:$(OBJS_CLIENT)

	@echo SRCS_SERVER:$(SRCS_SERVER)
	@echo OBJS_SERVER:$(OBJS_SERVER)

#
# Build client code to $(BIN_CLIENT) executable file
#
$(BIN_CLIENT): $(OBJS_CLIENT)
	@mkdir -p $(PATH_CLIENT)
	$(CC) $(CFLAGS) $(INC) -o $@ $^

$(PATH_CLIENT)/%.o: client/%.c
	@mkdir -p $(PATH_CLIENT)
	$(CC) $(CFLAGS) $(INC) -o $@  -c $<

#
# Build server code to $(BIN_SERVER) executable file
#
$(BIN_SERVER): $(OBJS_SERVER)
	@mkdir -p $(PATH_SERVER)
	$(CC) $(CFLAGS) $(INC) -o $@ $^

$(PATH_SERVER)/%.o: server/%.c
	@mkdir -p $(PATH_SERVER)
	$(CC) $(CFLAGS) $(INC) -o $@  -c $<

#
# Target for cleaning bin dir.
#
.PHONY: clean
clean:
	$(RM) -r bin/*
