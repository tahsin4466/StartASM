cmake_minimum_required(VERSION 3.10)
project(StartASM)

set(CMAKE_CXX_STANDARD 17)

# Set the CMAKE_PREFIX_PATH to the directory containing LLVMConfig.cmake if necessary
# set(CMAKE_PREFIX_PATH "/usr/lib/llvm-14/lib/cmake/llvm/")

# Find LLVM package
find_package(LLVM REQUIRED CONFIG)

# Include LLVM headers
include_directories(${LLVM_INCLUDE_DIRS})

# Find OpenMP package
find_package(OpenMP REQUIRED)

# Add LLVM and OpenMP definitions
add_definitions(${LLVM_DEFINITIONS})
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

# List all source files and headers
set(SOURCES
        src/Compiler.cpp
        src/Instructions.cpp
        src/Lexer.cpp
        src/Parser.cpp
        src/SemanticAnalyzer.cpp
        src/StartASM.cpp
        src/CodeGenerator.cpp
)

set(HEADERS
        src/include/Compiler.h
        src/include/Instructions.h
        src/include/Lexer.h
        src/include/Parser.h
        src/include/AbstractSyntaxTree.h
        src/include/SemanticAnalyzer.h
        src/include/CodeGenerator.h
)

# Specify the executable target
add_executable(startasm ${SOURCES} ${HEADERS})

# Include project headers
include_directories(include)

# Link against LLVM and OpenMP libraries
llvm_map_components_to_libnames(LLVM_LIBS core support irreader)
target_link_libraries(startasm ${LLVM_LIBS} OpenMP::OpenMP_CXX)

# Set the output directory for the executable
set_target_properties(startasm PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
