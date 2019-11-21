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
    



    END_PROFILING_SESSION();
}