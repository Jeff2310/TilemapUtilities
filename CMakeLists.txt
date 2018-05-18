cmake_minimum_required(VERSION 3.9)
project(TileGenerator)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(./ SRCS)
aux_source_directory(./srcs/Generator Generator_SRCS)
aux_source_directory(./srcs/Noise Noise_SRCS)
aux_source_directory(./srcs/TileMap TileMap_SRCS)

list(APPEND SRCS ${Generator_SRCS})
list(APPEND SRCS ${Noise_SRCS})
list(APPEND SRCS ${TileMap_SRCS})

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(TileGenerator ${SRCS})
target_link_libraries(TileGenerator -lfreeimage)