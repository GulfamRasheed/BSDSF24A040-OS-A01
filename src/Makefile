CC = gcc
CFLAGS = -I../include -Wall
PIC_FLAG = -fPIC
OBJ_DIR = ../obj
BIN_DIR = ../bin
LIB_DIR = ../lib

LIB_OBJECTS = $(OBJ_DIR)/mystrfunctions.o $(OBJ_DIR)/myfilefunctions.o
STATIC_LIB = $(LIB_DIR)/libmyutils.a
DYNAMIC_LIB = $(LIB_DIR)/libmyutils.so

STATIC_TARGET = $(BIN_DIR)/client_static
DYNAMIC_TARGET = $(BIN_DIR)/client_dynamic

all: $(STATIC_TARGET) $(DYNAMIC_TARGET)

$(STATIC_TARGET): $(OBJ_DIR)/main.o $(STATIC_LIB)
	$(CC) $(OBJ_DIR)/main.o -L$(LIB_DIR) -lmyutils -o $(STATIC_TARGET)

$(STATIC_LIB): $(LIB_OBJECTS)
	ar rcs $(STATIC_LIB) $(LIB_OBJECTS)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DYNAMIC_TARGET): $(OBJ_DIR)/main.o $(DYNAMIC_LIB)
	$(CC) $(OBJ_DIR)/main.o -L$(LIB_DIR) -lmyutils -o $(DYNAMIC_TARGET)

$(DYNAMIC_LIB): $(LIB_OBJECTS)
	$(CC) -shared -o $(DYNAMIC_LIB) $(LIB_OBJECTS)

pic_objects:
	$(CC) $(PIC_FLAG) $(CFLAGS) -c mystrfunctions.c -o $(OBJ_DIR)/mystrfunctions.o
	$(CC) $(PIC_FLAG) $(CFLAGS) -c myfilefunctions.c -o $(OBJ_DIR)/myfilefunctions.o

clean:
	rm -f $(OBJ_DIR)/*.o $(STATIC_LIB) $(DYNAMIC_LIB) $(STATIC_TARGET) $(DYNAMIC_TARGET)
