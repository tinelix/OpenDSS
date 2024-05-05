# Compiler (GCC by default)
CC=gcc
CC_FLAGS=-g -std=c++98 -Wall -Wl,-O1 -pipe -O2 -flto=2 \
	 -fno-fat-lto-objects -fuse-linker-plugin -fPIC

# Directories needed for build
SRC_DIR=src
OUT_DIR=out

# Libraries
FFH_DIRS	= -I./out/ffmpeg/include
FFL_DIRS	= -L./out/ffmpeg/lib
LIBS		= $(FFH_DIRS) $(FFL_DIRS)
EXT_INCLUDES	= -I./src
POSTLIBS	= -lncursesw $(EXT_INCLUDES) -lstdc++

# Source codes
SOURCES=$(SRC_DIR)/*.cpp $(SRC_DIR)/utils/*.cpp
OUT_FILE=$(OUT_DIR)/opendss

# Clean files function
DEL_FILE      	= rm -f

# Targets
build: $(SOURCE)
	$(CC) $(CC_FLAGS) $(LIBS) $(SOURCES) -o $(OUT_FILE) $(POSTLIBS)

clean:
	$(DEL_FILE) out/*
