#include <SFML/Graphics.hpp>
#include "StaticPlatform.h"
#include "MovingPlatform.h"
#include "Player.h"
#include "Timeline.h"
#include "GameObject.h"
#include "EventManager.h"

#include <thread>
#include <mutex> 
#include <iostream>
#include <cmath>
#include <vector>
#include <json.hpp>
#include <zmq.hpp>
#include <chrono>

/** The req-rep port for sockets of this type. */
#define REQ_REP_PORT "tcp://*:5555"


#define ADD_POINTS "addPoints"
#define PLAYER_DEAD "playerDead"
#define WINNER "winner"


/** movement condition variable */
std::condition_variable movementCV;
std::condition_variable platformCV;
std::condition_variable gravityCV;
std::condition_variable replyCV;
std::condition_variable pubCV;
std::mutex mutex;

Timeline realTime = Timeline(1);
Timeline gameTime = Timeline(&realTime, 1);

bool running = true; // allows communication to threads.

EventManager* manager = manager->getInstance();

EventHandler* handler;

using json = nlohmann::json;

int main() {
	int score = 0;
	int lives = 3;
	zmq::context_t context{ 1 };

	// set up server
	zmq::socket_t replySocket{ context, zmq::socket_type::rep };
	replySocket.bind(REQ_REP_PORT);
	while (true) {
		std::cout << "Your score: " << score << std::endl;
		zmq::message_t request;
		replySocket.recv(std::ref(request), zmq::recv_flags::none); // the server spends most of its time just sitting here.

		std::string info = std::string(static_cast<char*>(request.data()), request.size());
		std::istringstream bootlegScanner(info);
		std::string command;
		bootlegScanner >> command;
		if (command == ADD_POINTS) {
			score += 20;
			std::cout << "Nice! You destroyed an enemy!" << std::endl;
		}
		else if (command == PLAYER_DEAD) {
			lives--;
			std::cout << "Oh no! You died!" << std::endl;
			std::cout << "Lives remaining: " << lives << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		else if (command == WINNER) {
			std::cout << "You won!" << std::endl;
			std::cout << "Your score: " << score << std::endl;
			break;
		}
		if (lives == 0) {
			std::cout << "Game over!" << std::endl;
			std::cout << "Your score: " << score << std::endl;
			break;
		}
		replySocket.send(zmq::buffer(std::to_string(lives)), zmq::send_flags::none); // sends a reply that has the number of lives.
	}
	replySocket.send(zmq::buffer(std::to_string(lives)), zmq::send_flags::none); // sends a reply that has the number of lives.
	
}