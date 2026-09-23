CC = gcc
CFLAGS = -I../include -Wall
OBJ_DIR = ../obj
BIN_DIR = ../bin

OBJECTS = $(OBJ_DIR)/mystrfunctions.o $(OBJ_DIR)/myfilefunctions.o $(OBJ_DIR)/main.o
TARGET = $(BIN_DIR)/client

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
