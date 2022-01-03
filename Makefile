CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -O3

SINGLE_HEADERS = 
HEADERS = helpers.h requests.h parson.h buffer.h commands.h client.h \
			prompts.h commands_requests.h commands_helpers.h commands_follow.h
SINGLE_SOURCES = 
SOURCES = $(SINGLE_SOURCES) $(HEADERS:.h=.c)
OBJECTS = $(SOURCES:.c=.o)
EXEC = client


.PHONY: default build run clean

default: build

build: $(EXEC)

%.o: %.c $(HEADERS) $(SINGLE_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

run: default
	./$(EXEC)

clean:
	rm -f $(EXEC)
	rm -f $(OBJECTS)