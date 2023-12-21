#pragma once
#include "Actor.h"
#include "Game.h"
#include "Vector2.h"




class Car : public Game
{
public:

	Car();
	void carInit(const char* myName, string carType);
	void carMove();
	void carReset();
	void setKeyHoldState(SDL_Scancode thisKey, Car thisCar, bool setTo);
	void setupControls(SDL_Scancode forwardKey, SDL_Scancode backKey, SDL_Scancode leftKey, SDL_Scancode rightKey);

	Actor* car;

	SDL_Scancode controlKeyForGas, controlKeyForReverse, controlKeyForTurnLeft, controlKeyForTurnRight;

	bool keyHeld_Gas, keyHeld_Reverse, keyHeld_TurnLeft, keyHeld_TurnRight;

	bool resetGame;

	Vector2 carPos;

	const float carSize = 10.0f;

	float carSpeed = 0.0f;

	const char* carName;

private:

	


	Vector2 carVelocity;
	Vector2 nextPos;
	Vector2 homePos;

	

	
	float carAng = 0.0f;

	const float GROUNDSPEED_DECAY_MULT = 0.94f;
	const float DRIVE_POWER = 0.25f;
	const float REVERSE_POWER = 0.2f;
	const float TURN_RATE = 0.03f;
	const float MIN_TURN_SPEED = 0.5f;

};

