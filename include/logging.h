/**
 * Header file for logging
 * \author Malte Wegener
*/


#pragma once


#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <profiling.h>

namespace Mesh_Quality{
    struct ProfileResult{
        std::string Name;
        long long Start, End;
        uint32_t ThreadID;
    };

    struct LoggingSession{
        std::string Name;
    };

    enum LogLevel{
        Info,
        Warn,
        Error,
        Fatal
    };

    class Logger{
    private:
        LoggingSession* m_CurrentSession;
        std::ofstream m_OutputStream;
        int m_ProfileCount;
        LogLevel m_level = LogLevel::Warn;
        void Log(const std::string& Message, LogLevel level){
            m_OutputStream << Message << std::endl;
            if(level >= m_level){
                std::cout << Message << std::endl;
            }
        }

        // Variadic template recursion to assemble a message from parameters
        // Only works with C++17 or higher
        template <class T, class... Ts>
        void assembler(std::stringstream &message, T const &first, Ts const&... rest) {
            message << first;
             if constexpr (sizeof...(rest) > 0) {
                 message << " ";
                 assembler(message, rest...);
             }
        }

        void WriteHeader(){
            m_OutputStream << "Hello and Welcome to the " << m_CurrentSession->Name << " Session" << std::endl;
        }
    public:
        Logger()
            : m_CurrentSession(nullptr), m_ProfileCount(0){
        }

        void BeginSession(const std::string& name, const std::string& filepath = "Mesh_Quality.log"){
            m_OutputStream.open(filepath);
            m_CurrentSession = new LoggingSession{ name };
            WriteHeader();
        }

        void EndSession(){
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
            m_ProfileCount = 0;
        }
        void SetLogLevel(LogLevel level){
            m_level = level;
        }

        template <class... Ts>
        void Info( Ts const&... args){
            PROFILE_FUNCTION;
            std::stringstream message;
            message << "INFO:\t\t";
            assembler(message, args...);
            Log(message.str(), LogLevel::Info);
        }

        template <class... Ts>
        void Warn(Ts const&... args){
            PROFILE_FUNCTION;
            std::stringstream message;
            message << "WARNING:\t";
            assembler(message, args...);
            Log(message.str(), LogLevel::Warn);
        }

        template <class... Ts>
        void Error(Ts const&... args){
            PROFILE_FUNCTION;
            std::stringstream message;
            message << "ERROR:\t\t";
            assembler(message, args...);
            Log(message.str(), LogLevel::Error);
        }

        template <class... Ts>
        void Fatal(Ts const&... args){
            PROFILE_FUNCTION;
            std::stringstream message;
            message << "FATAL:\t\t";
            assembler(message, args...);
            Log(message.str(), LogLevel::Fatal);
        }

        static Logger& Get(){
            static Logger instance;
            return instance;
        }
    };
}