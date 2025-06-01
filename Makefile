CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
SRCDIR = src
INCDIR = include
OBJDIR = obj
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
HEADERS = $(wildcard $(INCDIR)/*.h) $(wildcard $(SRCDIR)/*.h)
TARGET = oom-memory-gobbler

# Include directories
CXXFLAGS += -I$(INCDIR)

.PHONY: all clean format test debug release

all: CXXFLAGS += -O2
all: $(TARGET)

debug: CXXFLAGS += -g -DDEBUG -O0
debug: clean $(TARGET)

release: CXXFLAGS += -DNDEBUG -O2
release: clean $(TARGET)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJDIR) *.d

format:
	@echo "Formatting code..."
	clang-format -i $(SOURCES) $(HEADERS)

test: $(TARGET)
	@echo "Running tests..."
	./$(TARGET) -h
	./$(TARGET) -m 1M -d 0
	./$(TARGET) -m 1M -d 0-0
	@echo "Tests successful."

# Dependency generation
ifneq ($(MAKECMDGOALS),clean)
    -include $(OBJECTS:.o=.d)
endif
