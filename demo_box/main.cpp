#include"Client_Box.h"

int main(int argc, char* argv[])
{
	Client_Box kk;

	//���ӷ�����
	int sockserver = kk.linkserver();

	////��ʼ��ѭ����������
	kk.InitPlaySound();
	//��ʼ���շ���������
	kk.RecvData(&kk);

	//kk.RecvData();

	getchar();
	return 0;
}