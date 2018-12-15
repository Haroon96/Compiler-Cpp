CC = g++

COMPILER_SRC = src/compiler.cpp src/lex/*.cpp src/parser/*.cpp src/translator/*.cpp
INTERPRETER_SRC = src/interpreter.cpp src/vm/*.cpp
CONSTANTS_SRC = src/constants.cpp
MODELS_SRC = src/models/*.cpp

COMPILER_BIN = build/compile.exe
INTERPRETER_BIN = build/interpret.exe


all: compiler interpreter

compiler: create_build_dir
	$(CC) $(COMPILER_SRC) $(CONSTANTS_SRC) $(MODELS_SRC) -o $(COMPILER_BIN) -std=c++11

interpreter: create_build_dir
	$(CC) $(INTERPRETER_SRC) $(CONSTANTS_SRC) -o $(INTERPRETER_BIN) -std=c++11

create_build_dir:
	mkdir -p build

clean:
	rm -f $(COMPILER_BIN) $(INTERPRETER_BIN)
