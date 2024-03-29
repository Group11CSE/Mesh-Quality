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

#include <mesh.h>

namespace Mesh_Quality{
    Mesh::Mesh( std::string& name,
                std::vector<std::size_t>& nodetags,
                std::vector<double>& nodeCoords,
                std::vector<int>& elementTypes,
                std::vector<std::vector<std::size_t>>& elementTags,
                std::vector<std::vector<std::size_t>>& nodeTagsElems){
        
        PROFILE_FUNCTION;

        m_name = name;

        // Populating the vertex vector
        for(unsigned int i = 0; i < nodetags.size(); i++){
            m_vertices.insert({nodetags[i], std::make_shared<Vertex>(nodeCoords[3*i], nodeCoords[3*i+1], nodeCoords[3*i+2])});
        }

        // Populating the Element Vector, this has to be a nested loop, as gmsh gives data that way
        // If somebody has a better idea hwo to do this please be my guest
        // INFO: May inmprove may not touch ever again
        for(unsigned int i = 0; i < elementTypes.size(); i++){
            for(unsigned int j = 0; j < elementTags[i].size(); j++){
                unsigned int n = nodeTagsElems[i].size()/elementTags[i].size();
                std::vector<std::size_t> tags(n);
                for(unsigned int k = 0; k < n; k++){
                    tags[k] = nodeTagsElems[i][n*j+k];
                }
                m_elements.insert({elementTags[i][j], std::make_shared<Element>(elementTypes[i], tags)});
            }
        }

    }
    
    std::map<std::size_t, double> Mesh::Area(){
        PROFILE_FUNCTION;

        std::map<std::size_t, double> areas;
        for(auto const& element: m_elements){
            try{
                areas[element.first] = element.second->Area(m_vertices);
            }
            catch(std::exception& e){
                throw;
            }
        }
        return areas;
    }

    std::map<std::size_t, double> Mesh::AspectRatio(){
        PROFILE_FUNCTION;

        std::map<std::size_t, double> areas;
        for(auto const& element: m_elements){
            try{
                areas[element.first] = element.second->AspectRatio(m_vertices);
            }
            catch(std::exception& e){
                Logger::Get().Warn("No aspect ratio implemented for anything else than triangles or quadrilaterals");
            }
        }
        return areas;
    }

    std::ostream& operator<<(std::ostream& os, const Mesh& mesh){
        os << mesh.m_name << std::endl;
        os << "-------Vertices-------" << std::endl;

        for(const auto& vert: mesh.m_vertices){
            os << "Vertex: Tag: " << vert.first << " Position: " << *(vert.second) << std::endl;
        }
        return os;
    }
}