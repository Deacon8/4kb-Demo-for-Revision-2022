CC   = gcc.exe
CL   = crinkler.exe
CLFLAGS = /OUT:demo.exe /PRIORITY:NORMAL /COMPMODE:SLOW /HASHSIZE:100 /HASHTRIES:40 /TRANSFORM:CALLS  /SUBSYSTEM:WINDOWS /RANGE:opengl32 /RANGE:glu32 
OBJ  = main.o
LINKOBJ  = main.o
CLLIBS = kernel32.lib user32.lib gdi32.lib opengl32.lib glu32.lib winmm.lib msvcrt.lib
CLLIBPATH = /LIBPATH:"D:\Lib\10.0.19041.0\um\x86"
LIBS =  -lmingw32 -lopengl32 -lglu32 -lgdi32 -lkernel32 -luser32 -lm -lwinmm -mwindows -mno-cygwin -nostdlib -lmsvcrt
BIN  = demo.exe
CFLAGS = -Os
RM = rm -f
IPATHS = -IRevision22 main.c
# -ID:\Include  -ID:\Include\10.0.19041.0\shared

.PHONY: all all-before all-after clean clean-custom

all: all-before demo.exe all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

# crinkler ...
$(BIN): $(OBJ)
	$(CL)  $(CLLIBPATH) $(CLFLAGS) $(CLLIBS) main.o
    	
# normal linking
#$(BIN): $(OBJ)
#	$(CC) $(LINKOBJ) -s -o "demo.exe" $(LIBS)

main.o: main.c
	$(CC) -c $(CFLAGS) $(IPATHS) -o main.o
