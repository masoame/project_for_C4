#include"Client_Box.h"

int main(int argc, char* argv[])
{
	Client_Box kk;

	//链接服务器
	int sockserver = kk.linkserver();

	kk.arr[0] = true;
	kk.arr[1] = true;
	////初始化循环播放序列
	kk.InitPlaySound();

	//
	//kk.RecvData();

	getchar();
	return 0;
}