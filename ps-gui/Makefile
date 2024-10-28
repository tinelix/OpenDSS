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
			  -I../$(LIBS_DIR)

POSTLIBS		= -lncursesw $(EXT_INCLUDES) $(EXT_LIBS) -lstdc++ -lm -lpthread -ldl \

LIB_SOURCES 		= $(ID3_SRC)/*.cpp \
			  ../$(LIBS_DIR)/dse/*.c

# Source codes
SOURCES			= $(SRC_DIR)/*.cpp \
			  $(SRC_DIR)/utils/*.cpp \
			  $(SRC_DIR)/windows/*.cpp \
			  $(SRC_DIR)/interfaces/*.cpp \
			  $(SRC_DIR)/controls/*.cpp \
			  $(LIB_SOURCES)

OUT_FILE=$(OUT_DIR)/opendss

# Static libraries linking
SA_CC_FLAGS 		= -g -std=c++98 -Wall -static
SA_LIBS			= $(EXT_INCLUDES) -L./out
SA_JCPP_FILE		= $(OUT_DIR)/jsoncpp.o
SA_JCPP_ARCH_FILE	= $(OUT_DIR)/jsoncpp.a
SA_POSTLIBS		= -lncursesw $(EXT_INCLUDES) -ltinfo -lstdc++

ifeq ($(OS),Windows_NT)
	POSTLIBS	= -lncursesw $(EXT_INCLUDES) $(EXT_LIBS) -lstdc++ -lm -lpthread \
			  -DRAUDIO_STANDALONE \
			  -DSUPPORT_MODULE_RAUDIO -DSUPPORT_FILEFORMAT_WAV \
			  -DSUPPORT_FILEFORMAT_OGG -DSUPPORT_FILEFORMAT_MP3 -DSUPPORT_FILEFORMAT_FLAC
	SA_CC_FLAGS 	= -g -std=c++98 -Wall
	SA_POSTLIBS 	= -lncursesw -lstdc++ $(EXT_INCLUDES) -I/mingw64/include/ncurses -static -DNCURSES_STATIC \
			  -DSUPPORT_MODULE_RAUDIO -DRAUDIO_STANDALONE -DSUPPORT_FILEFORMAT_WAV \
			  -DSUPPORT_FILEFORMAT_OGG -DSUPPORT_FILEFORMAT_MP3 -DSUPPORT_FILEFORMAT_FLAC
endif

# Clean files function
DEL_FILE      	= rm -rf
BUILD_RAUDIO_DLL =

ifeq ($(OS),Windows_NT)
	BUILD_RAUDIO_DLL = $(CC) -DRAUDIO_STANDALONE -DSUPPORT_FILEFORMAT_WAV -DSUPPORT_FILEFORMAT_OGG \
				 -DSUPPORT_FILEFORMAT_MP3 -DSUPPORT_FILEFORMAT_FLAC \
				 -Wall $(RAUDIO_SRC)/raudio.c -shared -o $(OUT_DIR)/raudio.dll
endif

# Targets
build: $(SOURCE)
	$(CC) $(CC_FLAGS) $(LIBS) $(SOURCES) -o $(OUT_FILE) $(POSTLIBS)

libre-build: $(SOURCE)
	$(CC) $(CC_FLAGS) $(LIBS) $(SOURCES) -o $(OUT_FILE) $(POSTLIBS) -DLICENSE_GPLV3

standalone:
	mkdir -p ./out/libs
	$(BUILD_RAUDIO_DLL)
	$(CC) $(SA_CC_FLAGS) $(LIBS) $(SOURCES) -o $(OUT_FILE) $(SA_POSTLIBS)

clean:
	$(DEL_FILE) out/*
