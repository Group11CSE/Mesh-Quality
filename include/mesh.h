#pragma once

#include <pch.h>

namespace Mesh_Quality{
    class Mesh{
    public:
        Mesh(){
            Logger::Get().Info("Creating a mesh");
        };
        std::string m_name = "Test name";
        std::vector<std::size_t> m_nodeTags;
        std::vector<double> m_nodeCoords;
        std::vector<double> m_nodeParaCoords;
        std::vector<int> m_elementTypes;
        std::vector<std::vector<std::size_t>> m_elementTags;
        std::vector<std::vector<std::size_t>> m_nodeTagsElems;
        friend std::ostream& operator<<(std::ostream& os, const Mesh& dt);
    };
    std::ostream& operator<<(std::ostream& os, const Mesh& m){

        // remember current flags, precision
        auto flags = os.flags();
        auto current_precision = os.precision();
        // output numers with fixed point and three decimal precision
        os.setf(std::ios::fixed, std::ios::floatfield);
        os.precision(5);    
        PROFILE_FUNCTION;
        os << "Displaying Mesh with name: " << m.m_name << std::endl;
        os << "--------------------------Node Tags----------------------" << std::endl;
        for(const auto &i : m.m_nodeTags){
            os << i << std::endl;
        }
        int c = 0;
        os << "--------------------------Node Coords--------------------" << std::endl;
        for(auto i = m.m_nodeCoords.begin(); i != m.m_nodeCoords.end();){
            os << c << ":\tx: " << *i << "\t";
            ++i;
            os << ", y: " << *i << "\t";
            ++i;
            os << ", z: " << *i << std::endl;
            ++i;
            c++;
        }
        c = 0;
        os << "---------------------Node Parametric Coords--------------" << std::endl;
        for(auto i = m.m_nodeParaCoords.begin(); i != m.m_nodeParaCoords.end();){
            os << c << ":\tx: " << *i << "\t";
            ++i;
            os << ", y: " << *i << "\t";
            ++i;
            os << ", z: " << *i << std::endl;
            ++i;
            c++;
        }
        os << "-------------------------Element Types-------------------" << std::endl;
        for(const auto &i : m.m_elementTypes){
            os << i << std::endl;
        }

        os << "-------------------------Element Tags--------------------" << std::endl;
        for(const auto &i : m.m_elementTags){
            for(const auto& j : i){
                os << j << ", ";
            }
            os << std::endl;
        }

        os << "----------------------Element Node Tags------------------" << std::endl;
        for(const auto &i : m.m_elementTags){
            for(const auto& j : i){
                os << j << ", ";
            }
            os << std::endl;
        }
        os.flags(flags);
        os.precision(current_precision);
        return os;
    }
}