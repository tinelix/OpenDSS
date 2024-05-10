# Compiler (GCC by default)
CC=gcc

ifeq ($(OSTYPE), "msys")
	CC=mingw-w64-x86_64-gcc
endif

CC_FLAGS=-g -std=c++98 -Wall -Wl,-O1 -pipe -O2 -flto=2 \
	 -fno-fat-lto-objects -fuse-linker-plugin -fPIC

# Directories needed for build
SRC_DIR=src
LIBS_DIR=libs
OUT_DIR=out

# Libraries
FFH_DIRS	= -I./out/ffmpeg/include
FFL_DIRS	= -L./out/ffmpeg/lib
ID3_SRC=libs/libid3/src

LIBS		= $(FFH_DIRS) $(FFL_DIRS)
EXT_INCLUDES	= -I./src
POSTLIBS	= -lncursesw $(EXT_INCLUDES) -lstdc++

# Source codes
SOURCES=$(SRC_DIR)/*.cpp $(SRC_DIR)/utils/*.cpp $(SRC_DIR)/controls/*.cpp $(ID3_SRC)/id3.cpp
OUT_FILE=$(OUT_DIR)/opendss

# Clean files function
DEL_FILE      	= rm -f

# Targets
build: $(SOURCE)
	$(CC) $(CC_FLAGS) $(LIBS) $(SOURCES) -o $(OUT_FILE) $(POSTLIBS)

clean:
	$(DEL_FILE) out/*
