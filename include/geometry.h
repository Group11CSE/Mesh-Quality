#pragma once

#include <pch.h>

namespace Mesh_Quality{
    class Vertex{
    public:
        Vertex(double x, double y, double z): m_x(x), m_y(y), m_z(z){};
        double m_x;
        double m_y;
        double m_z; 
    };

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
    private:
        int m_type;
        std::vector<std::size_t> m_tags;
    };
}