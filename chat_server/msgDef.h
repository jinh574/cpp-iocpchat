#pragma once

//�޼��� ����
enum MSG_TYPE {
	MSG_CONN,					//	0			�ý��� ����
	MSG_DISSCONN,				//	1			�ý��� ���� ����
	MSG_SYS,					//	2			�ý��� �޼���
	MSG_USER_LIST,				//	3			���� ����Ʈ
	MSG_JOIN,					//	4			ȸ�� ���� ��û
	MSG_JOIN_ACK,				//	5			ȸ�� ���� ���� ����
	MSG_LOGIN,					//	6			�α��� ��û
	MSG_LOGIN_ACK,				//	7			�α��� ���� ����
	MSG_LOGIN_AUTH,				//	8			�α��� ������ ����ġ�� �����ڵ� ��û
	MSG_LOGIN_AUTH_CODE,		//	9			�α��� �����ڵ� ����
	MSG_LOGIN_AUTH_ACK,			//	10			�α��� �����ڵ� ���� ����
	MSG_MESSAGE_LOBBY,			//	11			�κ񿡼� �޼��� ����
	MSG_BROADCAST_LOBBY,		//	12			�κ� ��ü ����
	MSG_CREATEROOM,				//	13			�游��� ��û
	MSG_CREATEROOM_ACK,			//	14			�游��� ��û ���� ����
	MSG_ENTERROOM,				//	15			����� ��û
	MSG_ENTERROOM_ACK,			//	16			����� ��û ���� ����
	MSG_CHATROOM_COMP,			//	17			�� ���̾�α� ���� �Ϸ�
	MSG_CHATROOM_LIST,			//	18			ä�ù� ����Ʈ ����
	MSG_CHATROOM_DISCONN,		//	19			äƼ�� �ݱ�
	MSG_CHATROOM_MESSAGE,		//	20			ä�ù� �޼���
	MSG_CHATROOM_BROADCAST,		//	21			ä�ù� ��ü ����
	MSG_WHISPER,				//	22			�ӼӸ� ��û
	MSG_WHISPER_ACK,			//	23			�ӼӸ� ����
	MSG_LOGIN_LOG,				//	24			�α��� �̷� ��û
	MSG_LOGIN_LOG_ACK,			//	25			�α��� �̷� ����
	MSG_CHAT_LOG,				//	26			ä�� �̷� ��û
	MSG_CHAT_LOG_ACK,			//	27			ä�� �̷� ����
	MSG_CHAT_LOG_ROOM,			//	28			ä�ù� ���� ��û
	MSG_CHAT_LOG_ROOM_ACK,		//	29			ä�ù� ���� ����
	MSG_CHAT_LOG_ROOM_MORE,		//	30			ä�ù� ���� ������ ��û
	MSG_CHAT_LOG_ROOM_MORE_ACK,	//	30			ä�ù� ���� ������ ����
	MSG_CHAT_LOG_MORE,			//	31			ä�� �̷� ������ ��û
	MSG_CHAT_LOG_MORE_ACK,		//	32			ä�� �̷� ������ ����
	MSG_ALL_DISCONN				//	33			���� ����
};

//���� ����
#define SERVERPORT			9000
#define BUFSIZE				4096

enum IO_TYPE {
	IO_ACCEPT,
	IO_RECV,
	IO_SEND
};
