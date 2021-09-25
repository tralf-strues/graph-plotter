# -----------------------------------Constants----------------------------------
AllWarnings    = -Wall -Wextra -pedantic
SomeWarnings   = -Wall -Wextra
LittleWarnings = -Wall
NoWarnings     = 
# -----------------------------------Constants----------------------------------

Mode = DEBUG_MODE
# ----------------------------------Debug-mode----------------------------------
ifeq ($(Mode), DEBUG_MODE)
	ModeLinkerOptions   = -g
	ModeCompilerOptions = -O0 -g
endif
# ----------------------------------Debug-mode----------------------------------

# ---------------------------------Release-mode---------------------------------
ifeq ($(Mode), RELEASE_MODE)
	ModeLinkerOptions   = 
	ModeCompilerOptions = -O3
endif
# ---------------------------------Release-mode---------------------------------

# ------------------------------------Options-----------------------------------
LIBS = sdl2 sdl2_ttf

LXXFLAGS = $(shell pkg-config --libs $(LIBS)) $(ModeLinkerOptions)
CXXFLAGS = $(shell pkg-config --cflags $(LIBS)) $(ModeCompilerOptions) $(AllWarnings) -std=c++2a 
# ------------------------------------Options-----------------------------------

# -------------------------------------Files------------------------------------
SrcDir = src
BinDir = bin
IntDir = $(BinDir)/intermediates

3dDir         = $(SrcDir)/3d_graphics
AppDir        = $(SrcDir)/app
CoreDir       = $(SrcDir)/core
ContainersDir = $(CoreDir)/containers
GraphWrapDir  = $(CoreDir)/graphics_wrapper
MathDir       = $(CoreDir)/math
GUIDir        = $(SrcDir)/gui

Deps = $(wildcard $(SrcDir)/*.h)        \
       $(wildcard $(3dDir)/*.h)         \
       $(wildcard $(AppDir)/*.h)        \
       $(wildcard $(CoreDir)/*.h)       \
       $(wildcard $(ContainersDir)/*.h) \
       $(wildcard $(GraphWrapDir)/*.h)  \
       $(wildcard $(MathDir)/*.h)       \
       $(wildcard $(GUIDir)/*.h)

CppSrc = $(notdir $(wildcard $(SrcDir)/*.cpp))        \
		 $(notdir $(wildcard $(3dDir)/*.cpp))         \
		 $(notdir $(wildcard $(AppDir)/*.cpp))        \
		 $(notdir $(wildcard $(CoreDir)/*.cpp))       \
		 $(notdir $(wildcard $(ContainersDir)/*.cpp)) \
		 $(notdir $(wildcard $(GraphWrapDir)/*.cpp))  \
		 $(notdir $(wildcard $(MathDir)/*.cpp))       \
		 $(notdir $(wildcard $(GUIDir)/*.cpp))        

Objs   = $(addprefix $(IntDir)/, $(CppSrc:.cpp=.o))
Exec   = $(BinDir)/graph-plotter
# -------------------------------------Files------------------------------------

# ----------------------------------Make rules----------------------------------
$(Exec): $(Objs) $(Deps)
	$(CXX) -o $(Exec) $(Objs) $(LXXFLAGS)

vpath %.cpp $(SrcDir) $(3dDir) $(AppDir) $(CoreDir) $(ContainersDir) $(GraphWrapDir) $(MathDir) $(GUIDir)
$(IntDir)/%.o: %.cpp $(Deps)
	$(CXX) -c $< $(CXXFLAGS) -o $@

.PHONY: init
init:
	mkdir -p bin/intermediates

.PHONY: clean
clean:
	rm -f $(Objs) $(Exec)
# ----------------------------------Make rules----------------------------------
