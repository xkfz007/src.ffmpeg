#ifndef _UNISTD_H_
#define _UNISTD_H_
#include <windows.h> // 影响 near 变量， near或许做关键字解析, 移到config_w32_add.h中

static void usleep(int x)
{
	Sleep(x/1000);
}
#endif