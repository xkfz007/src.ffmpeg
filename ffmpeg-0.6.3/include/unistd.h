#ifndef _UNISTD_H_
#define _UNISTD_H_
#ifdef WIN32
#include <windows.h> 
static void usleep(int x)
{
	Sleep(x/1000);
}
#endif
#endif
