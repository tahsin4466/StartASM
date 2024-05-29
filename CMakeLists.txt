cmake_minimum_required(VERSION 3.10)
project(startasm)

set(CMAKE_CXX_STANDARD 17)

# Set the CMAKE_PREFIX_PATH to the directory containing LLVMConfig.cmake if necessary
if(APPLE)
    set(CMAKE_PREFIX_PATH "/usr/local/opt/llvm")
    set(LLVM_DIR "/usr/local/opt/llvm/lib/cmake/llvm")
    # Set the compiler to use Homebrew LLVM
    set(CMAKE_C_COMPILER "/usr/local/opt/llvm/bin/clang")
    set(CMAKE_CXX_COMPILER "/usr/local/opt/llvm/bin/clang++")
    set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/llvm/include")
    set(OpenMP_C_LIB_NAMES "omp")
    set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/llvm/include")
    set(OpenMP_CXX_LIB_NAMES "omp")
    set(OpenMP_omp_LIBRARY "/usr/local/opt/llvm/lib/libomp.dylib")
endif()

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
        src/.Secrets.cpp
)

set(HEADERS
        src/include/Compiler.h
        src/include/Instructions.h
        src/include/Lexer.h
        src/include/Parser.h
        src/include/AbstractSyntaxTree.h
        src/include/SemanticAnalyzer.h
        src/include/CodeGenerator.h
        src/include/.Secrets.h
)

# Specify the executable target
add_executable(startasm ${SOURCES} ${HEADERS})

# Include project headers
include_directories(src/include)

# Link against LLVM and OpenMP libraries
llvm_map_components_to_libnames(LLVM_LIBS core support irreader)
target_link_libraries(startasm ${LLVM_LIBS} OpenMP::OpenMP_CXX)

# Set the output directory for the executable to the root folder
set_target_properties(startasm PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# Platform-specific settings
if(APPLE)
    message(STATUS "Configuring for macOS")
    # macOS-specific settings here
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    find_library(LIBCXX_LIB c++)
    find_library(LIBCXXABI_LIB c++abi)
    target_link_libraries(startasm ${LIBCXX_LIB} ${LIBCXXABI_LIB})
elseif(UNIX)
    message(STATUS "Configuring for Linux")
    # Linux-specific settings here
endif()

# Ensure CMake RPATH settings for macOS
if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()