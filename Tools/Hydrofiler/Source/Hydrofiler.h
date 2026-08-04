/*

	Hydrofiler (Hydrogen Profiler)
	Part of Hydrogen project.

	Created by: Pouya Alizadeh aka Voltage
	Date: 2026/4/6


	Purpose:
		Hydrofiler provides functions and classes to make profiling and benchmarking eassy.
		Hydrofiler tend to be Fast and Easy To Use with minimum dependencies. 

*/

#pragma once

#include <chrono>
#include <iostream>
#include <fstream>
#include <stack>
#include <cstdint>

namespace Tools
{


	struct Profile
	{
		std::string											BlockName;
		std::chrono::time_point<std::chrono::steady_clock>  StartPoint;
		std::chrono::time_point<std::chrono::steady_clock>  StopPoint;

	};


	//Class Definition:
	class Hydrofiler
	{
	public:


		static void    StartProfiling(const std::string& pBlockName);
		static Profile StopProfiling();


		//Clears all profiles
		static void ClearProfiles();


		static inline  Profile GetLatestProfile() { 
			return m_LatestProfile;
		}


		static uint32_t GetDurationInSeconds(      const  Profile& pProfile);
		static uint32_t GetDurationInMilliSeconds(const  Profile& pProfile);
		static uint32_t GetDurationInMicroSeconds( const  Profile& pProfile);


	private:
		static Profile             m_LatestProfile;
		static std::stack<Profile> m_Profiles;
	};


};


//Macro Definition
#define START_PROFILE(s) Tools::Hydrofiler::StartProfiling(s);
#define STOP_PROFILE     Tools::Hydrofiler::StopProfiling();