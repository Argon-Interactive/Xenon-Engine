#ifdef XENON
	#define XAPI __declspec(dllexport) 
#else
	#define XAPI __declspec(dllimport) 
#endif

namespace Xenon
{
	XAPI void testMe();
	XAPI void runApp(const char* name);
}
