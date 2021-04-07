#pragma once
#include "Event.h"
#include "GameObject.h"
#include "Timeline.h"
#include <zmq.hpp>
#include <iostream>
#include <fstream>

class GameObject;
class Player;
class Platform;
class Spawn;
class Boundary;
class Timeline;

#define COLLISION_PLAT 0
#define COLLISION_VIEW 1
#define COLLISION_DEAD 2

class EventHandler {
public:
	/**
	* Each event handler must be able to handle an event.
	* 
	* @param event the event to handle.
	*/
	virtual void onEvent(Event& event) = 0;
};

/**
* Handler for platforms.
*/
class PlatformHandler : public EventHandler {
public:
	Platform* obj;
	PlatformHandler(Platform* obj);

	void onEvent(Event& e);
};

class SpawnHandler : public EventHandler {
public:
	Spawn* obj;
	SpawnHandler(Spawn* obj);

	void onEvent(Event& e);
};

class BoundaryHandler : public EventHandler {
public:
	Boundary* obj;
	BoundaryHandler(Boundary* obj);

	void onEvent(Event& e);
};

class PlayerHandler : public EventHandler {
public:
	Player* obj;
	PlayerHandler(Player* obj);

	void onEvent(Event& e);
};

class ConnectionHandler : public EventHandler {
public:
	zmq::socket_t* requestSocket;
	std::string message;

	ConnectionHandler(zmq::socket_t* reqSocket, std::string message);

	void onEvent(Event& e);
};

class AcceptConnectHandler : public EventHandler {
public:
	int* clientCount;
	int* clientID;
	std::vector<int>* clientIDVector;
	std::vector<float>* playerInfoVector;
	float currentPlayerX;
	float currentPlayerY;
	float playerR;
	float playerG;
	float playerB;

	AcceptConnectHandler(int* clientCount, int* clientID, std::vector<int>* clientIDVector, std::vector<float>* playerInfoVector,
		float currentPlayerX, float currentPlayerY, float playerR, float playerG, float playerB);

	void onEvent(Event& e);
};

class GameTimeHandler : public EventHandler {
public:
	Timeline* timeline;
	Player* obj;

	GameTimeHandler(Timeline* timeline, Player* obj);
	
	void onEvent(Event& e);
};

class ReplayHandler : public EventHandler {
public:
	std::vector<float> initPositions;
	std::vector<int> objIndices;

	// probably an IO writing thing
	std::ofstream recordingOutput; // output
	std::ifstream replayInput;

	double replayStartTime;
	double replayEndTime;
	int eventCount;

	ReplayHandler();

	void onEvent(Event& e);
};