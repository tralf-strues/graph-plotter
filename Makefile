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
LIBS = sdl2 sdl2_ttf sdl2_image

LXXFLAGS = $(shell pkg-config --libs $(LIBS)) $(ModeLinkerOptions)
CXXFLAGS = $(shell pkg-config --cflags $(LIBS)) $(ModeCompilerOptions) $(AllWarnings) -std=c++2a 
# ------------------------------------Options-----------------------------------

# -------------------------------------Files------------------------------------
Program = ray_tracer

SrcDir = src
BinDir = bin
IntDir = $(BinDir)/intermediates

3dDir         = $(SrcDir)/3d_graphics
CoreDir       = $(SrcDir)/core
ContainersDir = $(CoreDir)/containers
GraphWrapDir  = $(CoreDir)/graphics_wrapper
MathDir       = $(CoreDir)/math
GUIDir        = $(SrcDir)/gui
RTDir         = $(SrcDir)/ray_tracer
IGDir         = $(SrcDir)/ideal_gas

Deps = $(wildcard $(SrcDir)/*.h)        \
       $(wildcard $(3dDir)/*.h)         \
       $(wildcard $(CoreDir)/*.h)       \
       $(wildcard $(ContainersDir)/*.h) \
       $(wildcard $(GraphWrapDir)/*.h)  \
       $(wildcard $(MathDir)/*.h)       \
       $(wildcard $(GUIDir)/*.h)

CppSrc = $(notdir $(wildcard $(SrcDir)/*.cpp))        \
		 $(notdir $(wildcard $(3dDir)/*.cpp))         \
		 $(notdir $(wildcard $(CoreDir)/*.cpp))       \
		 $(notdir $(wildcard $(ContainersDir)/*.cpp)) \
		 $(notdir $(wildcard $(GraphWrapDir)/*.cpp))  \
		 $(notdir $(wildcard $(MathDir)/*.cpp))       \
		 $(notdir $(wildcard $(GUIDir)/*.cpp))        

Objs = $(addprefix $(IntDir)/, $(CppSrc:.cpp=.o))

ifeq ($(Program), ray_tracer)
	Deps   += $(wildcard $(RTDir)/*.h)
	CppSrc += $(notdir $(wildcard $(RTDir)/*.cpp))
	Exec    = $(BinDir)/ray-tracer.out
endif

ifeq ($(Program), ideal_gas)
	Deps   += $(wildcard $(IGDir)/*.h)
	CppSrc += $(notdir $(wildcard $(IGDir)/*.cpp))
	Exec    = $(BinDir)/ideal-gas-simulator.out
endif
# -------------------------------------Files------------------------------------

# ----------------------------------Make rules----------------------------------
$(Exec): $(Objs) $(Deps)
	$(CXX) -o $(Exec) $(Objs) $(LXXFLAGS)

Dirs = $(SrcDir) $(3dDir) $(CoreDir) $(ContainersDir) $(GraphWrapDir) $(MathDir) $(GUIDir)

ifeq ($(Program), ray_tracer)
	Dirs += $(RTDir)
endif

ifeq ($(Program), ideal_gas)
	Dirs += $(IGDir)
endif

vpath %.cpp $(Dirs)
$(IntDir)/%.o: %.cpp $(Deps)
	$(CXX) -c $< $(CXXFLAGS) -o $@

.PHONY: init
init:
	mkdir -p bin/intermediates

.PHONY: clean
clean:
	rm -f $(Objs) $(Exec)
# ----------------------------------Make rules----------------------------------
