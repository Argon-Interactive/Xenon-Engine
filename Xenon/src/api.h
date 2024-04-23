#ifndef _XENON_CORE_H_
#define _XENON_CORE_H_


#if defined _WIN32
	#ifdef XENON_DLL_BUILD
		#define XAPI __declspec(dllexport) 
	#else
		#define XAPI __declspec(dllimport) 
	#endif
#elif defined __linux__
	#ifdef XENON_DLL_BUILD
		#define XAPI __attribute__((visibility("default"))) 
	#else
		#define XAPI
	#endif
#else
	#error Currently we only support Windows and Linux systems
#endif


#endif // !_XENON_CORE_H_
