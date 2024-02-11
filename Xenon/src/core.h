#ifndef _XENON_CORE_H_
#define _XENON_CORE_H_


#ifdef _WIN32
	#ifdef XENON_DLL_BUILD
		#define XAPI __declspec(dllexport) 
	#else
		#define XAPI __declspec(dllimport) 
	#endif
#else
	#error We only support windows rn
#endif



#endif // !_XENON_CORE_H_