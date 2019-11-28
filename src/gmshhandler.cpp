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

#include <gmshhandler.h>

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

    GmshHandler::GmshHandler(){
        PROFILE_FUNCTION;
        Logger::Get().Info("Initializing GMSH");
        gmsh::initialize();
        gmsh::option::setNumber("General.Terminal", 2);
        m_gmsh_init = true;
    }
    GmshHandler::~GmshHandler(){
        PROFILE_FUNCTION;
        Logger::Get().Info("Finalizing GMSH");
        gmsh::finalize();
        m_gmsh_init = false;
    }

    std::pair<std::vector<std::size_t>, std::vector<std::vector<double>>>
    GmshHandler::getKeysValues(const std::map<std::size_t, double> &f){
        std::vector<std::size_t> keys;
        std::vector<std::vector<double>> values;
        keys.clear();
        values.clear();
        for(std::map<std::size_t, double>::const_iterator it = f.begin();
            it != f.end(); it++){
            keys.push_back(it->first);
            values.push_back(std::vector<double>(1, it->second));
        }

        return std::make_pair<>(keys, values);
    }

    void GmshHandler::LoadMesh(const std::string& filepath){
        PROFILE_FUNCTION;
        Logger::Get().Info("Loading Mesh from file:", filepath);
        gmsh::open(filepath);
        m_name = filepath;
        m_meshloaded = true;
    }

    void GmshHandler::RefineMesh(int n){
        PROFILE_FUNCTION;
        MQ_CHECK_STATUS_VALID;
        for(int i = 0; i < n; i++){
            gmsh::model::mesh::refine();
        }
    }

    void GmshHandler::Display(){
        MQ_CHECK_STATUS_VALID;
        m_meshloaded = false;
        gmsh::fltk::run();
    }

    // As a mesh can be quite big we want it to be heap allocated
    // Raw pointers are very dangerous for working so a shared pointer is used
    Mesh GmshHandler::GetExplicitMesh(){
        PROFILE_FUNCTION;
        MQ_CHECK_STATUS_VALID;
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

    void GmshHandler::DisplayElementData(const std::string& name, const std::map<std::size_t, double> data){
        PROFILE_FUNCTION;
        MQ_CHECK_STATUS_VALID;

        int data_view = gmsh::view::add(name);
        auto data_pair = getKeysValues(data);
        std::vector<std::string> names;
        gmsh::model::list(names);

        gmsh::vectorpair dimtag;
        gmsh::model::getEntities(dimtag);

        gmsh::view::addModelData(data_view, 0, names[0], "ElementData", data_pair.first, data_pair.second);
        
    }
    void GmshHandler::RunDisplay(){
        m_meshloaded = false;
        gmsh::fltk::run();
    }
}

#undef MQ_CHECK_STATUS_VALID