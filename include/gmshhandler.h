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

    class GmshNotInitializedException: std::exception{
    public:
        const char* what(){
            return "Gmsh isnt initialized";
        }
    };


    class GmshHandler{
    private:
        bool m_meshloaded = false;
        bool m_gmsh_init= false;
        std::string m_name = "Empty Name";
        // Disallow instantiation outside of the class
        GmshHandler(){
            PROFILE_FUNCTION;
            Logger::Get().Info("Initializing GMSH");
            gmsh::initialize();
            gmsh::option::setNumber("General.Terminal", 2);
            m_gmsh_init = true;
        }
        ~GmshHandler(){
            PROFILE_FUNCTION;
            Logger::Get().Info("Finalizing GMSH");
            gmsh::finalize();
            m_gmsh_init = false;
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
            m_name = filepath;
            m_meshloaded = true;
        }

        void RefineMesh(int n){
            PROFILE_FUNCTION;
            if (!m_meshloaded){
                throw NoMeshLoadedException();
            }
            if (!m_gmsh_init){
                throw GmshNotInitializedException();
            }
            for(int i = 0; i < n; i++){
                gmsh::model::mesh::refine();
            }
        }

        void Display(){
            if (!m_meshloaded){
                throw NoMeshLoadedException();
            }
            if (!m_gmsh_init){
                throw GmshNotInitializedException();
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
            if (!m_gmsh_init){
                throw GmshNotInitializedException();
            }
            // Soem containers to store the different things a mesh consists of
            std::vector<std::size_t> nodeTags;
            std::vector<double> nodeCoords;
            std::vector<double> nodeParaCoords;
            std::vector<int> elementTypes;
            std::vector<std::vector<std::size_t>> elementTags;
            std::vector<std::vector<std::size_t>> nodeTagsElems;


            gmsh::model::mesh::getNodes(nodeTags, nodeCoords, nodeParaCoords);
            Logger::Get().Info("Filled Node Info");
            gmsh::model::mesh::getElements(elementTypes, elementTags, nodeTagsElems);
            Logger::Get().Info("Filled Element Info");
            Logger::Get().Info("Making mesh");
            Mesh mesh(m_name, nodeTags, nodeCoords, elementTypes, elementTags, nodeTagsElems);
            return mesh;
        }
    }; 
}