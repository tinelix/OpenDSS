# Compiler (GCC by default)
CC=gcc
CC_FLAGS=-g -Wall -Wl,-O1 -pipe -O2 -flto=2 -fno-fat-lto-objects -fuse-linker-plugin -fPIC

# Directories needed for build
SRC_DIR=src
OUT_DIR=out

# Libraries
FFH_DIRS	= -I./out/ffmpeg/include
FFL_DIRS	= -L./out/ffmpeg/lib
LIBS		= $(FFH_DIRS) $(FFL_DIRS) -lncursesw

# Source codes
SOURCES=$(SRC_DIR)/*.cpp

# Clean files function
DEL_FILE      	= rm -f

# Targets
build: $(SOURCE)
	$(CC) $(CC_FLAGS) $(LIBS) $(SOURCES) -o $(OUT_DIR)

clean:
	$(DEL_FILE) out/*
