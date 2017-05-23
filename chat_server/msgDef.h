#pragma once

//메세지 정의
enum MSG_TYPE {
	MSG_CONN,					//	0			시스템 접속
	MSG_DISSCONN,				//	1			시스템 접속 해제
	MSG_SYS,					//	2			시스템 메세지
	MSG_USER_LIST,				//	3			유저 리스트
	MSG_JOIN,					//	4			회원 가입 요청
	MSG_JOIN_ACK,				//	5			회원 가입 성공 여부
	MSG_LOGIN,					//	6			로그인 요청
	MSG_LOGIN_ACK,				//	7			로그인 성공 여부
	MSG_LOGIN_AUTH,				//	8			로그인 아이피 불일치로 인증코드 요청
	MSG_LOGIN_AUTH_CODE,		//	9			로그인 인증코드 전송
	MSG_LOGIN_AUTH_ACK,			//	10			로그인 인증코드 성공 여부
	MSG_MESSAGE_LOBBY,			//	11			로비에서 메세지 전송
	MSG_BROADCAST_LOBBY,		//	12			로비 전체 전송
	MSG_CREATEROOM,				//	13			방만들기 요청
	MSG_CREATEROOM_ACK,			//	14			방만들기 요청 성공 여부
	MSG_ENTERROOM,				//	15			방들어가기 요청
	MSG_ENTERROOM_ACK,			//	16			방들어가기 요청 성공 여부
	MSG_CHATROOM_COMP,			//	17			방 다이얼로그 생성 완료
	MSG_CHATROOM_LIST,			//	18			채팅방 리스트 갱신
	MSG_CHATROOM_DISCONN,		//	19			채티방 닫기
	MSG_CHATROOM_MESSAGE,		//	20			채팅방 메세지
	MSG_CHATROOM_BROADCAST,		//	21			채팅방 전체 전송
	MSG_WHISPER,				//	22			귓속말 요청
	MSG_WHISPER_ACK,			//	23			귓속말 전송
	MSG_LOGIN_LOG,				//	24			로그인 이력 요청
	MSG_LOGIN_LOG_ACK,			//	25			로그인 이력 전송
	MSG_CHAT_LOG,				//	26			채팅 이력 요청
	MSG_CHAT_LOG_ACK,			//	27			채팅 이력 전송
	MSG_CHAT_LOG_ROOM,			//	28			채팅방 내용 요청
	MSG_CHAT_LOG_ROOM_ACK,		//	29			채팅방 내용 전송
	MSG_CHAT_LOG_ROOM_MORE,		//	30			채팅방 내용 더보기 요청
	MSG_CHAT_LOG_ROOM_MORE_ACK,	//	30			채팅방 내용 더보기 전송
	MSG_CHAT_LOG_MORE,			//	31			채팅 이력 더보기 요청
	MSG_CHAT_LOG_MORE_ACK,		//	32			채팅 이력 더보기 전송
	MSG_ALL_DISCONN				//	33			서버 종료
};

//서버 정보
#define SERVERPORT			9000
#define BUFSIZE				4096

enum IO_TYPE {
	IO_ACCEPT,
	IO_RECV,
	IO_SEND
};
