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
	//Ψһʶ��id
	case WAIT:

		//temp->id = io_data->socket;
		send(io_data->socket, (const char*)temp, sizeof(Head_code), 0);
		break;
	//�ظ��Ѿ��յ���Ӧ�ļ�
	case READ | ACK:

		break;
	}

	Basic_Server::POST_RECV(io_data);

	return true;
}
//�յ���̨�ļ����������
int Server_Box::FileSend(char* path, LPIO_DATA id)
{
	//ÿһ������֡�Ĵ�С
	const int framesize = TCP_MTU - sizeof(Head_code);

	//���ջ���ʶ��ͷ�ļ�
	LPHead_code temp = (LPHead_code)id->Buffer;

	//�����ļ���
	std::ifstream file;
	file.open(path, std::ios::in | std::ios::binary | std::ios::ate);
	//�޴��򷵻�false
	if (!file.is_open()) return false;

	//����Э��ͷ
	temp->target = READ;
	temp->group_num = 0;
	temp->size = file.tellg();
	//�����ļ���ȡ�ڴ�
	char* buffer = new char[temp->size];

	//���͸������������ý���׼��
	if (SOCKET_ERROR == send(id->socket, (char*)temp, sizeof(Head_code), 0))
	{
		return false;
	}

	//������λ���ļ�ͷ,׼����ȡ
	file.seekg(std::ios::_Seekbeg);

	//�ƶ���Ҫ���յ��ļ�ͷ��
	char* sendbuffer = (char*)(temp + 1);

	//���뵽������
	file.read(buffer, temp->size);
	char* ptr;

	while (temp->size)
	{
		//����ļ���СС��֡��׼������֡
		if ((temp->size - framesize) <= 0)
		{
			memcpy(sendbuffer, ptr, temp->size);
			send(id->socket, id->Buffer, temp->size + sizeof(Head_code), 0);

		}
		else
		{
			memcpy(sendbuffer, ptr, framesize);
			send(id->socket, id->Buffer, TCP_MTU, 0);
		}
		//�ȴ���Ӧ
		WaitACK();
		//
		ptr += framesize;
	}

	//�����ļ�����
	temp->target = READ | STOP;
	send(id->socket, id->Buffer, sizeof(Head_code), 0);
	


	return 0;
}
