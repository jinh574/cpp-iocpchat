#include "stdafx.h"
#define SOCKET int
#pragma comment(lib, "libmysql.lib")
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include <cstringt.h>
#include <sstream>
#include <string>
#include <json/json.h>
#include "COdbc.h"

#define MYSQL_HOST		"14.63.224.92"
#define MYSQL_USER		"jinh574"
#define MYSQL_PWD		"dhaprk12"
#define MYSQL_DB		"mydb"

COdbc::COdbc() {}

COdbc::~COdbc() {
	mysql_close(&mysql);
}

bool COdbc::ConnectMySQL(void) {
	mysql_init(&mysql);

	if(!mysql_real_connect(&mysql, MYSQL_HOST, MYSQL_USER, MYSQL_PWD, MYSQL_DB, 11002, 0, 0)) {
		//std::cout << mysql_error(&mysql) << std::endl;
		return false;
	}
	else {
		mysql_query(&mysql, "set names euckr");
		//std::cout << "DB 연결 성공" << std::endl;
		return true;
	}
}

bool COdbc::makeUser(char *id, char *pwd, char *authCode) {
	char query[128];
	sprintf(query, "INSERT INTO user(userid, password, authCode) VALUES ('%s', '%s', '%s')", id, pwd, authCode);

	if(mysql_query(&mysql, query)) {
		return false;
	}

	return true;
}

int COdbc::login(char *id, char *pwd) {
	char query[128], tmp[128];
	MYSQL_ROW		row;
	MYSQL_RES		*m_res;
	int result;
	
	sprintf(query, "SELECT * FROM user WHERE userid='%s' AND password='%s' LIMIT 1", id, pwd);
	if(mysql_query(&mysql, query)) {
		return -1;
	}

	if((m_res = mysql_store_result(&mysql)) == NULL) {
		return -1;
	}
	if(mysql_num_rows(m_res)) {
		row = mysql_fetch_row(m_res);
		result = atoi(row[0]);
		//printf("%d",result);
		return result;
	}
	else {
		return 0;
	}
}

bool COdbc::authCheck(int id, char *ip) {
	char query[128];
	MYSQL_ROW		row;
	MYSQL_RES		*m_res;
	
	sprintf(query, "SELECT * FROM user_login WHERE user_ID='%d' AND ip='%s'", id, ip); 
	
	if(mysql_query(&mysql, query)) {
		return false;
	}

	if((m_res = mysql_store_result(&mysql)) == NULL) {
		return false;
	}

	if(mysql_num_rows(m_res)) {
		return true;
	}
	else {
		return false;
	}
}

bool COdbc::authLogin(int id, char *authCode, char *ip) {
	char query[128];
	MYSQL_ROW		row;
	MYSQL_RES		*m_res;
	
	sprintf(query, "SELECT * FROM user WHERE ID_num='%d' AND authCode='%s'", id, authCode); 

	if(mysql_query(&mysql, query)) {
		return false;
	}

	if((m_res = mysql_store_result(&mysql)) == NULL) {
		return false;
	}
	if(mysql_num_rows(m_res)) {
		sprintf(query, "INSERT INTO user_login(user_ID, ip) VALUES ('%d', '%s')", id, ip); 
		if(mysql_query(&mysql, query)) {
			return false;
		}
		return true;
	}
	else {
		return false;
	}
}

int COdbc::makeRoom(char *name) {
	char query[128];
	int room_id = NULL;

	sprintf(query, "INSERT INTO room(name) VALUES ('%s')", name);
	if(mysql_query(&mysql, query)) {
		return false;
	}
	room_id = mysql_insert_id(&mysql);

	return room_id;
}

bool COdbc::log(int id, int room, char *msg) {
	char query[128];

	sprintf(query, "SET FOREIGN_KEY_CHECKS=0");
	if(mysql_query(&mysql, query)) {
		return false;
	}
	sprintf(query, "INSERT INTO mydb.chatLog(room_ID, user_ID, message) VALUES ('%d', '%d', '%s')", room, id, msg);
	
	if(mysql_query(&mysql, query)) {
		return false;
	}
	sprintf(query, "SET FOREIGN_KEY_CHECKS=1");
	if(mysql_query(&mysql, query)) {
		return false;
	}

	return true;
}

bool COdbc::login_log(int id, char *ip, int status) {
	char query[128];

	sprintf(query, "INSERT INTO user_login_log(user_ID, ip, status) VALUES ('%d', '%s', '%d')", id, ip, status);
	if(mysql_query(&mysql, query)) {
		return false;
	}

	return true;
}

CString COdbc::getLogin_log(int id) {
	char query[128];
	MYSQL_ROW		row;
	MYSQL_RES		*m_res;
	Json::Value root, data, capsule;
	Json::StyledWriter writer;
	CString result;
	
	sprintf(query, "SELECT ip, timestamp, status FROM user_login_log WHERE user_ID='%d' order by ID_login_log desc LIMIT 10", id); 

	if(mysql_query(&mysql, query)) {
		return false;
	}


	if((m_res = mysql_store_result(&mysql)) == NULL) {
		return false;
	}

	root["type"] = MSG_LOGIN_LOG_ACK;
	while(row = mysql_fetch_row(m_res)) {
		/*
		root["ip"].append(row[0]);
		root["timestamp"].append(row[1]);
		root["status"].append(row[2]);
		*/
		data["ip"] = row[0];
		data["timestamp"] = row[1];
		data["status"] = row[2];
		root["data"].append(data);
	}
	result.Format("%s",writer.write(root).c_str());

	return result;
}

CString COdbc::getChat_list(int id) {
	char query[512];
	MYSQL_ROW		row;
	MYSQL_RES		*m_res;
	Json::Value root, data, capsule;
	Json::StyledWriter writer;
	CString result;

	sprintf(query, "SELECT distinct room.ID_room, room.timestamp, room.name FROM mydb.chatLog join room on chatLog.room_ID=room.ID_room WHERE user_ID='%d' order by ID_room desc LIMIT 5", id); 

	if(mysql_query(&mysql, query)) {
		return false;
	}
	if((m_res = mysql_store_result(&mysql)) == NULL) {
		return false;
	}

	root["type"] = MSG_CHAT_LOG_ACK;
	while(row = mysql_fetch_row(m_res)) {
		data["room"] = row[0];
		data["timestamp"] = row[1];
		data["name"] = row[2];
		root["data"].append(data);
	}
	result.Format("%s",writer.write(root).c_str());
	return result;
}

CString COdbc::getChatRoom_list(int room) {
	char query[512];
	MYSQL_ROW		row;
	MYSQL_RES		*m_res;
	Json::Value root, data, capsule;
	Json::StyledWriter writer;
	CString result;

	sprintf(query, "SELECT message, timestamp FROM mydb.chatLog WHERE room_ID='%d' order by ID_chatlog desc LIMIT 5", room); 

	if(mysql_query(&mysql, query)) {
		return false;
	}
	if((m_res = mysql_store_result(&mysql)) == NULL) {
		return false;
	}

	root["type"] = MSG_CHAT_LOG_ROOM_ACK;
	while(row = mysql_fetch_row(m_res)) {
		data["message"] = row[0];
		data["timestamp"] = row[1];
		root["data"].append(data);
	}
	result.Format("%s",writer.write(root).c_str());
	return result;
}

CString COdbc::getChatRoom_list_more(int room, int count) {
	char query[512];
	MYSQL_ROW		row;
	MYSQL_RES		*m_res;
	Json::Value root, data, capsule;
	Json::StyledWriter writer;
	CString result;

	sprintf(query, "SELECT message, timestamp FROM mydb.chatLog WHERE room_ID='%d' order by ID_chatlog desc LIMIT %d, 5", room, count); 

	if(mysql_query(&mysql, query)) {
		return false;
	}
	if((m_res = mysql_store_result(&mysql)) == NULL) {
		return false;
	}

	root["type"] = MSG_CHAT_LOG_ROOM_MORE_ACK;
	while(row = mysql_fetch_row(m_res)) {
		data["message"] = row[0];
		data["timestamp"] = row[1];
		root["data"].append(data);
	}
	result.Format("%s",writer.write(root).c_str());

	return result;
}

CString COdbc::getChat_list_more(int id, int count) {
	char query[512];
	MYSQL_ROW		row;
	MYSQL_RES		*m_res;
	Json::Value root, data, capsule;
	Json::StyledWriter writer;
	CString result;

	sprintf(query, "SELECT distinct room.ID_room, room.timestamp, room.name FROM mydb.chatLog join room on chatLog.room_ID=room.ID_room WHERE user_ID='%d' order by ID_room desc LIMIT %d, 5", id, count); 

	if(mysql_query(&mysql, query)) {
		return false;
	}
	if((m_res = mysql_store_result(&mysql)) == NULL) {
		return false;
	}

	root["type"] = MSG_CHAT_LOG_MORE_ACK;
	while(row = mysql_fetch_row(m_res)) {
		data["room"] = row[0];
		data["timestamp"] = row[1];
		data["name"] = row[2];
		root["data"].append(data);
	}
	result.Format("%s",writer.write(root).c_str());
	return result;
}