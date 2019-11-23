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
        for(int i = 0; i < nodetags.size(); i++){
            m_vertices.insert({nodetags[i], std::make_unique<Vertex>(nodeCoords[3*i], nodeCoords[3*i+2], nodeCoords[3*i+2])});
        }

        // Populating the Element Vector, this has to be a nested loop, as gmsh gives data that way
        // If somebody has a better idea hwo to do this please be my guest
        // INFO: May inmprove may not touch ever again
        for(int i = 0; i < elementTypes.size(); i++){
            for(int j = 0; j < elementTags[i].size(); j++){
                int n = nodeTagsElems[i].size()/elementTags[i].size();
                std::vector<std::size_t> tags(n);
                for(int k = 0; k < n; k++){
                    tags[k] = nodeTagsElems[i][n*j+k];
                }
                m_elements.insert({elementTags[i][j], std::make_unique<Element>(elementTypes[i], tags)});
            }
        }

    }
}