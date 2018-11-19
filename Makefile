# Declaration of variables
CC = g++
CC_FLAGS = -w

# File names
EXEC = run
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPS = -lCore \
         -lGraf -lGraf3d -lHist -lHtml -lMatrix -lMinuit \
         -lPostscript -lProof -lTree \
         -lGpad -lGui -lGX11 -lRint \

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(DEPS)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
