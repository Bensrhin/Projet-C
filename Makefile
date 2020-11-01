# Repertoires du projet

BIN_DIR = bin
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
OBJPROF_DIR = obj-prof

# Options de compilation/édition des liens

CC = clang
LD = clang
INC = -I$(INC_DIR)

CFLAGS += $(INC) -Wall -std=c99 -O2 -g  -Wextra
LDFLAGS = -lm

# Liste des fichiers objet

# les notres...
OBJPROF_FILES =  $(OBJ_DIR)/huffman.o  $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/bitstream.o

# et les votres!
OBJ_FILES = $(OBJ_DIR)/main.o
OBJ_FILES += $(OBJ_DIR)/allouer.o
OBJ_FILES += $(OBJ_DIR)/jpeg2ppm.o
OBJ_FILES += $(OBJ_DIR)/YCbCr_vers_RGB.o
OBJ_FILES += $(OBJ_DIR)/mcu_to_pixels.o
OBJ_FILES += $(OBJ_DIR)/extraction_decompression.o
OBJ_FILES += $(OBJ_DIR)/idct.o
OBJ_FILES += $(OBJ_DIR)/image.o
OBJ_FILES += $(OBJ_DIR)/quantification_zigzag_inverse.o
OBJ_FILES += $(OBJ_DIR)/pgm_ppm.o
OBJ_FILES += $(OBJ_DIR)/upsampling.o

# cible par défaut

TARGET = $(BIN_DIR)/main

all: $(TARGET)

$(TARGET): $(OBJPROF_FILES) $(OBJ_FILES)
	$(LD) $(LDFLAGS) $(OBJPROF_FILES) $(OBJ_FILES) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/*.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ_FILES)
