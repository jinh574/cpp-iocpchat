#pragma once
#include "mysql.h"
class COdbc {
private:
	MYSQL			mysql;
public:
	COdbc();
	~COdbc();
	bool		ConnectMySQL();
	bool		makeUser(char *id, char *pwd, char *authCode);
	int			login(char *id, char *pwd);
	bool		authCheck(int id, char *ip);
	bool		authLogin(int id, char *authCode, char *ip);
	int			makeRoom(char *name);
	bool		log(int id, int room, char *msg);
	bool		login_log(int id, char *ip, int status);
	CString		getLogin_log(int id);
	CString		getChat_list(int id);
	CString		getChatRoom_list(int room);
	CString		getChatRoom_list_more(int room, int count);
	CString		getChat_list_more(int id, int count);
};