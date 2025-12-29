#pragma once

class GameRoom;

class BaseProp {
protected:
	const GameRoom& room;

public:
	BaseProp(const GameRoom& _room) : room(_room) {}
};