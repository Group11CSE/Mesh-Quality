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
            PROFILE_FUNCTION;
            m_OutputStream << Message << std::endl;
            if(level >= m_level){
                std::cout << Message << std::endl;
            }
        }

        // Variadic template recursion to assemble a message from parameters
        // Only works with C++17 or higher
        template <class T, class... Ts>
        void assembler(std::stringstream &message, T const &first, Ts const&... rest) {
            PROFILE_FUNCTION;
            message << first;
             if constexpr (sizeof...(rest) > 0) {
                 message << " ";
                 assembler(message, rest...);
             }
        }

        void WriteHeader(){
            PROFILE_FUNCTION;
            m_OutputStream << "Hello and Welcome to the " << m_CurrentSession->Name << " Session" << std::endl;
        }
    public:
        Logger()
            : m_CurrentSession(nullptr), m_ProfileCount(0){
        }

        void BeginSession(const std::string& name, const std::string& filepath = "Mesh_Quality.log"){
            PROFILE_FUNCTION;
            m_OutputStream.open(filepath);
            m_CurrentSession = new LoggingSession{ name };
            WriteHeader();
        }

        void EndSession(){
            PROFILE_FUNCTION;
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
            m_ProfileCount = 0;
        }
        void SetLogLevel(LogLevel level){
            PROFILE_FUNCTION;
            m_level = level;
        }

        template <class... Ts>
        void Info( Ts const&... args){
            PROFILE_FUNCTION;
            std::stringstream message;
            message << "INFO_MQ:\t";
            assembler(message, args...);
            Log(message.str(), LogLevel::Info);
        }

        template <class... Ts>
        void Warn(Ts const&... args){
            PROFILE_FUNCTION;
            std::stringstream message;
            message << "WARNING_MQ:\t";
            assembler(message, args...);
            Log(message.str(), LogLevel::Warn);
        }

        template <class... Ts>
        void Error(Ts const&... args){
            PROFILE_FUNCTION;
            std::stringstream message;
            message << "ERROR_MQ:\t";
            assembler(message, args...);
            Log(message.str(), LogLevel::Error);
        }

        template <class... Ts>
        void Fatal(Ts const&... args){
            PROFILE_FUNCTION;
            std::stringstream message;
            message << "FATAL_MQ:\t";
            assembler(message, args...);
            Log(message.str(), LogLevel::Fatal);
        }

        static Logger& Get(){
            PROFILE_FUNCTION;
            static Logger instance;
            return instance;
        }
    };
}