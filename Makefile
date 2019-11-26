CC = g++ # This is the main compiler
# CC = clang --analyze # and comment out the linker last line for sanity
SRCDIR = src
BUILDDIR = build
TARGET = bin/runner

SRCEXT = cpp
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS = -g -std=c++11 -O2
LIB =  -pthread -lraw  -lopencv_core  -lopencv_imgcodecs #-L lib 
INC = -I include `pkg-config --cflags eigen3`

OPENGL_LIBS = -lglut -lGL -lGLU
BOOST_LIBS = -lboost

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) `wx-config  --libs std gauge listbox spinctrl --gl_libs` $(OPENGL_LIBS) $(LIB) `pkg-config --cflags opencv`"; $(CC) $^ -o $(TARGET) `wx-config --libs --gl_libs` $(OPENGL_LIBS) $(LIB) `pkg-config --cflags opencv`

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $< `wx-config --cxxflags ` $(OPENGL_LIBS) `pkg-config --cflags opencv`"; $(CC) $(CFLAGS) $(INC) -c -o $@ $< `wx-config  --cxxflags` $(OPENGL_LIBS) `pkg-config --cflags opencv`

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Tests
tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) `wx-config  --cxxflags` $(OPENGL_LIBS) $(LIB) -o bin/tester

# Spikes
ticket:
	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) `wx-config  --cxxflags` $(OPENGL_LIBS) $(LIB) -o bin/ticket

.PHONY: clean
