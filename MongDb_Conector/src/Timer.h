#pragma once
#include <iostream>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <thread>
#include <Windows.h>
struct ProfileResult
{
	std::string name;
	long long start, end;
	uint32_t threadID;
	DWORD pid;
};


class Instrumentor
{
public:
	Instrumentor()
		: m_ProfileCount(0)
	{
	}

	void BeginSession(const std::string& filepath = "results")
	{
		m_OutputStream.open(filepath + ".json");
		WriteHeader();
	}

	void EndSession()
	{
		WriteFooter();
		m_OutputStream.close();
		m_ProfileCount = 0;
	}

	void WriteProfile(const ProfileResult& result)
	{
		if (m_ProfileCount++ > 0)
			m_OutputStream << ",";

		std::string name = result.name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_OutputStream << "{";
		m_OutputStream << "\"cat\":\"function\",";
		m_OutputStream << "\"dur\":" << (result.end - result.start) << ',';
		m_OutputStream << "\"name\":\"" << name << "\",";
		m_OutputStream << "\"ph\":\"X\",";
		m_OutputStream << "\"pid\":" << result.pid << ",";
		m_OutputStream << "\"tid\":" << result.threadID << ",";
		m_OutputStream << "\"ts\":" << result.start;
		m_OutputStream << "}";

		m_OutputStream.flush();
	}

	void WriteHeader()
	{
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
		m_OutputStream.flush();
	}

	void WriteFooter()
	{
		m_OutputStream << "]}";
		m_OutputStream.flush();
	}
	static Instrumentor& Get() 
	{
		static Instrumentor *instance = new Instrumentor();
		return *instance;
	}
private:
	std::ofstream m_OutputStream;
	int m_ProfileCount;
};

class InstrumentationTimer
{
public:
	InstrumentationTimer(const char* name)
	: m_Name(name), m_Stopped(false)
	{
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}

	~InstrumentationTimer() 
	{
		if(!m_Stopped)
			Stop();
	}

	void Stop() 
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		auto duration = end - start;
		double ms = duration * 0.001;

		uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Instrumentor::Get().WriteProfile({m_Name, start, end, threadID, GetCurrentProcessId()});
		m_Stopped = true;
	}
private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
	bool m_Stopped;
};