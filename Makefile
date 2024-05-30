# Compiler (GCC by default)
CC=gcc

ifeq ($(OS),Windows_NT)
	CC		= g++
endif

CC_FLAGS		= -g -std=c++98 -Wall -Wl,-O1 -pipe -O2 -flto=2 \
			 -fno-fat-lto-objects -fuse-linker-plugin -fPIC
SA_CC_FLAGS 		= -Wall -c -g

# Directories needed for build
SRC_DIR			= src
LIBS_DIR		= libs
OUT_DIR			= out

# Libraries
EXT_INCLUDES		= -I./$(SRC_DIR) \
			  -I./$(OUT_DIR)/ffmpeg/include \
			  -I./$(LIBS_DIR)/minimp3 \
			  -I./$(LIBS_DIR)/miniaudio
ID3_SRC			= $(LIBS_DIR)/libid3/src

POSTLIBS		= -lncursesw $(EXT_INCLUDES) $(EXT_LIBS) -lstdc++ -lm -lpthread

# Source codes
SOURCES			= $(SRC_DIR)/*.cpp $(SRC_DIR)/utils/*.cpp \
			  $(SRC_DIR)/windows/*.cpp $(SRC_DIR)/controls/*.cpp \
			  $(SRC_DIR)/decoders/*.cpp \
			  $(SRC_DIR)/decoders/mp3/*.cpp \
			  $(ID3_SRC)/*.cpp

OUT_FILE=$(OUT_DIR)/opendss

# Static libraries linking
SA_CC_FLAGS 		= -g -std=c++98 -Wall -static
SA_LIBS			= $(EXT_INCLUDES) -L./out
SA_JCPP_FILE		= $(OUT_DIR)/jsoncpp.o
SA_JCPP_ARCH_FILE	= $(OUT_DIR)/jsoncpp.a
SA_POSTLIBS		= -lncursesw $(EXT_INCLUDES) -ltinfo -lstdc++

ifeq ($(OS),Windows_NT)
	SA_CC_FLAGS 	= -g -std=c++98 -Wall
	SA_POSTLIBS 	= -lncursesw $(EXT_INCLUDES) -I/mingw64/include/ncurses -static -DNCURSES_STATIC
endif

# Clean files function
DEL_FILE      	= rm -rf

# Targets
build: $(SOURCE)
	$(CC) $(CC_FLAGS) $(LIBS) $(SOURCES) -o $(OUT_FILE) $(POSTLIBS)

standalone:
	mkdir -p ./out/libs
	$(CC) $(SA_CC_FLAGS) $(LIBS) $(SOURCES) -o $(OUT_FILE) $(SA_POSTLIBS)

clean:
	$(DEL_FILE) out/*
