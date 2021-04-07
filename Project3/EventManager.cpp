#include "EventManager.h"

EventManager* EventManager::instance;

void EventManager::reg(std::string eventType, std::vector<EventHandler *>& handler) {
	manager[eventType] = handler;
}

void EventManager::unreg(std::string eventType) {
	manager.erase(eventType);
}

void EventManager::raise(Event event) {
	if (!isReplaying) { // if it's not replaying, push the event into the queue.
		queue.push(event);
	}
	else if (event.timestamp <= recordingEndTime) { // or if the event is changed timescale (important), then push it.
		queue.push(event); // it will be sent to the beginning of the queue.
	}
	
}

void EventManager::handleEvents() {
	// All events should now be in the queue. 
	// To handle all of the events, we delegate that to the event handlers.
	// We should just be able to get all events as they were created
	// and clear the queue.
	while (!queue.empty() && !isReplaying) {
		Event e = queue.top();
		queue.pop();
		int guid = e.associatedGUID;
		if (guid == -1) { // if the event isn't associate with a game object, apply it to all watchers of a certain event.
			std::vector<EventHandler*> temp = manager[e.eventType];
			for (int i = 0; i < temp.size(); i++) {
				temp.at(i)->onEvent(e);
			}
		}
		else {
			EventHandler* associatedHandler = handlers[guid];
			associatedHandler->onEvent(e);
		}
		if (isRecording) { // if recording, send the event to the replay manager to be written
			replayHandler->onEvent(e);
		}
	}
}

void EventManager::handleEvents(double deltaT) {
	
	
	double maxEventTime = recordingStartTime + deltaT;
	while (!queue.empty() && recordingStartTime <= recordingEndTime) {
		Event e = queue.top();
		if (e.timestamp <= maxEventTime) {
			int guid = e.associatedGUID;
			if (guid == -1) { // if the event isn't associate with a game object, apply it to all watchers of a certain event.
				std::vector<EventHandler*> temp = manager[e.eventType];
				for (int i = 0; i < temp.size(); i++) {
					temp.at(i)->onEvent(std::ref(e));
				}
			}
			else {
				EventHandler* associatedHandler = handlers[guid];
				associatedHandler->onEvent(std::ref(e));
			}
			queue.pop(); // remove the event if we were actually able to handle it.
		}
		else {
			//std::cout << "event type " << e.eventType << " is to be handled next" << std::endl;
			recordingStartTime += deltaT;
			return; // the event doesn't need to be handled yet, break out.
		}
		
	}
	if (queue.empty()) {
		isReplaying = false;
		return;
	}
	

	/**
	while (!queue.empty()) {

		Event e = queue.top();
		recordingStartTime = e.timestamp;

		if (e.)
		int guid = e.associatedGUID;
		if (guid == -1) { // if the event isn't associate with a game object, apply it to all watchers of a certain event.
			std::vector<EventHandler*> temp = manager[e.eventType];
			for (int i = 0; i < temp.size(); i++) {
				temp.at(i)->onEvent(std::ref(e));
			}
		}
		else {
			EventHandler* associatedHandler = handlers[guid];
			associatedHandler->onEvent(std::ref(e));
		}
		queue.pop();
	}
	recordingStartTime += deltaT;
	// once the queue is empty, we are done, and we can enter normal operation again!
	if (queue.empty()) {
		isReplaying = false;
	}
	*/
	
	
}

EventManager* EventManager::getInstance() {
	if (!instance) {
		instance = new EventManager();
	}
	return instance;
}

void EventManager::addToHandlers(int guid, EventHandler* handler) {
	handlers[guid] = handler;
}

EventManager::EventManager() {
	manager = {};
	queue = {};
	replayHandler = new ReplayHandler();
	container.push_back(replayHandler);

	container.push_back(replayHandler);
	isRecording = false;
	isReplaying = false;
	recordingStartTime = 0;
	recordingEndTime = 0;
}