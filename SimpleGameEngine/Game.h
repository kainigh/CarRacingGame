#pragma once

#include <algorithm>
#include <vector>
#include "Window.h"
#include "Renderer.h"
#include "Vector2.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include <irrklang/irrKlang.h>
#include <SDL_ttf.h>

using namespace irrklang;


using std::vector;

class Game
{
public:

	enum 
	{
		TRACK_ROAD = 0,
		TRACK_WALL = 1,
		TRACK_PLAYER = 2,
		TRACK_GOAL = 3,
		TRACK_TREE = 4,
		TRACK_FLAG = 5
	};


	static Game& instance()
	{
		static Game inst;
		return inst;
	}

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;
	~Game();
	Game() : isRunning(true),
		isUpdatingActors(false)
	{}

	

public:
	bool initialize();
	void loop();
	void load();
	void unload();
	void close();

	void addActor(Actor* actor);
	void removeActor(Actor* actor);

	void drawText(const char* msg, int x, int y, int r, int g, int b, int size);

	int getTrackAtPixelCoord(float pixelX, float pixelY);

	Renderer& getRenderer() { return renderer; }
	ISoundEngine* SoundEngine = createIrrKlangDevice();

	bool isUpdatingActors;
	vector<Actor*> actors;
	vector<Actor*> pendingActors;


private:
	void processInput();
	void update(float dt);
	void render();
	void drawTrack();
	bool isWallAtTileCoord(int trackTileCol, int trackTileRow);
	
	int trackTileToIndex(int tileCol, int tileRow);
	void ReadySetGo();
	bool isRunning;
	Window window;
	Renderer renderer;
	
	
	
	Vector2 wall;
	
	Vector2 carVelocity;
	Vector2 nextPos;
	const float carSize = 10.0f;

public:

	int TILE_OFFSET = 20;
	int TRACK_COLS = 20;
	int TRACK_ROWS = 15;

	int TRACK_W = 40;
	int TRACK_H = 40;

	int trackGrid[20 * 15] = {  4,	4,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	4,
								4,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,
								1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,
								1,	0,	0,	0,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	0,	0,	1,
								1,	0,	0,	0,	1,	1,	1,	4,	4,	4,	4,	1,	1,	1,	1,	1,	1,	0,	0,	1,
								1,	0,	0,	1,	1,	0,	0,	1,	4,	4,	1,	1,	0,	0,	0,	1,	1,	0,	0,	1,
								1,	0,	0,	1,	0,	0,	0,	0,	1,	4,	1,	0,	0,	0,	0,	0,	1,	0,	0,	1,
								1,	0,	0,	1,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	1,	0,	0,	1,
								1,	2,	2,	1,	0,	0,	0,	0,	0,	0,	1,	0,	0,	5,	0,	0,	1,	0,	0,	1,
								1,	0,	0,	1,	0,	0,	5,	0,	0,	0,	5,	0,	0,	1,	0,	0,	1,	0,	0,	1,
								1,	1,	1,	1,	0,	0,	1,	1,	0,	0,	0,	0,	0,	1,	0,	0,	5,	0,	0,	1,
								1,	1,	5,	1,	0,	0,	1,	1,	1,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	1,
								0,	3,	0,	0,	0,	0,	1,	4,	1,	1,	0,	0,	1,	1,	0,	0,	0,	0,	0,	1,
								0,	3,	0,	0,	0,	0,	1,	4,	4,	1,	1,	1,	1,	1,	1,	0,	0,	0,	1,	1,
								1,	1,	5,	1,	1,	1,	1,	4,	4,	4,	4,	4,	4,	1,	1,	1,	1,	1,	1,	1 };


};
