CC = g++

COMPILER_SRC = src/compiler.cpp src/lex/*.cpp src/syntax/*.cpp src/translator/*.cpp
EXEC_SRC = src/exec.cpp src/vm/*.cpp
DEPENDENCIES = src/constants.cpp src/models/*.cpp

COMPILER_BIN = build/compile.exe
EXEC_BIN = build/exec.exe


build: build_compiler build_exec

build_compiler: create_build_dir $(SRC)
	$(CC) $(COMPILER_SRC) $(DEPENDENCIES) -o $(COMPILER_BIN) -std=c++11

build_exec: create_build_dir $(COMPILER_SRC)
	$(CC) $(EXEC_SRC) $(DEPENDENCIES) -o $(EXEC_BIN) -std=c++11

create_build_dir:
	mkdir -p build

clean:
	rm -r build
