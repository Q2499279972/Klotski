TARGETS = klotski.exe
CXXFLAGS = -pipe \
           -O2 -static\
           -fthread-jumps \
           -fexpensive-optimizations \
           -fcheck-new \
           -Wall -W -pedantic -Wno-long-long \
           -Wunknown-pragmas -Wundef -Wconversion -Wno-missing-braces \
           -Wuninitialized -Wpointer-arith -Wno-multichar \
           -mwindows 
objs:=main.o texture.o history.o Klotski.o KlotskiTypes.o \
KlotskiBlock.o KlotskiMap.o misc.o window.o RightButtonMenu.o\
ReadLuaConfig.o

all:$(SRC_DIR) $(objs)  
	g++ $(CXXFLAGS) $(objs) ./liblua.a ./regex2_7.a -o $(TARGETS) -lstdc++ -lopengl32 -lglu32
	strip $(TARGETS)
.cpp.o: 
#	g++ -c $(CXXFLAGS) $< -o $@  -D_EXPORTBMP_
	g++ -c $(CXXFLAGS) $< -o $@
