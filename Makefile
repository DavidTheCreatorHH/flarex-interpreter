CXX := g++
CXXFLAGS := -Wall -Wextra -O2 -std=c++17
SRCDIR := .
BUILDDIR := build
TARGET := fx
INC := include

# Explicitly list sources OR find all cpp files but ignore main duplicates
SRCS := $(shell find $(SRCDIR)/src -name "*.cpp")

# Build object files mirroring directory structure
OBJS := $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SRCS))

all: $(TARGET)

# Link target
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile rule
$(BUILDDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -I$(INC) $(CXXFLAGS) -c $< -o $@

run:
	clear
	@./$(TARGET) -a example.flx

clean:
	rm -rf $(BUILDDIR)
	rm -rf $(TARGET)

big: all run
	

.PHONY: all clean run big
