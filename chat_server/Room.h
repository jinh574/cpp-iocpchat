#pragma once

class Room {
private:
	std::vector<SOCKETINFO *> user_list;
	DWORD roomNo;
	DWORD dbNo;
	DWORD userCount;
	CString roomName;
public:
	Room();
	Room(DWORD roomNo, CString roomName);
	~Room();
	BOOL enterRoom(SOCKETINFO *ptr);
	DWORD leaveRoom(SOCKETINFO *ptr);
	DWORD getUserCount();
	DWORD getRoomNo();
	BOOL getRoomNo(DWORD n);
	std::vector<SOCKETINFO *>* getUserList();
	CString getRoomName();
	BOOL setDbNo(DWORD n);
	DWORD getDbNo();
	CString getName();
};