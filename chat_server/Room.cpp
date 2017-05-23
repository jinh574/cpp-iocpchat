#include "stdafx.h"
#include <vector>
#include "Room.h"
#include "msgDef.h"

Room::Room() {
	userCount = 0;
}

Room::Room(DWORD roomNo, CString roomName) {
	this->roomNo = roomNo;
	this->roomName = roomName;
	userCount = 0;
}

Room::~Room() {
	user_list.clear();
}

BOOL Room::enterRoom(SOCKETINFO *ptr) {
	user_list.push_back(ptr);
	userCount++;
	return true;
}

DWORD Room::leaveRoom(SOCKETINFO *ptr) {
	DWORD iter = -1;

	if(!ptr)
		return -1;

	for(int i=0; i<user_list.size(); i++) {
		if(user_list[i] == ptr) {
			iter = i;
			break;
		}
	}

	if(iter == -1) {
		return -1;
	}
	else {
		user_list.erase(user_list.begin()+iter);
		userCount--;
		return userCount;
	}	
}

DWORD Room::getUserCount() {
	return userCount;
}

DWORD Room::getRoomNo() {
	return roomNo;
}

BOOL Room::getRoomNo(DWORD n) {
	if(roomNo == n) return true;
	return false;
}

std::vector<SOCKETINFO *>* Room::getUserList() {
	return &user_list;
}

CString Room::getRoomName() {
	return roomName;
}

BOOL Room::setDbNo(DWORD n) {
	dbNo = n;

	return true;
}

DWORD Room::getDbNo() {
	return dbNo;
}

CString Room::getName() {
	return roomName;
}