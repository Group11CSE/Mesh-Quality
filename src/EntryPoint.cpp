#include <iostream>
#include <gmsh.h>
#include <pch.h>

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
    Mesh_Quality::Logger::Get().BeginSession("Main Logging");
    #ifdef DEBUG
        Mesh_Quality::Logger::Get().Info("Running in DEBUG MODE");
    #else
        Mesh_Quality::Logger::Get().Info("Running in RELEASE MODE");
    #endif
    // All code goes in here

    // Example on how to use the Logger
    Mesh_Quality::Logger::Get().Info("This is an info");
    Mesh_Quality::Logger::Get().Warn("This", "is", "a", "warning");
    Mesh_Quality::Logger::Get().Error("This is an info", "and i can print everything", 123, M_1_PI);
    Mesh_Quality::Logger::Get().Fatal("Not everything", "but if the type implements << i can print it");

    // Profile a scope like this
    {
        PROFILE_SCOPE("A SCOPE THAT I PROFILE");
        int a = 1 + 2;
    }
    

    // GMSH is basically a state machine, which makes interfacing with C++
    // Kinda akward, so we have to first abstract the mesh object, in order to keep it
    // OOP
    gmsh::initialize();
    gmsh::option::setNumber("General.Terminal", 1);

    gmsh::open("example.msh");
    gmsh::model::mesh::refine();
    gmsh::fltk::run();


    gmsh::finalize();

    END_PROFILING_SESSION();
}