#pragma once

#include <pch.h>

namespace Mesh_Quality{

    class NotSupportedException: MqException{
        public: const char* what(){return "The requested functionality is not supported";};
    };
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
        double Area(const std::map<std::size_t, std::unique_ptr<Vertex>>& vertices){
            PROFILE_FUNCTION;
            if(m_tags.size() == 4){
                const double x1 = vertices.at(m_tags[0])->m_x;
                const double y1 = vertices.at(m_tags[0])->m_y;
                const double x2 = vertices.at(m_tags[1])->m_x;
                const double y2 = vertices.at(m_tags[1])->m_y;
                const double x3 = vertices.at(m_tags[2])->m_x;
                const double y3 = vertices.at(m_tags[2])->m_y;
                const double x4 = vertices.at(m_tags[3])->m_x;
                const double y4 = vertices.at(m_tags[3])->m_y;

                double area = 0.5*(x1*y2+x2*y3+x3*y4+x4*y1-x2*y1-x3*y2-x4*y3-x1*y4);
                return m_tags[0];
            }
            else{
                throw NotSupportedException();
            }
        };
    private:
        int m_type;
        std::vector<std::size_t> m_tags;
    };
}