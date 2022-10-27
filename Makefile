CC = gcc
SRCS = code/main.c code/main.h
TARGET = ./zdb
BIN = /usr/bin/

all: $(SRCS)
	$(CC) $(SRCS) -o $(TARGET)
install: $(TARGET)
	cp $(TARGET) $(BIN)
clean: $(TARGET)
	rm $(TARGET)
