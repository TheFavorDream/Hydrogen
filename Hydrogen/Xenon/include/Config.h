/*

	Use this file to change the behaviors of Xenon,  you can overwrite:
	Xenon Data structures with your own implementation
	New/Delete operators with your own implementation
	Asserations and logging behavior
	Vectors & Matrices

*/


#pragma once

/*
	use these macros to interface Xenon's logging with your engines logging
	Define XE_LOG_ASSERT_IMPL  "Your implementation of assertion"
	Define XE_LOG_ERROR_IMPL   "Your implementation of error logging"
	Define XE_LOG_WARNS_IMPL   "Your implementation of warnings logging"
	Define XE_LOG_INFO_IMPL    "Your implementation of Info logging"

*/

//#define XE_LOG_ASSERT_IMPL(x, a)
//#define XE_LOG_ERROR_IMPL(x) std::cout << "Error:" << x << '\n';
//#define XE_LOG_WARNS_IMPL(x)
//#define XE_LOG_INFO_IMPL(x) std::cout << "Info:" << x << '\n';

//#define XE_SHUTUP

/*
	Use these macros to overwrite Xenon's Default internal vector with your own vectors.
	you can overwrite each vector type seperatly
*/

//#define XE_VEC2_IMPL
//#define XE_VEC3_IMPL
//#define XE_VEC4_IMPL


/*
	Use these macros to change the behavior of new and malloc.
	this overwrite will be valid in all Xenon and will not have any effect to other units.
*/

//#define XE_NEW_IMPL(size)
//#define XE_DELETE_IMPL(pointer)


/*

	Use these macros to replace the data structures used by Xenon, with your own implemention of standard structures like:
	#define XE_STRUCTURE_VECTOR --> std::vector,
	#define XE_STRUCTURE_STRING --> std::string,
	#define XE_STRUCTURE_UNMAP  --> std::unordered_map

	note that your implementions must be compatible with standard structures. for instance:
	the method std::vector::push_back() must be defined in your implemention with the exact name and parameters
*/

//#define XE_STRUCTURE_VECTOR std::vector
//#define XE_STRUCTURE_STRING std::string
//#define XE_STRUCTURE_UNMAP  std::unordered_map


/*

	Use these Macros to replace Xenon's Default Data Streaming with your own implementation.
	By Default Xenon uses C++ Standard library (fstream) to read/write data to/from files.
	
	your implementation of XE_STREAM_READ/XE_STREAM_WRITE should be a static function
	and accept a XE_STRING type as path.

	
	The Stream Read must return Data inside the file using XE_STRING.
	The Stream Write must accept a XE_STRING as Data that needs to written to the file specified by path
	if the Write was successful, STREAM_WRITE must return zero, otherwise a value greater than zero

*/

//#define XE_STREAM_READ_IMPL(Path)
//#define XE_STREAM_WRITE_IMPL(Path)

