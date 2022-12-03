CXXFLAGS = -std=c++20 -fmax-errors=5 -O2 -g   -Wall -fmessage-length=0 -I $(INCLUDEDIR)  

_OBJS = aoc2022.o util.o
OBJS = $(patsubst %,$(OBJSDIR)/%,$(_OBJS))
INCLUDEDIR = include
OBJSDIR = objs
SRCDIR = src

TARGET = AOC2022 

$(OBJSDIR)/%.o: $(SRCDIR)/%.cpp  $(INCLUDEDIR)/*.hpp
	$(CXX) $(CXXFLAGS)  -c -o $@  $<

$(TARGET):	$(OBJS) 
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all: $(TARGET)

debug: CFLAGS += -g -O0 -Wall -Wextra
debug: $(TARGET)

build: $(TARGET)

run:
	./$(TARGET)

rerun: clean all run

clean:
	rm -f $(OBJS) $(TARGET) 
