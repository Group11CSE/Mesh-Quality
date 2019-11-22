// Every major compielr supports this, I wont use a classic guard as the only possibility for this
// failing would b ein a build enviroment that copies files a lot, whic does not happen
#pragma once
#include <pch.h>
#include <mesh.h>

/**
 * Class to wrap the gmsh interface and abstract operations
 * \author Malte Wegener
 * 
 * This class should be a singletone as gmsh is working as a state machine
 * so constructing this class should prevent creation of another until it is finalized
 * 
 * Goal is to abstract all the gmsh code so far away that the actual Code for a user is
 * easy to use and expressive C++ template code, everything that is not necessary for Mesh
 * Qaulity should not be accesible
 */

namespace Mesh_Quality{
    class NoMeshLoadedException: std::exception{
    public:
        const char* what(){
            return "No Mesh is loaded in the module";
        }
    };


    class GmshHandler{
    private:
        bool m_meshloaded = false;
        // Disallow instantiation outside of the class
        GmshHandler(){
            PROFILE_FUNCTION;
            Logger::Get().Info("Constructor Called in Class Mesh");
            gmsh::initialize();
            gmsh::option::setNumber("General.Terminal", 2);
        }
        ~GmshHandler(){
            PROFILE_FUNCTION;
            Logger::Get().Info("Deconstructor Called in Class Mesh");
            gmsh::finalize();
        }

    public:
        // Cant copy, assign
        GmshHandler(const GmshHandler&) = delete;
        GmshHandler& operator=(const GmshHandler&) = delete;
        GmshHandler(GmshHandler &&) = delete;
        GmshHandler & operator=(GmshHandler &&) = delete;

        static GmshHandler& Get(){
            PROFILE_FUNCTION;
            static GmshHandler GmshHandler;
            return GmshHandler;
        }

        void LoadMesh(const std::string& filepath){
            PROFILE_FUNCTION;
            Logger::Get().Info("Loading Mesh from file:", filepath);
            gmsh::open(filepath);
            m_meshloaded = true;
        }

        void RefineMesh(int n){
            PROFILE_FUNCTION;
            if (!m_meshloaded){
                throw NoMeshLoadedException();
            }
            for(int i = 0; i < n; i++){
                gmsh::model::mesh::refine();
            }
        }

        void Display(){
            if (!m_meshloaded){
                throw NoMeshLoadedException();
            }
            m_meshloaded = false;
            gmsh::fltk::run();
        }

        // As a mesh can be quite big we want it to be heap allocated
        // Raw pointers are very dangerous for working so a shared pointer is used
        Mesh GetExplicitMesh(){
            PROFILE_FUNCTION;
            if (!m_meshloaded){
                throw NoMeshLoadedException();
            }
            Logger::Get().Info("Creating Mesh pointer");
            Mesh mesh;
            Logger::Get().Info("Created Mesh pointer");
            gmsh::model::mesh::getNodes(mesh.m_nodeTags, mesh.m_nodeCoords, mesh.m_nodeParaCoords);
            Logger::Get().Info("Filled Node Info");
            gmsh::model::mesh::getElements(mesh.m_elementTypes, mesh.m_elementTags, mesh.m_nodeTagsElems);
            Logger::Get().Info("Filled Element Info");
            return mesh;
        }
    }; 
}