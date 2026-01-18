#pragma once
#include <string>
#include "Object.h"

class Event {
public:
	const MapObject* actor = nullptr;
	Event(const MapObject* _actor):  actor(_actor) {};
};

class GameEndedEvent : public Event {};