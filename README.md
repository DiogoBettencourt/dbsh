# DBSH - Shell

A powerful command-line shell implementation written in C++ with support for command parsing, execution, and built-in operations.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Usage](#usage)
- [Architecture](#architecture)
- [Testing](#testing)
- [Time Breakdown](#time-breakdown)
- [Contributing](#contributing)
- [License](#license)

## Overview

DBSH is a custom shell implementation that provides a complete command-line interface with features including command parsing, execution management, and built-in command support. This project demonstrates core concepts in systems programming, command interpretation, and process management.

## Features

- **Command Parsing**: Robust lexical and syntactic analysis of user input
- **Command Execution**: Process execution with proper error handling
- **Built-in Commands**: Support for common shell operations without spawning new processes
- **Interactive Shell**: Full REPL (Read-Eval-Print Loop) functionality
- **Comprehensive Testing**: Unit tests for all major components

## Project Structure

```
dbsh/
├── main.cpp                 # Entry point of the shell
├── shell.h/cpp             # Main shell implementation and REPL loop
├── parser.h/cpp            # Command parsing and tokenization
├── executor.h/cpp          # Command execution engine
├── builtins.h/cpp          # Built-in command implementations
├── test_parser.cpp         # Parser unit tests
├── test_executor.cpp       # Executor unit tests
├── test_builtins.cpp       # Built-in commands unit tests
├── CMakeLists.txt          # CMake build configuration
└── README.md               # This file
```

## Installation

### Prerequisites

- CMake 3.10 or higher
- C++ compiler supporting C++17 (GCC, Clang, or MSVC)
- Catch2 library for testing

### Building

```bash
# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make

# Run the shell
./dbsh

# Run tests
./tests
```

## Usage

### Starting the Shell

```bash
./dbsh
```

The shell will display a prompt and accept commands just like a traditional Unix shell.

### Commands

The shell supports:
- **External commands**: Any executable available in your system PATH
- **Built-in commands**: Internal shell commands for common operations
- **Command pipes**: Chain commands with data flowing between them

## Architecture

### Components

#### Parser (`parser.h/cpp`)
- Tokenizes input strings into command tokens
- Handles quoted strings and escape sequences
- Builds abstract command structures
- Supports basic syntax error detection

#### Executor (`executor.h/cpp`)
- Manages process creation and lifecycle
- Handles I/O redirection
- Implements pipe functionality between commands
- Manages exit codes and error handling

#### Builtins (`builtins.h/cpp`)
- Implements shell built-in commands
- Examples: `cd`, `echo`, `exit`, etc.
- Operates within shell's own process context

#### Shell (`shell.h/cpp`)
- Main REPL implementation
- Coordinates parser, executor, and built-in systems
- Manages shell state and configuration

### Data Flow

```
User Input
    ↓
Parser (tokenize & parse)
    ↓
Executor (built-in or external command)
    ↓
Output
```

## Testing

The project includes comprehensive unit tests built with Catch2:

```bash
# Run all tests
cd build
./tests

# Run specific test suite
./tests [test_name]
```

### Test Coverage

- **Parser Tests**: Input tokenization, syntax validation, edge cases
- **Executor Tests**: Process spawning, error handling, return codes
- **Built-in Tests**: Individual command behavior, argument validation

## Time Breakdown

Understanding the effort distribution across this project:

| Component | Hours | Percentage | Description |
|-----------|-------|-----------|-------------|
| **Architecture & Design** | 1-2h | 7-9% | Planning, system design, component interaction |
| **Parser Implementation** | 4-5h | 18-22% | Tokenization, syntax analysis, string handling |
| **Executor Implementation** | 4-5h | 18-22% | Process management, I/O handling, signal handling |
| **Built-in Commands** | 2-3h | 9-13% | Command implementations, argument processing |
| **Shell REPL Loop** | 1-2h | 5-9% | Main loop, prompt management, user interaction |
| **Unit Testing** | 3-4h | 13-18% | Test cases for all components, edge cases |
| **Documentation & Cleanup** | 1-2h | 5-9% | README, comments, code organization |
| **Build System (CMake)** | 1h | 4-5% | CMakeLists.txt, Catch2 integration |
| **Debugging & Optimization** | 1-2h | 5-9% | Bug fixes, performance improvements |
| **TOTAL** | **18-26h** | **100%** | Average: ~22 hours |

### Effort Notes

- **Highest effort**: Parser and Executor components due to complexity
- **Critical path**: Parser → Executor pipeline must be well-designed
- **Testing overhead**: ~15-18% reflects solid test coverage

## Contributing

1. Fork the repository
2. Create a feature branch
3. Implement changes with tests
4. Ensure all tests pass
5. Submit a pull request

## License

This project is provided as-is for educational and development purposes.