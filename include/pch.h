/**
 * Copyright (C) <2019>

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


// This is a precompiled header
// Include everything here that is used very often, please don't
// include stuff in here which also includes this (cyclic includes are bad)

#pragma once

#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <logging.h>
#include <profiling.h>
#include <functional>
#include <memory>
#include <gmsh.h>
#include <mqexception.h>
#include <string_view>


// Debugging build detection
#ifndef NDEBUG
    #define DEBUG
#endif