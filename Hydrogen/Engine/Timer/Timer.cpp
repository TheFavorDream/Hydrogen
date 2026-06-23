#include "Timer.h"
#include "HydPch.h"


namespace Hydrogen
{

	void Timer::StartTimer()
	{
		ResetTimer();
		m_StartTime = std::chrono::steady_clock::now();
	}

	void Timer::StopTimer()
	{
		std::chrono::time_point<std::chrono::steady_clock> Current = std::chrono::steady_clock::now();
		m_Elapsed = (uint32)std::chrono::duration_cast<std::chrono::milliseconds>(Current - m_StartTime).count();
	}

	void Timer::ResetTimer()
	{
		m_Elapsed = 0;
	}

#ifndef DIST
	std::stack<Profiler> Profiler::s_Profiles;

	void Profiler::StartProfile(const std::string & pBlockName)
	{
		s_Profiles.push(Profiler(pBlockName));
		s_Profiles.top().StartTimer();
	}

	void Profiler::StopProfile()
	{
		s_Profiles.top().StopTimer();
		Log::SetInfo(Log::FmtStr("%s Took %ims", s_Profiles.top().GetName().c_str(), s_Profiles.top().GetElapsedInMillis()));
		s_Profiles.pop();
	}

	Profiler::Profiler(const std::string& pBlockName):
		m_BlockName(pBlockName)
	{

	}

#endif

};