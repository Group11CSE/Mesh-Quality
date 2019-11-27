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
                std::vector<std::size_t>& nodetags,
                std::vector<double>& nodeCoords,
                std::vector<int>& elementTypes,
                std::vector<std::vector<std::size_t>>& elementTags,
                std::vector<std::vector<std::size_t>>& nodeTagsElems);
        
        std::map<std::size_t, double> Area();
        friend std::ostream& operator<<(std::ostream& os, const Mesh& mesh);
    private:
        std::string m_name = "No Name";
        std::map<std::size_t, std::shared_ptr<Vertex>> m_vertices;
        std::map<std::size_t, std::shared_ptr<Element>> m_elements;


    };


    std::ostream& operator<<(std::ostream& os, const Mesh& mesh);
}