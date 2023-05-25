#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define MAX_USER (1024)

#define TCP_MTU (1460)

#define UDP_MTU (1472)

#define Check_ret(n,error_code)\
if((n)==error_code){\
	perror(#n);\
	printf("错误代码为: %d",GetLastError());\
	return -1;\
}
