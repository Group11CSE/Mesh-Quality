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
#include <gmsh.h>


// Debugging build detection
#ifndef NDEBUG
    #define DEBUG
#endif