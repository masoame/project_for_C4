#include"Client_Box.h"

int main(int argc, char* argv[])
{
	Client_Box kk;

	//���ӷ�����
	int sockserver = kk.linkserver();

	kk.arr[0] = true;
	kk.arr[1] = true;
	////��ʼ��ѭ����������
	kk.InitPlaySound();

	//
	//kk.RecvData();

	getchar();
	return 0;
}