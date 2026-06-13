CXX = g++
CXXFLAGS = -Wall -fPIC

CPLEX_DIR = /opt/ibm/ILOG/CPLEX_Studio2211
CPLEX_INCDIR = $(CPLEX_DIR)/cplex/include
CPLEX_LIBDIR = $(CPLEX_DIR)/cplex/lib/x86-64_linux/static_pic
CONCERT_INCDIR = $(CPLEX_DIR)/concert/include
CONCERT_LIBDIR = $(CPLEX_DIR)/concert/lib/x86-64_linux/static_pic

INCLUDES = -I$(CPLEX_INCDIR) -I$(CONCERT_INCDIR)
LIBS = -L$(CPLEX_LIBDIR) -L$(CONCERT_LIBDIR) -lconcert -lilocplex -lcplex -lm -lpthread

SOURCES = $(wildcard *.cpp)
EXECUTABLES = $(SOURCES:.cpp=)

all: $(EXECUTABLES)

%: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@ $(LIBS)

run: $(EXECUTABLES)
	@for exe in $(EXECUTABLES); do ./$$exe; echo; done

clean:
	rm -f $(EXECUTABLES)

.PHONY: all run clean
