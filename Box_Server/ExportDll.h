#pragma once
#include"Server_box.h"


extern"C" __declspec(dllexport) void __stdcall server_start();
extern"C" __declspec(dllexport) void __stdcall sendFile(char* path, void* id);