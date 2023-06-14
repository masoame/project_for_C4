#include"Basic_Server.h"
#pragma once
#include"_stdlib_.h"

struct Send
{
	//����Ŀ����׽���
	SOCKET target_sock;
	//����
	uint16_t Area;
	//���
	uint64_t Group;
	//��С(ȡTCP_MTU���)
	uint64_t Size;
};

//�Զ���Э��
struct DIY_protocol
{
	//�̶�����(���ɸı�)
	const uint16_t DIY = 0xffee;

	//PROCTOCOL_CTRL CTRL;
};

enum PROCTOCOL_CTRL
{
	//����
	WAIT = 0x0000,
	//ǿ��ֹͣ
	STOP = 0x0001,
	//�ʶ�
	READ = 0x0002,
	//����
	DISALOGUE = 0x0004,

	//����UDP����ʹ��
	PACKAGE_READ = 0x0008,
	PACKAGE_DISALOGUE = 0x0010
};

class server_box :public Basic_Server
{
	int POST_RECV(LPIO_DATA io_data);
};
