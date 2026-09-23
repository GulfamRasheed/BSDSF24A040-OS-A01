CC = gcc
CFLAGS = -I../include -Wall
OBJ_DIR = ../obj
BIN_DIR = ../bin
LIB_DIR = ../lib

LIB_OBJECTS = $(OBJ_DIR)/mystrfunctions.o $(OBJ_DIR)/myfilefunctions.o
STATIC_LIB = $(LIB_DIR)/libmyutils.a
TARGET = $(BIN_DIR)/client_static

all: $(TARGET)

$(TARGET): $(OBJ_DIR)/main.o $(STATIC_LIB)
	$(CC) $(OBJ_DIR)/main.o -L$(LIB_DIR) -lmyutils -o $(TARGET)

$(STATIC_LIB): $(LIB_OBJECTS)
	ar rcs $(STATIC_LIB) $(LIB_OBJECTS)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(STATIC_LIB) $(TARGET)
