# Makefile cho dự án KTLT2
CXX = C:/msys64/ucrt64/bin/g++.exe
CXXFLAGS = -fdiagnostics-color=always -g -std=c++17 -Wall
TARGET = main.exe
SOURCES = main.cpp ui/menu.cpp services/quanlylop.cpp services/thongke.cpp middlewares/datevalidator.cpp controllers/filemanager.cpp

# Build target
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Debug build (same as normal since -g is already included)
debug: $(TARGET)

# Clean build files
clean:
	del /f $(TARGET) 2>nul || echo "No build files to clean"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Help
help:
	@echo "Available targets:"
	@echo "  all    - Build the program (default)"
	@echo "  debug  - Build with debug symbols"
	@echo "  clean  - Remove build files"
	@echo "  run    - Build and run the program"
	@echo "  help   - Show this help"

.PHONY: all debug clean run help 