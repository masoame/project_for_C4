#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define MAX_USER (1024)

#define TCP_MTU (1460)

#define UDP_MTU (1472)

#define SOCKADDR_IN_LEN (sizeof(sockaddr_in))

#define Check_ret(n,error_code)\
if((n)==error_code){\
	perror(#n);\
	std::cout << "file:" << __FILE__<< "\nline:" << __LINE__<< "\nGetLastError: " << GetLastError() << "\nWSAGetLastError: " << WSAGetLastError()<<std::endl;\
	return false;\
}
