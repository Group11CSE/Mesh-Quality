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

// Every major compielr supports this, I wont use a classic guard as the only possibility for this
// failing would b ein a build enviroment that copies files a lot, whic does not happen
#pragma once
#include <pch.h>
#include <mesh.h>

#define MQ_CHECK_STATUS_VALID if (!m_meshloaded){throw NoMeshLoadedException();}if (!m_gmsh_init){throw GmshNotInitializedException();}

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
    class NoMeshLoadedException: MqException{
    public:
        const char* what() const noexcept{
            return "No Mesh is loaded in the module";
        }
    };

    class GmshNotInitializedException: MqException{
    public:
        const char* what() const noexcept{
            return "Gmsh isnt initialized";
        }
    };


    class GmshHandler{
    private:
        bool m_meshloaded = false;
        bool m_gmsh_init= false;
        std::string m_name = "Empty Name";
        // Disallow instantiation outside of the class
        GmshHandler();
        ~GmshHandler();

        std::pair<std::vector<std::size_t>, std::vector<std::vector<double>>>
        getKeysValues(const std::map<std::size_t, double> &f);
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

        void LoadMesh(const std::string& filepath);
        void RefineMesh(int n);
        void Display();
        Mesh GetExplicitMesh();
        void DisplayElementData(const std::string& name, const std::map<std::size_t, double> data);
        void RunDisplay();
    }; 
}

#undef MQ_CHECK_STATUS_VALID