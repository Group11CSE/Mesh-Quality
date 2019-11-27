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

#include <iostream>
#include <pch.h>
#include <gmshhandler.h>
#include <CGAL/AABB_tree.h>

// In order to profile a function
// Note this introduces an overhead, in order to disable profiling got to profiling.h and undef PROFILING
void test_function(void){
    PROFILE_FUNCTION;
    //Do stuff
    int a = 1+2;
}

int main(int argc, char *argv[]){
    // Starting Profiling and Logging session
    START_PROFILING_SESSION("Main");
    InstrumentationTimer timer(_CUSTOM_FUNC_SIG_);
    Mesh_Quality::Logger::Get().BeginSession("Main Logging");
    Mesh_Quality::Logger::Get().SetLogLevel(Mesh_Quality::LogLevel::Warn);
    #ifdef DEBUG
        Mesh_Quality::Logger::Get().Info("Running in DEBUG MODE");
    #else
        Mesh_Quality::Logger::Get().Info("Running in RELEASE MODE");
    #endif
    // All code goes in here

    // Example on how to use the Logger
    // Mesh_Quality::Logger::Get().Info("This is an info");
    // Mesh_Quality::Logger::Get().Warn("This", "is", "a", "warning");
    // Mesh_Quality::Logger::Get().Error("This is an info", "and i can print everything", 123, M_1_PI);
    // Mesh_Quality::Logger::Get().Fatal("Not everything", "but if the type implements << i can print it");

    // Profile a scope like this
    // {
    //     PROFILE_SCOPE("A SCOPE THAT I PROFILE");
    //     int a = 1 + 2;
    // }
    std::string file = "circle_2d.msh";
    if(argc == 2){
        file = std::string(argv[1]);
    }
    
    try{
        Mesh_Quality::GmshHandler::Get().LoadMesh(file);
        auto mesh = Mesh_Quality::GmshHandler::Get().GetExplicitMesh();
        auto areas = mesh.Area();
        Mesh_Quality::GmshHandler::Get().DisplayElementData("Area", areas);
        Mesh_Quality::GmshHandler::Get().Display();
    }
    catch(Mesh_Quality::MqException& e){
        Mesh_Quality::Logger::Get().Error("An Error occured in the module:", e.what());
    }
    catch(const std::exception& e){
        Mesh_Quality::Logger::Get().Fatal("An exception Occured that is not related to the Mesh Qualityu module:", e.what());
    }

    timer.~InstrumentationTimer();
    END_PROFILING_SESSION();
}
