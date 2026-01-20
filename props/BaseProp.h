#pragma once

class GameRoom;

class BaseProp {
protected:
	GameRoom& room;

public:
	BaseProp(GameRoom& _room) : room(_room) {}
};