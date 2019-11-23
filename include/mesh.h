#pragma once

#include <pch.h>
#include <map>
#include <geometry.h>

namespace Mesh_Quality{

    /**
     * Container to save a mesh
     */
    class Mesh{
    public:
        Mesh(   std::string& name,
                std::vector<int> Nodetags,
                std::vector<double> nodeCoords,
                std::vector<int> m_elementTypes,
                std::vector<std::vector<std::size_t>> elementTags,
                std::vector<std::vector<std::size_t>> nodeTagsElems);
        
    private:
        std::string m_name = "No Name";
        std::map<std::size_t, std::unique_ptr<Vertex>> Vertices;
        std::map<std::size_t, std::unique_ptr<Element>> Elements;


    };
}