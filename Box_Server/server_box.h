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

};
//�Զ���Э��
typedef struct Head_code
{
	//�̶�����(���ɸı�)
	const uint16_t DIY = 0xffee;

	//������״̬(Ĭ��)
	status target = WAIT;

	//���(�������Ϊ-1)
	uint32_t group_num;

	//����С
	uint16_t size;


}Head_code, * LPHead_code;



class server_box :public Basic_Server
{
	int POST_RECV(LPIO_DATA io_data);
};
