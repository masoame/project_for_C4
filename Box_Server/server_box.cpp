#include"Server_Box.h"

int Server_Box::POST_RECV(LPIO_DATA io_data)
{
	LPHead_code temp = (LPHead_code)io_data->Buffer;

	//��֤���ɹ��򷵻�
	if (temp->DIY != 0xffee)
	{
		//ɾ����صĽṹ��
		BOX_GROUP.erase(io_data);
		//�ر��׽���
		closesocket(io_data->socket);
		//ɾ���ṹ��
		delete io_data;

		return false;
	}

	switch (temp->target)
	{
		//�ȴ�
	case WAIT:

		break;
		//������
	case READ:


		break;
		//�ظ��Ѿ��յ���Ӧ�ļ�
	case READ | ACK:

		break;
	}

	Basic_Server::POST_RECV(io_data);

	return true;
}
