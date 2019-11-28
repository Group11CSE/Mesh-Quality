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
        std::map<std::size_t, double> AspectRatio();
        friend std::ostream& operator<<(std::ostream& os, const Mesh& mesh);
    private:
        std::string m_name = "No Name";
        std::map<std::size_t, std::shared_ptr<Vertex>> m_vertices;
        std::map<std::size_t, std::shared_ptr<Element>> m_elements;


    };

    std::ostream& operator<<(std::ostream& os, const Mesh& mesh);
}