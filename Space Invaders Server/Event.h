#pragma once
#include <string>
#include <map>

/**These are all event types, as strings. */
#define POSITION_CHANGED "POSITION_CHANGED"
#define VELOCITY_CHANGED "VELOCITY_CHANGED"
#define SPAWNED "SPAWNED"
#define DIED "DIED"
#define CHANGED_TIMESCALE "CHANGED_TIMESCALE"
#define PAUSED "PAUSED"
#define UNPAUSED "UNPAUSED"
#define COLLISION "COLLISION"
#define NEW_CLIENT "NEW_CLIENT"
#define TRANSPOSE "TRANSPOSE"
#define JUMPED "JUMPED"
#define BEGIN_REC "BEGIN_REC"
#define SET_INIT_POS "SET_INIT_POS"
#define BEGIN_REPLAY "BEGIN_REPLAY"
#define END_REC "END_REC"
#define FRICTION "FRICTION"
#define GRAVITY "GRAVITY"
#define LATERAL_INPUT "LATERAL_INPUT"
#define ADJUST_OFFSET "ADJUST_OFFSET"

/** These are the types of event arguments. */
#define X_POS "xPos"
#define Y_POS "yPos"
#define X_VEL "xVel"
#define Y_VEL "yVel"
#define ID "id"
#define X_DIST "xDist"
#define Y_DIST "yDist"
#define COLLISION_TYPE "collisionType"
#define PAUSED_STATUS "pausedStatus"
#define OBJ_OFFSET "objOffset"
#define PLAY_OFFSET "playOffset"
#define FROM_LEFT "fromLeft"
#define SCALE "scale"

/** types as macros to ensure continuity.*/
#define T_INT "int"
#define T_FLO "float"
#define T_BOO "bool"
#define T_DOU "double"
/**
* Taken from textbook.
*/
class Variant {
public:
	enum class Type {
		INTEGER,
		FLOAT,
		BOOL,
		DOUBLE
	};
	Type type;
	union {
		int intData;
		float floatData;
		bool boolData;
		double doubleData;
	};
};

class Event {

public:
	std::string eventType;

	/** The timestamp when the event was raised/created. */
	double timestamp;
	/** The virtual time to sort the events, will use later eventually. */
	int virtualTime;

	int associatedGUID;

	std::map<std::string, Variant> args; // args for event.

	Event(std::string eventType, int associatedGUID);

	Event(std::string eventType, int associatedGUID, double timestamp);

	Variant getArg(std::string key);
	void addArg(std::string key, Variant value);

	double getCurrentTimeFromChronoAsDouble();

	std::string to_string();
};

