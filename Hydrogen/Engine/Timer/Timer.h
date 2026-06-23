#pragma once

#include "Common.h"
#include "Log/Log.h"
#include <chrono>


namespace Hydrogen
{

	class Timer
	{
	public:

		HYD void StartTimer();
		HYD void StopTimer();

		HYD void ResetTimer();

		HYD inline uint32 GetElapsedInMillis() { return m_Elapsed; }

	private:
		//Time in Milliseconds
		uint32 m_Elapsed = 0;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
	};


#ifndef DIST
	//Use to measure how long it takes for a block of code to be excuted
	class Profiler : public Timer
	{
	public:

		HYD static void StartProfile(const std::string& pBlockName);
		HYD static void StopProfile();

		HYD Profiler(const std::string& pBlockName);

		HYD inline std::string& GetName() { return m_BlockName; }
	private:
		std::string m_BlockName;
	private:
		static std::stack<Profiler> s_Profiles;
	};
#endif
};


#ifndef DIST
#define PROFILE_FUNC(func) Hydrogen::Profiler::StartProfile(#func); func; Hydrogen::Profiler::StopProfile();
#define PROFILE_START(s)   Hydrogen::Profiler::StartProfile(s);
#define PROFILE_STOP       Hydrogen::Profiler::StopProfile();
#else
#define PROFILE_FUNC(func) func;
#define PROFILE_START(s)
#define PROFILE_STOP
#endif