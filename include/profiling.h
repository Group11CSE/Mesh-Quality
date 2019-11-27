
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

/**
 * Scope based profiling, for use with the Chrome
 * Tracing tool, about:tracing
 * Written originally by Cherno
 * https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e
 * 
 * Modified by Malte Wegener
 * 
 * */


#pragma once
#define PROFILING

// Preprocessor definitons for function signature
// and only doping profiling if one of the major compilers is used
#ifdef __GNUC__
    #define _CUSTOM_FUNC_SIG_ __PRETTY_FUNCTION__
#elif __clang__
    #define _CUSTOM_FUNC_SIG_ __PRETTY_FUNCTION__
#elif _MSC_VER
    #define _CUSTOM_FUNC_SIG_ __FUNCSIG__
#else
    #undef PROFILING
#endif

#ifndef PROFILING
#define START_PROFILING_SESSION(NAME)
#define END_PROFILING_SESSION()
#define PROFILE_FUNCTION
#define PROFILE_SCOPE(NAME)
#endif

#ifdef PROFILING

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

#define START_PROFILING_SESSION(NAME) Instrumentor::Get().BeginSession(NAME)
#define END_PROFILING_SESSION() Instrumentor::Get().EndSession()

#define PROFILE_FUNCTION InstrumentationTimer timer(_CUSTOM_FUNC_SIG_)
#define PROFILE_SCOPE(NAME) InstrumentationTimer timer(NAME)

struct ProfileResult{
    std::string Name;
    long long Start, End;
    uint32_t ThreadID;
};

struct InstrumentationSession{
    std::string Name;
};

class Instrumentor{
private:
    InstrumentationSession* m_CurrentSession;
    std::ofstream m_OutputStream;
    int m_ProfileCount;
public:
    Instrumentor()
        : m_CurrentSession(nullptr), m_ProfileCount(0){
    }

    void BeginSession(const std::string& name, const std::string& filepath = "profiling.json"){
        m_OutputStream.open(filepath);
        WriteHeader();
        m_CurrentSession = new InstrumentationSession{ name };
    }

    void EndSession(){
        WriteFooter();
        m_OutputStream.close();
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
        m_ProfileCount = 0;
    }

    void WriteProfile(const ProfileResult& result){
        if (m_ProfileCount++ > 0)
            m_OutputStream << ",";

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{";
        m_OutputStream << "\"cat\":\"function\",";
        m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
        m_OutputStream << "\"name\":\"" << name << "\",";
        m_OutputStream << "\"ph\":\"X\",";
        m_OutputStream << "\"pid\":0,";
        m_OutputStream << "\"tid\":" << result.ThreadID << ",";
        m_OutputStream << "\"ts\":" << result.Start;
        m_OutputStream << "}";

        m_OutputStream.flush();
    }

    void WriteHeader(){
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        m_OutputStream.flush();
    }

    void WriteFooter(){
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

    static Instrumentor& Get(){
        static Instrumentor instance;
        return instance;
    }
};

class InstrumentationTimer{
public:
    InstrumentationTimer(const char* name)
        : m_Name(name), m_Stopped(false){
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer(){
        if (!m_Stopped)
            Stop();
    }

    void Stop(){
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

        m_Stopped = true;
    }
private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    bool m_Stopped;
};
#endif