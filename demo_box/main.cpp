#include"Client_Box.h"

int main(int argc, char* argv[])
{
	Client_Box kk;

	//链接服务器
	int sockserver = kk.linkserver();

	////初始化循环播放序列
	kk.InitPlaySound();
	//开始接收服务器请求
	kk.RecvData(&kk);

	//kk.RecvData();

	getchar();
	return 0;
}