#include "Car.h"
#include "SpriteComponent.h"
#include "Assets.h"
#include "Maths.h"

#include <cmath>
#include <iostream>

using namespace std;



Car::Car()
	: Game() { 

	
	/*this->keyHeld_Gas = false;
	this->keyHeld_Reverse = false;
	this->keyHeld_TurnLeft = false;
	this->keyHeld_TurnRight = false;*/

	
}

void Car::carInit(const char* myName, string carType)
{

	auto actor = new Actor();
	auto sprite = new SpriteComponent(actor, Assets::getTexture(carType));
	this->carName = myName;

	car = actor;

}

void Car::carMove()
{

	carPos.x = car->getPosition().x;
	carPos.y = car->getPosition().y;

	carPos.y += cos(carAng) * carSpeed;
	carPos.x += sin(carAng) * carSpeed;

	nextPos.x = carPos.x + sin(carAng) * carSpeed;
	nextPos.y = carPos.y + cos(carAng) * carSpeed;

	int	drivingIntoTileType = Game::instance().getTrackAtPixelCoord(nextPos.x, nextPos.y);

	

	if (drivingIntoTileType == TRACK_ROAD)
	{
		
		carPos.x = nextPos.x;
		carPos.y = nextPos.y;
		car->setPosition(Vector2{ carPos.x, carPos.y });

	}
	else if (drivingIntoTileType == TRACK_GOAL) {
		cout << this->carName << " won the race" << endl;
		
		SoundEngine->play2D("score-beep.mp3", false);
		//winner = this->carName.c_str();

		resetGame = true;

		
		

	}
	else
	{
			SoundEngine->play2D("crash.wav", false);

		this->carSpeed = -0.5 * carSpeed;
		
	}


	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
	

	if (keyboardState[this->controlKeyForGas])
	{
		this->carSpeed -= DRIVE_POWER;
	}
	if (keyboardState[this->controlKeyForReverse])
	{
		this->carSpeed += REVERSE_POWER;
	}

	if ( abs(carSpeed) >= MIN_TURN_SPEED)
	{
		if (keyboardState[this->controlKeyForTurnLeft])
		{
			
			this->carAng += TURN_RATE * Maths::pi;

			this->car->setRotation(carAng);

		}
		if (keyboardState[this->controlKeyForTurnRight])
		{
			
			this->carAng -= TURN_RATE * Maths::pi;

			this->car->setRotation(carAng);
		}
	}

	carSpeed = carSpeed * GROUNDSPEED_DECAY_MULT;

}

void Car::setKeyHoldState(SDL_Scancode thisKey, Car thisCar, bool setTo)
{

	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

	

	if (keyboardState[thisKey] == keyboardState[thisCar.controlKeyForTurnLeft])
	{
		thisCar.keyHeld_TurnLeft = setTo;
	}

	if (keyboardState[thisKey] == keyboardState[thisCar.controlKeyForTurnRight])
	{
		thisCar.keyHeld_TurnRight = setTo;
	}

	if (keyboardState[thisKey] == keyboardState[thisCar.controlKeyForGas])
	{
		thisCar.keyHeld_Gas = setTo;
	}

	if (keyboardState[thisKey] == keyboardState[thisCar.controlKeyForReverse])
	{
		thisCar.keyHeld_Reverse = setTo;
	}


}

void Car::setupControls(SDL_Scancode forwardKey, SDL_Scancode backKey, SDL_Scancode leftKey, SDL_Scancode rightKey)
{
	this->controlKeyForGas = forwardKey;
	this->controlKeyForReverse = backKey;
	this->controlKeyForTurnLeft = leftKey;
	this->controlKeyForTurnRight = rightKey;
}

void Car::carReset()
{
	carSpeed = 0.0f;
	carAng = 0.0f;
	car->setRotation(0.0f);


	if (!this->homePos.x)
	{

		for (int i = 0; i <= sizeof(Game::instance().trackGrid); i++) {
			if (Game::instance().trackGrid[i] == TRACK_PLAYER) {

				float	tileRow = floor(i / TRACK_COLS);
				float	tileCol = i % TRACK_COLS;

				this->homePos.x = tileCol * TRACK_W + 0.5 * TRACK_W;
				this->homePos.y = tileRow * TRACK_H + 0.5 * TRACK_H;

				Game::instance().trackGrid[i] = Game::instance().TRACK_ROAD;

				break;	//	found	it,	so	no	need	to	keep	searching	

			}
		}
	}
		
	car->setPosition(Vector2{ this->homePos.x, this->homePos.y });

	
}

