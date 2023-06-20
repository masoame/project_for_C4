#include"Basic_Server.h"
#pragma once
#include"_stdlib_.h"
enum status
{
	//����
	WAIT = 0x0000,
	//ֹͣ
	STOP = 0x0001,
	//�ʶ�
	READ = 0x0002,
	//����
	DISALOGUE = 0x0004,
	//��һ��
	NEXT = 0x0008,
	//Ӧ��ACK
	ACK = 0x0010,
	//��Ϣ
	Message = 0x0020
};
//�Զ���Э��
typedef struct Head_code
{
	//�̶�����(���ɸı�)
	const uint16_t DIY = 0xffee;

	//������״̬(Ĭ��)
	uint16_t target = WAIT;

	//����С
	uint32_t size;
}Head_code, * LPHead_code;

typedef class Server_Box :public Basic_Server
{
public:
	int Cmd_Model();

	int POST_RECV(LPIO_DATA io_data);

	int FileSend(char* path, LPIO_DATA id);
}*LpServer_Box;
