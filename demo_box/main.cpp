#include"Client_Box.h"
int main(int argc, char* argv[])
{
	Client_Box client;
	//���ӷ�����
	int sockserver = client.linkserver();

	int ret;
	while (ret = recv(sockserver, client.buffer, TCP_MTU, 0))
	{
		//ʶ������Զ������ݰ�
		if (client.buf->DIY != 0xffee) continue;

		switch (client.buf->target)
		{
			//���յ�������
		case READ:
			//�첽���������

			groove.detach();

			//





		}




	}

	MakeFile("test.wav", "����", 7);
	MakeFile("test.wav", "err����", 10);


	return 0;
}