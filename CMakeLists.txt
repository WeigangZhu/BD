cmake_minimum_required (VERSION 2.6)

# project name
project (BD)

# The version number.
set (BD_VERSION_MAJOR 1)
set (BD_VERSION_MINOR 0)

# set compile flag
add_definitions ("-Wall -g")
# set(CMAKE_C_FLAGS  "-Wall")
# set(CMAKE_CXX_FLAGS  "-Wall")

# set extern libraries
set (libraries libm.so)

# set source directory
aux_source_directory(src  SRC_LIST)

# head files
include_directories(include)

# set src file 
# set (SRCLIST  src/main.c src/bd_sov.c)

# set prefix
set(ps_build_binary_prefix  "ps_")
 
add_executable(${ps_build_binary_prefix}${PROJECT_NAME}  ${SRC_LIST})

