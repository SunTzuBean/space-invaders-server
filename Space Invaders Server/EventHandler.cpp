#include "EventHandler.h"
#include "Player.h"
#include "EventManager.h"
#include <mutex>
/**
* Handler for platforms.
*/

PlatformHandler::PlatformHandler(Platform* obj) {
	this->obj = obj;
}

void PlatformHandler::onEvent(Event& e) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	if (e.eventType == POSITION_CHANGED) {
		float newXPos = e.getArg(X_POS).floatData;
		float newYPos = e.getArg(Y_POS).floatData;
		obj->getShape()->setPosition(sf::Vector2f(newXPos, newYPos));
	}
	else if (e.eventType == VELOCITY_CHANGED) {
		float newXVelocity = e.getArg(X_VEL).floatData;
		float newYVelocity = e.getArg(Y_VEL).floatData;
		obj->setXVelocity(newXVelocity);
		obj->setYVelocity(newYVelocity);
	}
	else if (e.eventType == TRANSPOSE) {
		int oOffset = e.getArg(OBJ_OFFSET).intData;
		obj->getShape()->move(oOffset, 0);
		obj->viewOffset = oOffset;

		EventManager* manager = manager->getInstance();
		if (!manager->isReplaying) {
			Event p(POSITION_CHANGED, obj->guid, e.timestamp);

			Variant x;
			Variant y;

			x.type = Variant::Type::FLOAT;
			x.floatData = obj->getShape()->getPosition().x;
			y.type = Variant::Type::FLOAT;
			y.floatData = obj->getShape()->getPosition().y;

			p.addArg(X_POS, x);
			p.addArg(Y_POS, y);


			manager->raise(p);
		}
	}
	else if (e.eventType == BEGIN_REC) {
		EventManager* manager = manager->getInstance();

		manager->isRecording = true;
		Event p(POSITION_CHANGED, obj->guid, 0);

		Variant x;
		Variant y;

		x.type = Variant::Type::FLOAT;
		x.floatData = obj->getShape()->getPosition().x;
		y.type = Variant::Type::FLOAT;
		y.floatData = obj->getShape()->getPosition().y;

		p.addArg(X_POS, x);
		p.addArg(Y_POS, y);

		manager->raise(p);
		manager->positionsMap[obj->guid] = obj->getShape()->getPosition();
	}
}


SpawnHandler::SpawnHandler(Spawn* obj) {
	this->obj = obj;
}

void SpawnHandler::onEvent(Event& e) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	if (e.eventType == POSITION_CHANGED) {
		float newXPos = e.getArg(X_POS).floatData;
		float newYPos = e.getArg(Y_POS).floatData;
		obj->getShape()->setPosition(sf::Vector2f(newXPos, newYPos));
	}
	else if (e.eventType == TRANSPOSE) {
		int oOffset = e.getArg(OBJ_OFFSET).intData;
		obj->getShape()->move(oOffset, 0);
		obj->viewOffset = oOffset;

		EventManager* manager = manager->getInstance();
		if (!manager->isReplaying) {
			Event p(POSITION_CHANGED, obj->guid, e.timestamp);

			Variant x;
			Variant y;

			x.type = Variant::Type::FLOAT;
			x.floatData = obj->getShape()->getPosition().x;
			y.type = Variant::Type::FLOAT;
			y.floatData = obj->getShape()->getPosition().y;

			p.addArg(X_POS, x);
			p.addArg(Y_POS, y);


			manager->raise(p);
		}
	}
	else if (e.eventType == BEGIN_REC) {
		EventManager* manager = manager->getInstance();

		manager->isRecording = true;
		Event p(POSITION_CHANGED, obj->guid, 0);

		Variant x;
		Variant y;

		x.type = Variant::Type::FLOAT;
		x.floatData = obj->getShape()->getPosition().x;
		y.type = Variant::Type::FLOAT;
		y.floatData = obj->getShape()->getPosition().y;

		p.addArg(X_POS, x);
		p.addArg(Y_POS, y);

		manager->raise(p);
		manager->positionsMap[obj->guid] = obj->getShape()->getPosition();
	}
}

BoundaryHandler::BoundaryHandler(Boundary* obj) {
	this->obj = obj;
}

void BoundaryHandler::onEvent(Event& e) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	if (e.eventType == COLLISION) {
		// What happens with a boundary collision?
		// Lots of stuff
	}
	else if (e.eventType == POSITION_CHANGED) {
		// update the position
		float newXPos = e.getArg(X_POS).floatData;
		float newYPos = e.getArg(Y_POS).floatData;
		obj->getShape()->setPosition(sf::Vector2f(newXPos, newYPos));
	}
	else if (e.eventType == TRANSPOSE) {
		int oOffset = e.getArg(OBJ_OFFSET).intData;
		obj->getShape()->move(oOffset, 0);
		obj->viewOffset = oOffset;

		EventManager* manager = manager->getInstance();
		if (!manager->isReplaying) {
			Event p(POSITION_CHANGED, obj->guid, e.timestamp);

			Variant x;
			Variant y;

			x.type = Variant::Type::FLOAT;
			x.floatData = obj->getShape()->getPosition().x;
			y.type = Variant::Type::FLOAT;
			y.floatData = obj->getShape()->getPosition().y;

			p.addArg(X_POS, x);
			p.addArg(Y_POS, y);


			manager->raise(p);
		}
		
	}
	else if (e.eventType == BEGIN_REC) {
		EventManager* manager = manager->getInstance();

		manager->isRecording = true;
		Event p(POSITION_CHANGED, obj->guid, 0);

		Variant x;
		Variant y;

		x.type = Variant::Type::FLOAT;
		x.floatData = obj->getShape()->getPosition().x;
		y.type = Variant::Type::FLOAT;
		y.floatData = obj->getShape()->getPosition().y;

		p.addArg(X_POS, x);
		p.addArg(Y_POS, y);

		manager->raise(p);
		manager->positionsMap[obj->guid] = obj->getShape()->getPosition();
	}
}

PlayerHandler::PlayerHandler(Player* obj) {
	this->obj = obj;
}

void PlayerHandler::onEvent(Event& e) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	if (!obj->paused) { // if the player is paused, don't do anything
		if (e.eventType == VELOCITY_CHANGED) {
			double newXVelocity = e.getArg(X_VEL).doubleData;
			double newYVelocity = e.getArg(Y_VEL).doubleData;
			obj->setXVelocity(newXVelocity);
			obj->setYVelocity(newYVelocity);
		}
		else if (e.eventType == POSITION_CHANGED) {
			float newXPos = e.getArg(X_POS).floatData;
			float newYPos = e.getArg(Y_POS).floatData;
			obj->setPosition(sf::Vector2f(newXPos, newYPos));
		}
		else if (e.eventType == DIED) {
			// If the player was to wait in limbo for a while here, we would do that.
			// but since nothing happens in my game (the player immediately gets teleported),
			// we will raise another event to be handled.
			e.eventType = SPAWNED;
			EventManager* manager = manager->getInstance();
			manager->raise(e);
		}
		
		else if (e.eventType == COLLISION) {
			int typeOfCollision = e.getArg(COLLISION_TYPE).intData;
			if (typeOfCollision == COLLISION_PLAT) {
				obj->setYVelocity(0);
				obj->setAirborne(false);
			}
		}
		else if (e.eventType == LATERAL_INPUT) {
			float xDist = e.getArg(X_DIST).floatData;
			obj->move(xDist, 0);

			EventManager* manager = manager->getInstance();
			if (!manager->isReplaying) {
				Event p(POSITION_CHANGED, obj->guid, e.timestamp);

				Variant x;
				Variant y;

				x.type = Variant::Type::FLOAT;
				x.floatData = obj->getPosition().x;
				y.type = Variant::Type::FLOAT;
				y.floatData = obj->getPosition().y;

				p.addArg(X_POS, x);
				p.addArg(Y_POS, y);

				manager->raise(p);
			}
		}
		else if (e.eventType == GRAVITY) {
			float yDist = e.getArg(Y_DIST).floatData;

			obj->move(0, yDist);

			EventManager* manager = manager->getInstance();
			if (!manager->isReplaying) {
				Event p(POSITION_CHANGED, obj->guid, e.timestamp);

				Variant x;
				Variant y;

				x.type = Variant::Type::FLOAT;
				x.floatData = obj->getPosition().x;
				y.type = Variant::Type::FLOAT;
				y.floatData = obj->getPosition().y;

				p.addArg(X_POS, x);
				p.addArg(Y_POS, y);


				manager->raise(p);
			}
		}
		else if (e.eventType == FRICTION) {
			float xDist = e.getArg(X_DIST).floatData;
			float yDist = e.getArg(Y_DIST).floatData;
			obj->move(xDist, yDist);

			EventManager* manager = manager->getInstance();
			if (!manager->isReplaying) {
				Event p(POSITION_CHANGED, obj->guid, e.timestamp);

				Variant x;
				Variant y;

				x.type = Variant::Type::FLOAT;
				x.floatData = obj->getPosition().x;
				y.type = Variant::Type::FLOAT;
				y.floatData = obj->getPosition().y;

				p.addArg(X_POS, x);
				p.addArg(Y_POS, y);


				manager->raise(p);
			}
		}
		else if (e.eventType == JUMPED) {
			obj->setYVelocity(6500);
			// give the player a bit of headroom to perform a jump, smooth so that the object is
			// not colliding in the next frame
			obj->move(0, -4); // this is necessary to get the player to jump/get uncollided with something.
			obj->setAirborne(true);
		}
		else if (e.eventType == TRANSPOSE) {
			int pOffset = e.getArg(PLAY_OFFSET).intData;
			obj->move(pOffset, 0);


			EventManager* manager = manager->getInstance();
			if (!manager->isReplaying) {
				Event p(POSITION_CHANGED, obj->guid, e.timestamp);

				Variant x;
				Variant y;

				x.type = Variant::Type::FLOAT;
				x.floatData = obj->getPosition().x;
				y.type = Variant::Type::FLOAT;
				y.floatData = obj->getPosition().y;

				p.addArg(X_POS, x);
				p.addArg(Y_POS, y);


				manager->raise(p);
			}
		}
		else if (e.eventType == ADJUST_OFFSET) {
			int pOffset = e.getArg(PLAY_OFFSET).intData;
			obj->move(pOffset, 0);

			EventManager* manager = manager->getInstance();

			if (!manager->isReplaying) {
				Event p(POSITION_CHANGED, obj->guid, e.timestamp);

				Variant x;
				Variant y;

				x.type = Variant::Type::FLOAT;
				x.floatData = obj->getPosition().x;
				y.type = Variant::Type::FLOAT;
				y.floatData = obj->getPosition().y;

				p.addArg(X_POS, x);
				p.addArg(Y_POS, y);


				manager->raise(p);
			}

		}
		else if (e.eventType == SPAWNED) {
			float newXPos = e.getArg(X_POS).floatData;
			float newYPos = e.getArg(Y_POS).floatData;
			obj->setPosition(sf::Vector2f(newXPos, newYPos));
			obj->setYVelocity(0);
			obj->setXVelocity(0);
			obj->setAirborne(true);
		}
		else if (e.eventType == BEGIN_REC) {
			EventManager* manager = manager->getInstance();

			manager->isRecording = true;
			manager->recordingStartTime = e.timestamp;
			Event p(POSITION_CHANGED, obj->guid, 0);

			Variant x;
			Variant y;

			x.type = Variant::Type::FLOAT;
			x.floatData = obj->getPosition().x;
			y.type = Variant::Type::FLOAT;
			y.floatData = obj->getPosition().y;

			p.addArg(X_POS, x);
			p.addArg(Y_POS, y);

			manager->raise(p);
			manager->positionsMap[obj->guid] = obj->getPosition();
		}
		else if (e.eventType == END_REC) {
			Event p(BEGIN_REPLAY, obj->guid, e.timestamp);
			EventManager* manager = manager->getInstance(); // defer to replay handler for this one.
			manager->raise(p);
			//manager->handleEvents(); // handle this immediately.
		}
	}
	
}

ConnectionHandler::ConnectionHandler(zmq::socket_t* reqSocket, std::string message) {
	this->requestSocket = reqSocket;
	this->message = message;
}

void ConnectionHandler::onEvent(Event& e) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	if (e.eventType == NEW_CLIENT) {
		Variant v = e.getArg(ID);
		int num = v.intData;

		// The message that we get was already complete and ready to send.
		// A substring from index 1 and onward will duplicate it.
		std::string subMess = message.substr(1);
		std::string sent = std::to_string(num).append(" ");
		sent.append(subMess);

		// now send.
		requestSocket->send(zmq::buffer(sent), zmq::send_flags::none);
	}
}

AcceptConnectHandler::AcceptConnectHandler(int* clientCount, int* clientID, std::vector<int>* clientIDVector, std::vector<float>* playerInfoVector,
										   float currentPlayerX, float currentPlayerY, float playerR, float playerG, float playerB) {
	this->clientCount = clientCount;
	this->clientID = clientID;
	this->clientIDVector = clientIDVector;
	this->playerInfoVector = playerInfoVector;
	this->currentPlayerX = currentPlayerX;
	this->currentPlayerY = currentPlayerY;
	this->playerR = playerR;
	this->playerG = playerG;
	this->playerB = playerB;
}

void AcceptConnectHandler::onEvent(Event& e) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	if (e.eventType == NEW_CLIENT) {
		(*clientCount)++;
		// And add to the list of connected clients.
		clientIDVector->push_back(*clientCount);
		// Since we know this is a new client, it should now be at the end of the list.
		// Its new ID is the same as clientCount.
		*clientID = *clientCount;
		// Now we put new coordinates in the vector that holds them.
		playerInfoVector->push_back(currentPlayerX);
		playerInfoVector->push_back(currentPlayerY);
		// We also need to put in the colors of this client into this vector.
		playerInfoVector->push_back(playerR);
		playerInfoVector->push_back(playerG);
		playerInfoVector->push_back(playerB);
	}
}

GameTimeHandler::GameTimeHandler(Timeline* timeline, Player* obj) {
	this->timeline = timeline;
	this->obj = obj;
}

void GameTimeHandler::onEvent(Event& e) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	if (e.eventType == PAUSED) {
		obj->paused = true;
		timeline->pause();
	}
	else if (e.eventType == UNPAUSED) {
		obj->paused = false;
		timeline->unpause();
	}
	else if (e.eventType == CHANGED_TIMESCALE) {
		double newScale = e.getArg(SCALE).doubleData;
		timeline->changeTick(newScale);
		std::cout << "new timescale = " << newScale << std::endl;
	}
}

ReplayHandler::ReplayHandler() {
	this->initPositions = {};
	this->objIndices = {};
	// writing and reading tool init here
	recordingOutput.open("recording.txt");
	replayStartTime = 0;
	replayEndTime = 0;
	eventCount = 0;
	
}

void ReplayHandler::onEvent(Event& e) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	if (e.eventType == BEGIN_REPLAY) {
		recordingOutput.close();
		replayInput.open("recording.txt");
		EventManager* manager = manager->getInstance();
		manager->isRecording = false; // we are no longer recording any events. They will be queued and played back.
		manager->recordingEndTime = e.timestamp;

		// if we got this, it's time to raise a lot of events.
		//std::cout << "the file is open: " << recordingOutput.is_open() << std::endl;
		// now load in the initial positions of everything.
		/**
		std::map<int, sf::Vector2f>::iterator i;
		for (i = manager->positionsMap.begin(); i != manager->positionsMap.end(); i++) {
			Event initialPosEvent(POSITION_CHANGED, i->first, 0); // must be handled first.
			Variant x;
			Variant y;

			x.type = Variant::Type::FLOAT;
			x.floatData = i->second.x;
			y.type = Variant::Type::FLOAT;
			y.floatData = i->second.y;

			initialPosEvent.addArg(X_POS, x);
			initialPosEvent.addArg(Y_POS, y);
			manager->raise(initialPosEvent);
		}
		*/
		
		// I'm not sure how to ensure they'll be handled yet.
		// operator >> gets a new line from the input stream.
		//recording.clear();
		replayInput.seekg(0, std::ios::beg); // this should return the cursor to the beginning of the file, ready for reading.
		std::string currEvent;
		while (std::getline(replayInput, currEvent)) {
			// pick apart the string similarly to how I get stuff from main
			// TODO finish this!

			std::istringstream bootlegScanner(currEvent);
			std::string currentString;
			int currentInt;
			float currentFloat;
			bool currentBool;
			double currentDouble;

			// first thing is the event's type.
			bootlegScanner >> currentString;
			std::string eventType = currentString;

			// next is the associated guid.
			bootlegScanner >> currentInt;
			int assocGUID = currentInt;

			// next is the timestamp.
			bootlegScanner >> currentDouble;
			double timestamp = currentDouble;

			// now we can construct an event.
			Event replayEvent(eventType, assocGUID, timestamp);

			// next is the number of arguments the event had.
			bootlegScanner >> currentInt;
			int numOfArgs = currentInt;
			
			// now time to collect the arguments.
			for (int i = 0; i < numOfArgs; i++) {
				Variant v;

				// the first thing to collect is a string that says what the name of the argument is.
				bootlegScanner >> currentString;
				std::string argName = currentString;

				// next is the type of the next piece of data.
				bootlegScanner >> currentString;
				if (currentString == T_INT) {
					v.type = Variant::Type::INTEGER;

					bootlegScanner >> currentInt;
					v.intData = currentInt;
					replayEvent.addArg(argName, v);
				}
				else if (currentString == T_FLO) {
					v.type = Variant::Type::FLOAT;

					bootlegScanner >> currentFloat;
					v.floatData = currentFloat;
					replayEvent.addArg(argName, v);
				}
				else if (currentString == T_BOO) {
					v.type = Variant::Type::BOOL;

					bootlegScanner >> currentBool;
					v.boolData = currentBool;
					replayEvent.addArg(argName, v);
				}
				else if (currentString == T_DOU) {
					v.type = Variant::Type::DOUBLE;
					
					bootlegScanner >> currentDouble;
					v.doubleData = currentDouble;
					replayEvent.addArg(argName, v);
				}
				
			}
			// now, the event is fully created and ready to be raised.
			manager = manager->getInstance();
			manager->raise(replayEvent);
		}
		manager->isReplaying = true; // the replay is officially playing now!
		replayInput.close();
		recordingOutput.open("recording.txt"); // open for recording for next time
	}
	else if (e.eventType != BEGIN_REC && e.eventType != END_REC) {
			std::string stringRep = e.to_string();
			recordingOutput << stringRep << std::endl;
	}
		
}