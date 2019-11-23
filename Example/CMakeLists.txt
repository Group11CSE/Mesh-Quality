cmake_minimum_required(VERSION 3.10)

include_directories(${PROJECT_SOURCE_DIR}/include/)
add_executable(Example Example.cpp)
target_link_libraries (Example  ${GMSH_LIB} ${EXTRA_LIBS})