#include"Server_Box.h"

int Server_Box::Cmd_Model()
{
	char path[1024];
	while (isopen)
	{
		std::cin >> buffer_cmd;

		if (strcmp(buffer_cmd, "quit") == 0)
		{
			isopen = false;
			std::cout << "�������ر�..." << std::endl;
			exit(-1);
		}
		if (strcmp(buffer_cmd, "select") == 0)
		{
			for (auto temp : BOX_GROUP)
			{
				std::cout << temp << std::endl;
			}
		}
		if (strcmp(buffer_cmd, "send") == 0)
		{
			std::cin >> path;
			auto A = BOX_GROUP.begin();
			FileSend(path, *A);
		}
	}
	return 0;
}

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

		break;

	}

	Basic_Server::POST_RECV(io_data);

	return true;
}
//�յ���̨�ļ����������
int Server_Box::FileSend(char* path, LPIO_DATA io_data)
{

	//���ջ���ʶ��ͷ�ļ�
	LPHead_code temp = (LPHead_code)io_data->Buffer;

	//�����ļ���
	std::ifstream file;
	file.open(path, std::ios::in | std::ios::binary | std::ios::ate);
	//�޴��򷵻�false
	if (!file.is_open()) return false;

	//����Э��ͷ
	temp->target = READ;
	temp->size = file.tellg();

	//�����ļ���ȡ�ڴ�
	char* buffer = new char[temp->size];

	//���͸������������ý���׼��
	if (SOCKET_ERROR == send(io_data->socket, (char*)temp, sizeof(Head_code), 0))
	{
		return false;
	}

	//������λ���ļ�ͷ,׼����ȡ
	file.seekg(std::ios::_Seekbeg);

	//���뵽������
	file.read(buffer, temp->size);

	send(io_data->socket, buffer, temp->size, 0);

	std::cout << "�ļ����ͳɹ�" << std::endl;

	return 0;
}