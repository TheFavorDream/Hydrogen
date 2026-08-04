#include "Hydrofiler.h"



namespace Tools
{


	Profile             Hydrofiler::m_LatestProfile;
	std::stack<Profile> Hydrofiler::m_Profiles;


	void Hydrofiler::StartProfiling(const std::string & pBlockName)
	{

		Profile NewProfile;
		NewProfile.BlockName = pBlockName;
		m_Profiles.push(NewProfile);

		m_Profiles.top().StartPoint = std::chrono::steady_clock::now();

	}

	Profile Hydrofiler::StopProfiling()
	{
		//Get Latest profile
		std::chrono::time_point<std::chrono::steady_clock> Current = std::chrono::steady_clock::now();

		m_LatestProfile = m_Profiles.top();
		m_Profiles.pop();

		//duration in microseconds
		m_LatestProfile.StopPoint = Current;

		return m_LatestProfile;
	}

	void Hydrofiler::ClearProfiles()
	{
		
		while (m_Profiles.empty() != true)
			m_Profiles.pop();
		
	}

	//Duration casts
	uint32_t Hydrofiler::GetDurationInSeconds(const Profile& pProfile)
	{
		uint32_t Duration = std::chrono::duration_cast<std::chrono::seconds>(pProfile.StopPoint - pProfile.StartPoint).count();
		return Duration;
	}

	uint32_t Hydrofiler::GetDurationInMilliSeconds(const Profile& pProfile)
	{
		uint32_t Duration = std::chrono::duration_cast<std::chrono::milliseconds>(pProfile.StopPoint - pProfile.StartPoint).count();
		return Duration;
	}

	uint32_t Hydrofiler::GetDurationInMicroSeconds(const Profile& pProfile)
	{
		uint32_t Duration = std::chrono::duration_cast<std::chrono::microseconds>(pProfile.StopPoint - pProfile.StartPoint).count();
		return Duration;
	}



};