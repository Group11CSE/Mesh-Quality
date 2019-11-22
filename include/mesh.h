#include <pch.h>

/**
 * Class to wrap the gmsh interface and abstract operations
 * \author Malte Wegener
 * 
 * This class should be a singletone as gmsh is working as a state machine
 * so constructing this class should prevent creation of another until it is finalized
 */

namespace Mesh_Quality{
    class NoMeshLoadedException: std::exception{
    public:
        const char* what(){
            return "No Mesh is loaded in the module";
        }
    };


    class Mesh{
    private:
        bool m_meshloaded = false;
        Mesh(){
            Logger::Get().Info("Constructor Called in Class Mesh");
            gmsh::initialize();
            gmsh::option::setNumber("General.Terminal", 2);
        }; // Disallow instantiation outside of the class.
        ~Mesh(){
            Logger::Get().Info("Deconstructor Called in Class Mesh");
            gmsh::finalize();
        }
    public:
        // Cant copy, assign
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        Mesh(Mesh &&) = delete;
        Mesh & operator=(Mesh &&) = delete;

        static Mesh& Get(){
            static Mesh Mesh;
            return Mesh;
        }

        void LoadMesh(const std::string& filepath){
            Logger::Get().Info("Loading Mesh from file:", filepath);
            gmsh::open(filepath);
            m_meshloaded = true;
        }

        void RefineMesh(int n){
            if (!m_meshloaded){
                throw NoMeshLoadedException();
            }
            if (n>0){
                gmsh::model::mesh::refine();
                RefineMesh(n-1);
            }
        }

        void Display(){
            if (!m_meshloaded){
                throw NoMeshLoadedException();
            }
            m_meshloaded = false;
            gmsh::fltk::run();
        }
    }; 
}