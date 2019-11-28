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

#include <geometry.h>


namespace Mesh_Quality{
    std::ostream& operator<<(std::ostream& os, const Vertex& vert){
        os << "x: " << vert.m_x << ", y: " << vert.m_y << ", z: " << vert.m_x;
        return os;
    }

    double Element::Area(const std::map<std::size_t, std::shared_ptr<Vertex>>& vertices){
            PROFILE_FUNCTION;
            std::vector<Point> points;

            for(const auto &tag: m_tags){
                auto p = vertices.find(tag);
                if(p == vertices.end()) throw NotSupportedException();
                double x = p->second->m_x;
                double y = p->second->m_y;
                points.push_back(Point(x, y));
            }

            Polygon_2 polygon (points.begin(), points.end());
            double area = polygon.area();
            return area;
        };
}