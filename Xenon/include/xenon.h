#ifdef XENON
	#define XAPI __declspec(dllexport) 
#else
	#define XAPI __declspec(dllimport) 
#endif


XAPI void testMe();