#ifdef WINDLL

#include<iostream>
#include<windows.h>
using namespace std;
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		cout << "dll导入..." << endl;
		break;

	case DLL_THREAD_ATTACH:
		cout << "创建线程..." << endl;
		break;

	case DLL_THREAD_DETACH:
		cout << "销毁线程..." << endl;
		break;

	case DLL_PROCESS_DETACH:
		cout << "dll卸载..." << endl;
		if (lpvReserved != nullptr)
		{
			break;
		}

		break;
	}
	return TRUE;
}

#endif 