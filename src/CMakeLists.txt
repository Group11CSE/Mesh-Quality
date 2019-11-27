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

cmake_minimum_required(VERSION 3.10)

include_directories(${PROJECT_SOURCE_DIR}/include/)
add_executable(Mesh_Quality ${main_sources})
target_link_libraries (Mesh_Quality  ${GMSH_LIB} ${EXTRA_LIBS})