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
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>



typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon_2;

namespace Mesh_Quality{

    class NotSupportedException: MqException{
    public:
        const char* what() const noexcept {return "The requested functionality is not supported";};
    };
    class Vertex{
    public:
        Vertex(double x, double y, double z): m_x(x), m_y(y), m_z(z){};
        friend std::ostream& operator<<(std::ostream& os, const Vertex& vert);
        double m_x;
        double m_y;
        double m_z; 
    };

    std::ostream& operator<<(std::ostream& os, const Vertex& vert);

    class Element{
    public:
        Element(int type, std::vector<std::size_t> tags): m_type(type), m_tags(tags){};
        int GetType(){return m_type;};

        // Defining custom iterators to make code more expressive, we can only ever
        // iterate over the tags of the Element, because everything else is not 
        // an iterable variable
        inline std::vector<std::size_t>::iterator begin() noexcept { return m_tags.begin(); }
        inline std::vector<std::size_t>::const_iterator cbegin() const noexcept { return m_tags.cbegin(); }
        inline std::vector<std::size_t>::iterator end() noexcept { return m_tags.end(); }
        inline std::vector<std::size_t>::const_iterator cend() const noexcept { return m_tags.cend(); }


        // TODO: Implem,ent a function that returns skewness, aspect ratio etc
        double Area(const std::map<std::size_t, std::shared_ptr<Vertex>>& vertices){
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
    private:
        int m_type;
        std::vector<std::size_t> m_tags;
    };
}