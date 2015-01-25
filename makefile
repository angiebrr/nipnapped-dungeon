SOURCES=$(wildcard src/*.cpp)
OBJS=$(SOURCES:.cpp=.o)

# Change CFLAGS depending on if its debug or release build
ifneq (,$(findstring debug,$(MAKECMDGOALS)))
	CFLAGS=-g
else
	CFLAGS=-s
endif

# Linker Options -> changes if using Linux or Windows MinGW
ifeq ($(shell sh -c 'uname -s'),Linux)
	LIBFLAGS=-L. -ltcod_debug -ltcodxx_debug -Wl,-rpath=.
else
	LIBFLAGS=-Llib -ltcod-mingw-debug -static-libgcc -static-libstdc++
endif

# Add debug and release targets
debug : nipnapped
release : nipnapped

# Generate executable
nipnapped : $(OBJS)
	g++ $(OBJS) -o nipnapped -Wall $(LIBFLAGS) $(CFLAGS)

# Generate precompiled headers
src/main.hpp.gch : src/*.hpp
	g++ src/main.hpp -Iinclude -Wall

# Retrieve object files
src/%.o : src/%.cpp
	g++ $< -c -o $@ -Iinclude -Wall $(CFLAGS)

# Remove every compiled file
clean :
	rm -f src/main.hpp.gch $(OBJS)