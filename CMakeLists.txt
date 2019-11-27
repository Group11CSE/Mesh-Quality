# /**
#  * Copyright (C) <2019>

#  * This program is free software: you can redistribute it and/or modify
#  * it under the terms of the GNU General Public License as published by
#  * the Free Software Foundation, either version 3 of the License, or
#  * (at your option) any later version.

#  * This program is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.

#  * You should have received a copy of the GNU General Public License
#  * along with this program.  If not, see <https://www.gnu.org/licenses/>.
# */

# TAKEN from the GMSH repository and modified
# to work in this project

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

find_library(GMSH_LIB gmsh)
if(NOT GMSH_LIB)
  message(FATAL_ERROR "Could not find libgmsh")
endif()

find_path(GMSH_INC gmsh.h)
if(NOT GMSH_INC)
  message(FATAL_ERROR "Could not find gmsh.h")
endif()

if(GMSH_LIB MATCHES ".a")
  find_library(BLAS_LIB blas)
  if(BLAS_LIB)
    list(APPEND EXTRA_LIBS ${BLAS_LIB})
  endif()
  find_library(LAPACK_LIB lapack)
  if(LAPACK_LIB)
    list(APPEND EXTRA_LIBS ${LAPACK_LIB})
  endif()
endif()

if(WIN32 OR CYGWIN)
  list(APPEND EXTRA_LIBS winmm wsock32 ws2_32 psapi)
endif()

find_package(CGAL)
list(APPEND EXTRA_LIBS CGAL::CGAL)

include_directories(${GMSH_INC})

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra")

file(GLOB main_sources . src/*.cpp)

add_subdirectory(src)