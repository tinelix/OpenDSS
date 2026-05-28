PROJECT         = OpenDSS

OPENDSS_ROOT    = ..\..

INC_DIR         = $(OPENDSS_ROOT)\include
LIBS_INC_DIR    = $(OPENDSS_ROOT)\libs
SRC_DIR         = $(OPENDSS_ROOT)\src
RES_DIR         = $(OPENDSS_ROOT)\res

BIN_DIR         = $(OPENDSS_ROOT)\out\bin
OBJ_DIR         = $(OPENDSS_ROOT)\out\obj


C_FLAGS	        = -MT -W3 -Zi -EHsc -Od

!ifdef DEBUG
C_FLAGS         = -MT -W3 -Zi -EHsc -Od -DDEBUG
LD_FLAGS        = -debug -pdb:none
!endif


LD_LIBS         = user32.lib \
                  $(CROCON_ROOT)\out\library\bin\crocon.lib \
                  $(OPENDSE_ROOT)\out\bin\opendse.lib

CC_FLAGS        = $(C_FLAGS) -I$(INC_DIR) -I$(LIBS_INC_DIR) \
                  -DWIN32 -DWINDOWS -DWIN32 -DWINDOWS -DWIN32_MME -DMSVC_GE_800

CC_FLAGS_DLL    = $(C_FLAGS) -I$(INC_DIR)

CC              = cl -nologo
LINKER          = link.exe -nologo

OUT_EXE         = $(BIN_DIR)\opendss.exe

OBJECTS         = $(OBJ_DIR)\main.obj

all: prepare $(OUT_EXE)

$(OUT_EXE): $(OBJECTS)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(LINKER) $(LD_FLAGS) $(LD_LIBS) -out:$@ $**
	
	copy $(OPENDSE_ROOT)\out\bin\opendse.dll $(BIN_DIR)\opendse.dll
	copy $(CROCON_ROOT)\out\bin\crocon.dll $(BIN_DIR)\crocon.dll

{$(SRC_DIR)}.c{$(OBJ_DIR)}.obj:
	$(CC) $(CC_FLAGS) -c $< -Fo$@

prepare:
	@if not exist $(LIBS_INC_DIR) mkdir $(LIBS_INC_DIR)
	@if not exist $(LIBS_INC_DIR)\crocon mkdir $(LIBS_INC_DIR)\crocon
	@if not exist $(LIBS_INC_DIR)\dse    mkdir $(LIBS_INC_DIR)\dse

	xcopy $(CROCON_ROOT)\library\include  $(LIBS_INC_DIR)\crocon  /s
	xcopy $(OPENDSE_ROOT)\include         $(LIBS_INC_DIR)\dse     /s

	@if not exist $(OPENDSS_ROOT)\out mkdir $(OPENDSS_ROOT)\out
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

clean:
	-del $(OBJ_DIR)\*.obj
	-del $(BIN_DIR)\*.exe
	-del $(BIN_DIR)\*.exp
	@if exist $(LIBS_INC_DIR) rd $(LIBS_INC_DIR) /s
