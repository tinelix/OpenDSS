PROJECT         = opendss

OPENDSS_ROOT    = ../..
OPENDSS_DEBUG   =? false

PREFIX          = /usr/local

INC_DIRS        = $(OPENDSS_ROOT)/include
LIBS_DIR        = $(OPENDSS_ROOT)/libs
SRC_DIR         = $(OPENDSS_ROOT)/src
DEF_DIR         = $(OPENDSS_ROOT)/def
RES_DIR         = $(OPENDSS_ROOT)/res

SRC_DIRS        = $(SRC_DIR)

LD_LIBS         = -lcrocon -lopendse

ifeq ($(OPENDSS_LEGACY),true)

ifeq ($(OPENDSS_DEBUG),true)
C_FLAGS         =  -O0 -fPIC -DUNIX -DUNIX_LEGACY -DHAVE_VPRINTF -g3 -fno-omit-frame-pointer
else
C_FLAGS         =  -O2 -fPIC -DUNIX -DUNIX_LEGACY -DHAVE_VPRINTF
endif

else

ifeq ($(OPENDSS_DEBUG),true)
C_FLAGS         =  -O0 -fPIC -DUNIX -DHAVE_VPRINTF -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls 
else
C_FLAGS         =  -O2 -fPIC -DUNIX -DHAVE_VPRINTF 
endif

endif

BIN_DIR         = $(OPENDSS_ROOT)/out/bin
OBJ_DIR         = $(OPENDSS_ROOT)/out/obj

SOURCES         = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))

OBJECTS         = $(OBJ_DIR)/main.o

all: prepare $(BIN_DIR)/$(PROJECT).bin

prepare:
	mkdir -p $(LIBS_DIR)
	cp -R $(CROCON_ROOT)/library/include/. $(LIBS_DIR)/crocon
	cp -R $(OPENDSE_ROOT)/include/. $(LIBS_DIR)/dse
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(C_FLAGS) -I$(INC_DIRS) -I$(LIBS_DIR) -c $< -o $@

$(BIN_DIR)/$(PROJECT).bin: $(OBJECTS)
	$(CC) $(LD_ARCH) $(OBJECTS) -DUNIX \
	-L$(CROCON_ROOT)/out/library/bin \
	-L$(OPENDSE_ROOT)/out/bin \
	$(LD_LIBS) -o $@ $(LD_FLAGS)

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)

rebuild:
	clean all

.PHONY: 
	all clean rebuild
