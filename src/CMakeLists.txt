cmake_minimum_required(VERSION 3.10)

include_directories(${PROJECT_SOURCE_DIR}/include/)
add_executable(Mesh_Quality ${main_sources})
target_link_libraries (Mesh_Quality  ${GMSH_LIB} ${EXTRA_LIBS})
