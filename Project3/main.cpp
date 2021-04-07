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

/** offset for moving platforms */
#define MAX_OFFSET 100

/** how much to reduce the jump velocity and find the next position. */
#define JUMP_VELOCITY_FACTOR -8800

/** How long the platform will move in one direction in seconds */
#define PLATFORM_HALF_PERIOD 2

/** How long we will protect the game from subsequent player presses to spam P to pause. */
#define PAUSE_SPAM_PERIOD 0.5

/** The frame time for 60 FPS*/
#define THRITY_FPS_FRAME_TIME 0.0666

/** The pub-sub port for sockets of this type. */
#define PUB_SUB_PORT "tcp://*:5551"

/** The req-rep port for sockets of this type. */
#define REQ_REP_PORT "tcp://*:5555"



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

void platformMovementThreadFunction(std::vector<Platform *>& platformVect, double& deltaT) {

    while (running) {
        std::unique_lock<std::mutex> movementLock(mutex);
        platformCV.wait(movementLock);

        for (Platform *p : platformVect) {
            p->move(deltaT);
        }


        /**
        for (int i = 0; i < movingPlatformVector.size(); i++) {
            //std::cout << "Moved platform xVel: " << movingPlatform.getVelocity() << " deltaT: " << deltaT << std::endl;
            movingPlatformVector.at(i).move(sf::Vector2f(movingPlatformVector.at(i).getVelocity() * deltaT, 0));

            if (totalDeltaT <= PLATFORM_HALF_PERIOD && movingPlatformVector.at(i).getIsMovingRight()) {
                totalDeltaT += deltaT;
                //If we get to the left edge, start moving it left.
                if (totalDeltaT > PLATFORM_HALF_PERIOD) {
                    movingPlatformVector.at(i).setIsMovingRight(false);
                    movingPlatformVector.at(i).setVelocity(-100);
                    totalDeltaT = PLATFORM_HALF_PERIOD;
                }
            }
            //
            else if (totalDeltaT >= 0 && !movingPlatformVector.at(i).getIsMovingRight()) {
                totalDeltaT -= deltaT;
                if (totalDeltaT < 0) {
                    movingPlatformVector.at(i).setIsMovingRight(true);
                    movingPlatformVector.at(i).setVelocity(100);
                    totalDeltaT = 0;
                }
            }

            // Check if we move the platform this time


            /**
            if (!pause) {
                if (posOffset < MAX_OFFSET && movingPlatform.getIsMovingRight()) {
                    posOffset++;
                }
                else if (posOffset == MAX_OFFSET) {
                    movingPlatform.setIsMovingRight(false);
                    posOffset--;
                    movingPlatform.setVelocity(-100);
                }
                else if (posOffset > -MAX_OFFSET && !movingPlatform.getIsMovingRight()) {
                    posOffset--;

                }
                else if (posOffset == -MAX_OFFSET) {
                    movingPlatform.setIsMovingRight(true);
                    posOffset++;
                    movingPlatform.setVelocity(100);

                }
                pause = true;
            }
            else {
                pause = false;
            }


            //std::cout << "position of moving platform x: " << movingPlatform.getPosition().x << "y: " << movingPlatform.getPosition().y << std::endl;
        }
        */
        replyCV.notify_one();
        movementLock.unlock();

    }
    
}



/**
* A large amount of this code comes from the tutorials at https://www.sfml-dev.org/
* 
*/
int main() {
    double deltaT = 0; // init deltaT and current time
    double currentTime = 0;

    // create the window
    //sf::RenderWindow window(sf::VideoMode(800, 600), "Hi Paola!");
    
    //***********************************************************************************************
    /** WORLD MODEL STUFF */
    //***********************************************************************************************

    std::vector<Platform *> platformVect; // This is where we keep all of the platforms.
    std::vector<Spawn *> spawnVect; // This is where we keep all of the spawns.
    std::vector<Boundary *> viewBoundVect; // This is where the view bounds are.
    std::vector<Boundary *> killBoundVect; // This is where the kill bounds are.

    // The base platform
    sf::RectangleShape *base = new sf::RectangleShape(sf::Vector2f(600, 100));
    base->setFillColor(sf::Color(40, 209, 88));
    base->setPosition(sf::Vector2f(-100, 500));
    // this texture comes from https://www.pinterest.com/pin/434175220311777176/
    Platform* plat0 = new Platform(base, true, 0, 0, 0, "grass.jpg"); // base platform
    plat0->setTexture("grass.jpg");

    // base platform after discontinuity
    sf::RectangleShape* base1 = new sf::RectangleShape(sf::Vector2f(1000, 100));
    base1->setFillColor(sf::Color(40, 209, 88));
    base1->setPosition(sf::Vector2f(700, 500));
    Platform* plat1 = new Platform(base1, true, 0, 0, 0, "grass.jpg");

    // Another platform
    sf::RectangleShape *move1 = new sf::RectangleShape(sf::Vector2f(100, 20));
    move1->setFillColor(sf::Color(237, 208, 21));
    move1->setPosition(sf::Vector2f(300, 350));
    Platform *plat2 = new Platform(move1, true, 100, 0, 5, ""); // the first moving platform I made (moves left and right)

    sf::RectangleShape *move2 = new sf::RectangleShape(sf::Vector2f(100, 20));
    move2->setFillColor(sf::Color(100, 100, 100));
    move2->setPosition(sf::Vector2f(1300, 150));
    Platform *plat3 = new Platform(move2, true, 0, 100, 3, ""); // the one that moves up and down
    
    sf::RectangleShape *stat1 = new sf::RectangleShape(sf::Vector2f(200, 50));
    stat1->setFillColor(sf::Color(255, 50, 255));
    stat1->setPosition(sf::Vector2f(150, 250));
    Platform *plat4 = new Platform(stat1, true, 0, 0, 0, ""); // big pink static one.

    sf::RectangleShape* stat2 = new sf::RectangleShape(sf::Vector2f(150, 20));
    stat2->setFillColor(sf::Color(108, 64, 158));
    stat2->setPosition(sf::Vector2f(950, 350));
    Platform* plat5 = new Platform(stat2, true, 0, 0, 0, "");

    platformVect.push_back(plat0);
    platformVect.push_back(plat1);
    platformVect.push_back(plat2);
    platformVect.push_back(plat3);
    platformVect.push_back(plat4);
    platformVect.push_back(plat5);

    // Spawn objects
    Spawn* spawn1 = new Spawn(50, 420);

    spawnVect.push_back(spawn1);

    // Boundary objects
    Boundary* viewBound1 = new Boundary(1, 1800, 800, 0, 0); // middle viewpoint.

    viewBoundVect.push_back(viewBound1);

   // Boundary* killBound1 = new Boundary(1, 1500, 0, 0, 0); // leftmost kill bound 
    Boundary* killBound2 = new Boundary(2000, 1, 0, 700, 0); // bottom kill bound
    //Boundary* killBound3 = new Boundary(1, 1500, 1600, 0, 0); // right kill bound

    //killBoundVect.push_back(killBound1);
    killBoundVect.push_back(killBound2);
    //killBoundVect.push_back(killBound3);

    //std::thread movementThread(movementThreadFunction, std::ref(player), std::ref(deltaT));

    std::thread platformMovementThread(platformMovementThreadFunction, std::ref(platformVect), std::ref(deltaT));
    // run the program as long as the window is open
    
    //std::thread collisionAndGravityThread(collisionAndGravityThreadFunction, std::ref(player), std::ref(movingPlatform), std::ref(basePlatform), std::ref(deltaT));

    zmq::context_t context{ 1 };

    // We can actually create a socket to reply here!
    zmq::socket_t replySocket{ context, zmq::socket_type::rep };
    replySocket.bind(REQ_REP_PORT);

    // construct a pub socket and bind to interface
    zmq::socket_t publisherSocket{ context, zmq::socket_type::pub };
    publisherSocket.bind(PUB_SUB_PORT);

    std::vector<std::string> stringPlatVect; // the actual strings we will send the clients.
    std::vector<std::string> stringSpawnVect;
    std::vector<std::string> stringViewBoundsVect;
    std::vector<std::string> stringKillBoundsVect;

    // Initialize the data structures that will be stored by the JSON object.
    std::vector<int> clientIDVector;
    std::vector<float> playerInfoVector;
    std::vector<int> disconnectedVect;

    // Adding strings to json object.

    for (Platform *p : platformVect) {
        stringPlatVect.push_back(p->to_string());
    }
    
    for (Spawn* s : spawnVect) {
        stringSpawnVect.push_back(s->to_string());
    }

    for (Boundary* b : viewBoundVect) {
        stringViewBoundsVect.push_back(b->to_string());
    }

    for (Boundary* b : killBoundVect) {
        stringKillBoundsVect.push_back(b->to_string());
    }


    // Initialize the JSON object we will pass back and forth.
    // The client will avoid drawing clients twice by syncing its ID with the indicies.
    json structure;
    structure["playerInfo"] = playerInfoVector;
    structure["newDisconnected"] = disconnectedVect;
    structure["platformInfo"] = stringPlatVect;
    structure["spawnInfo"] = stringSpawnVect;
    structure["viewBounds"] = stringViewBoundsVect;
    structure["killBounds"] = stringKillBoundsVect;


    // This is the number of clients that have connected.
    // New connections will be indicated to the server by the client sending some number.
    // I think it's best if this number is 0.
    // Then the server will assign it an ID (clientCount) so we also know the order that
    // clients connected.
    // The client will receive this number back and change its ID to this, and for future processing
    // a client sending a number other than 0 means the client has already connected.
    // Also, if a client disconnects, the rest of the clients may need new IDs.
    // This can be accomplished after setting the new client vector structure and then having
    // The server send back the new client's ID.
    int clientCount = 0;

    std::vector<int> clientsThatNeedUpdating = {};

    while (true) {
        // check all the window's events that were triggered since the last iteration of the loop
        // sf::Event event;
        // tell the movement thread we're good to go

        deltaT = gameTime.getTime() - currentTime;
        if (deltaT > THRITY_FPS_FRAME_TIME || deltaT < 0) {
            deltaT = 0;

        }
        platformCV.notify_all(); // Notify the platform we're ready to run.
        currentTime = gameTime.getTime();        
        
        //***********************************************************************************************
        /** SERVER STUFF */
        //***********************************************************************************************
        // there are two stages. First, the server receives a message from one of the clients.
        // This message will be a string of numbers. As I parse them, I will identify them.
        // It will do these in a loop so that the clients have time to catch up

        zmq::message_t request;

        // Now we update the position of the moving platform.
        // The position of the platform is entirely coordinated by the server.
        // Function will do something like this.
        for (int i = 0; i < clientIDVector.size() || clientIDVector.size() == 0; i++) {
            //std::cout << "about to receive reply..." << std::endl;
            replySocket.recv(std::ref(request), zmq::recv_flags::none);
            // Now we have this full message from the client.
            // Let's parse the messsage first.
            std::string newClientOrNot = std::string(static_cast<char*>(request.data()), request.size());
            std::istringstream bootlegScanner(newClientOrNot);
            int currentInt;
            float currentFloat;

            // The 1st number is the ID of the client. It's important that this is an int.
            bootlegScanner >> currentInt;
            int clientID = currentInt;

            // The 2nd number tells the server whether or not to keep this connection open.
            // Acts like a bool, but I think it would be harder to have a bool.
            bootlegScanner >> currentInt;
            int isConnectionOpen = currentInt;

            // The 3rd number is the X coord of the player. 
            bootlegScanner >> currentFloat;
            float currentPlayerX = currentFloat;

            // The 4th number is the Y coord of the player.
            bootlegScanner >> currentFloat;
            float currentPlayerY = currentFloat;

            // The 5th and 6th numbers are the velocities of the player.
            // These are of no use to the server.
            bootlegScanner >> currentFloat;
            bootlegScanner >> currentFloat;

            // The 7th and 8th numbers tell the airborne status of the player.
            // These are also of no use to the server.
            bootlegScanner >> currentInt;
            bootlegScanner >> currentInt;

            // The 9th, 10th, and 11th numbers are the colors of the player.
            // When a client creates a player, it creates it with random colors.
            bootlegScanner >> currentFloat;
            float playerR = currentFloat;
            bootlegScanner >> currentFloat;
            float playerG = currentFloat;
            bootlegScanner >> currentFloat;
            float playerB = currentFloat;

            // The 12th number is the health of the player.
            // This currently has no use to the server.
            bootlegScanner >> currentFloat;

            // The 13th number is the offset of the player.
            // On the screen, the player's location is within its own client's viewpoint: between 800 and 600.
            // If the client is actually where we can't see the objects, we should add 800 to its position.
            // This is fine because the client will actually tell us that information.
            bootlegScanner >> currentInt;
            int offset = currentInt;

            // Okay! So now we have all of the important information about the client.
            // Now let's see if it's a new client:
            // If it is a new client, we want to increase the number of clients connected.
            if (clientID == 0) {
                // Create an event and raise it to the manager.
                Event e(NEW_CLIENT, -1);
                handler = new AcceptConnectHandler(&clientCount, &clientID, &clientIDVector, &playerInfoVector, currentPlayerX, currentPlayerY,
                                                   playerR, playerG, playerB);
                std::vector<EventHandler*>connect{ handler };
                manager->reg(NEW_CLIENT, connect);
                manager->raise(e);
                manager->handleEvents(); // immediately handle the event.
            }

            // We need to check if this client needed to be updated because a client to the left
            // disconnected at some point.
            for (int i = 0; i < clientsThatNeedUpdating.size(); i++) {
                if (clientID == clientsThatNeedUpdating.at(i)) {
                    clientID = clientID - 1;
                    clientsThatNeedUpdating.erase(clientsThatNeedUpdating.begin() + i);
                    break;
                }
            }

            // Now we should check to see if the client list has been updated since the last time
            // we asked for infomration.


            // Otherwise, this client directly corresponds to an index in the information list.
            int index = (clientID - 1) * 5;

            // These are not mutually exclusive, but if there is a disconnect, we need to alter the
            // player info vector. We need to remove the information associated with the client
            // that is going to disconnect.
            // If the client wants to disconnect, the number for isConnectionOpen will be 0.
            if (isConnectionOpen == 0) {
                std::cout << "someone disconnected" << std::endl;
                clientCount--;
                // The client also needs to know which clients disconnected.
                // These are specifically an index.
                disconnectedVect.push_back(clientID - 1);
                // We need to decrease the client IDs now.
                clientIDVector.erase(clientIDVector.begin() + clientID - 1);
                for (int i = clientID - 1; i < clientIDVector.size(); i++) {
                    // This should decrement every value after the one we just erased in the client array.
                    // We add the clients that will say they have the wrong ID to this list.
                    // The client still thinks they are client 4, but they don't know that a disconnect
                    // has occurred. We can update them by correcting their ID.
                    clientsThatNeedUpdating.push_back(clientIDVector.at(i));
                    clientIDVector.at(i) = clientIDVector.at(i) - 1;
                }
                playerInfoVector.erase(playerInfoVector.begin() + index, playerInfoVector.begin() + index + 5);
                // All elements are shifted automatically.
            }
            else { // Otherwise, if the connection is to remain open,
             // We will need to update the correct set of coordinates in the player info vector.
                playerInfoVector.at(index) = currentPlayerX - offset;
                playerInfoVector.at(index + 1) = currentPlayerY;
                // Setting the colors is unnecessary because the client only needs to know about them once.
            }



            // The client is now expecting a message in return. Send them one. But we have what we need now.
            // The client only needs its ID and the positions and colors of the static platforms.
            std::string reply;
            reply.append(std::to_string(clientID).append(" "));
            reply.append(std::to_string(deltaT));// The client terms are separated by strings!

            // Now we need to give the client information about the static platforms on the map.

            // WHEN IT SENDS A MESSAGE BACK WE CAN GIVE IT AN ID and DELTAT
            replySocket.send(zmq::buffer(reply), zmq::send_flags::none);
            // This was the final step. after we are done with the preparation of the states of each client,
            // we can let the main thread know that the passed-in data structures are ready to be worked with.


            //// Great! That was long. Now we have all of the data we need to send to other clients. We will end up adding more data eventually.
            //// The client needs to know all of the above information (except the ID) plus the position of the moving platform.
            //// Each of these terms needs to be added to a primitive list so that JSON can parse it for each client.
            //// EXCEPT the moving platform position, because the moving platform should be the same for EVERY client, so each
            //// client will only have one set of coordinates to read from.


            //// This should be all of the information the player will need. We copy the information
            //// we got into the JSON object.
            //structure["xCoords"] = playerInfoVector;
            //structure["platformCoords"] = movingPlatformCoords;
            //structure["isMovingRight"] = movingPlatform.getIsMovingRight();

        }

        for (int i = 0; i < stringPlatVect.size(); i++) {
            stringPlatVect.at(i) = platformVect.at(i)->to_string();
        }


        structure["playerInfo"] = playerInfoVector;
        structure["platformInfo"] = stringPlatVect;
        structure["disconnected"] = disconnectedVect;


        // Now we convert the message into a string.
        std::string subscriberMessage = structure.dump();

        // Now the JSON object is also updated with accurate info.
        // We can send this updated object to our subscribed clients now:

        publisherSocket.send(zmq::buffer(subscriberMessage), zmq::send_flags::none);

        // We must reset the disconnectedVect after each iteration because next iteration
        // there MAY not be any disconnects.
        disconnectedVect = {};
        

        //***********************************************************************************************
        /** END SERVER STUFF */
        //***********************************************************************************************
        
        // The server doesn't have to draw anything, so no window.
    }
    //std::unique_lock<std::mutex> movementLock(mutex); // don't edit while another thread might be looking
    running = false;

    movementCV.notify_all(); // let all the threads know to terminate

    platformMovementThread.join();

    return 0;
}