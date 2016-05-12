# Poject name.
PROJECT=info

# Configures and builds the project.
all: build/CMakeCache.txt
	@cmake --build build

# Configures the project.
build/CMakeCache.txt: CMakeLists.txt build
	@cd build && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=.. ..

# Creates the build directory.
build:
	@mkdir build

# Installs the project.
install: build/CMakeCache.txt
	@cmake --build build --target install

# Compiles and runs the service on Makefile based systems.
run: all
	@build/$(PROJECT)

# Removes the build directory containing binary and project files.
clean:
	rm -rf build
