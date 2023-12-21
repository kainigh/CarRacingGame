#include "Game.h"
#include "Timer.h"
#include "Assets.h"
#include "Maths.h"
#include "Car.h"

#include <iostream>
#include <cmath>

using namespace std;

auto p1 = new Car();
auto p2 = new Car();

bool CheckCollision(Car& one, Car& two);

bool Game::initialize()
{
	bool isWindowInit = window.initialize();
	bool isRendererInit = renderer.initialize(window);
	return isWindowInit && isRendererInit; // Return bool && bool && bool ...to detect error
}

Game::~Game()
{
	delete p1;
	delete p2;
	TTF_Quit();
}

void Game::load()
{
	Assets::loadTexture(renderer, "yellow-car.png", "car01");
	Assets::loadTexture(renderer, "green-car.png", "car02");
	Assets::loadTexture(renderer, "track_wall.png", "wall");
	Assets::loadTexture(renderer, "track_road.png", "road");
	Assets::loadTexture(renderer, "track_flag.png", "flag");
	Assets::loadTexture(renderer, "track_tree.png", "tree");
	Assets::loadTexture(renderer, "track_goal.png", "goal");

	drawTrack();

	p1->carInit("Yellow Car", "car01");
	p1->carReset();
	p1->setupControls(SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D);

	p2->carInit("Green Car", "car02");
	p2->carReset();
	p2->setupControls(SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT);

	TTF_Init();
	
	SoundEngine->play2D("rock-loop.wav", true);

	ReadySetGo();
	
}

void Game::ReadySetGo()
{

	renderer.beginDraw();

	renderer.draw();
	drawText("Ready", window.getWidth() / 2 - 50, window.getHeight() / 2 - 100, 244, 0, 0, 100);

	renderer.endDraw();
	SDL_Delay(1000);

	renderer.beginDraw();

	renderer.draw();
	drawText("Set", window.getWidth() / 2 - 50, window.getHeight() / 2 - 100, 244, 0, 0, 100);

	renderer.endDraw();
	SDL_Delay(1000);

	renderer.beginDraw();

	renderer.draw();
	drawText("GO", window.getWidth() / 2 - 50, window.getHeight() / 2 - 100, 244, 0, 0, 100);

	renderer.endDraw();
	SDL_Delay(1000);

}

void Game::processInput()
{
	// SDL Event
	SDL_Event event;


	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			SoundEngine->drop();
			break;
		}

	}


	p1->carMove();
	p2->carMove();

	if (CheckCollision(*p1, *p2))
	{

		p1->carSpeed = -0.80f * p1->carSpeed;
		p2->carSpeed = -0.80f * p2->carSpeed;	

	}


	if (p1->resetGame)
	{
		renderer.beginDraw();

		renderer.draw();

		drawText("Winner ", window.getWidth() / 2 - 50, window.getHeight() / 2 - 100, 244, 200, 0, 100);
		drawText(p1->carName, window.getWidth() / 2 - 100, window.getHeight() / 2 + 50, 244, 200, 0, 100);

		renderer.endDraw();

		SDL_Delay(2000);

		p1->carReset();
		p2->carReset();

		p1->resetGame = false;
		p2->resetGame = false;

		ReadySetGo();

	}
	else if (p2->resetGame)
	{
		renderer.beginDraw();

		renderer.draw();
		
		drawText("Winner ", window.getWidth()/2 - 50, window.getHeight() / 2 - 100, 175, 255, 20, 100);
		drawText(p2->carName, window.getWidth() / 2 - 100, window.getHeight() / 2 + 50, 175, 255, 20, 100);

		renderer.endDraw();

		SDL_Delay(3000);

		p1->carReset();
		p2->carReset();

		p1->resetGame = false;
		p2->resetGame = false;

		ReadySetGo();

	}



	// Keyboard state
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
	// Escape: quit game
	if (keyboardState[SDL_SCANCODE_ESCAPE])
	{
		isRunning = false;
	}

	// Actor input
	isUpdatingActors = true;
	for (auto actor : actors)
	{
		actor->processInput(keyboardState);
	}
	isUpdatingActors = false;

}

bool CheckCollision(Car& one, Car& two) // AABB - AABB collision
{
	// collision x-axis?
	bool collisionX = one.carPos.x + one.carSize >= two.carPos.x &&
		two.carPos.x + two.carSize >= one.carPos.x;
	// collision y-axis?
	bool collisionY = one.carPos.y+ one.carSize >= two.carPos.y &&
		two.carPos.y + two.carSize >= one.carPos.y;
	// collision only if on both axes
	return collisionX && collisionY;
}

void Game::update(float dt)
{
	// Update actors 
	isUpdatingActors = true;
	for (auto actor : actors)
	{
		actor->update(dt);
	}
	isUpdatingActors = false;

	// Move pending actors to actors
	for (auto pendingActor : pendingActors)
	{
		actors.emplace_back(pendingActor);
	}
	pendingActors.clear();

	// Delete dead actors
	vector<Actor*> deadActors;
	for (auto actor : actors)
	{
		if (actor->getState() == Actor::ActorState::Dead)
		{
			deadActors.emplace_back(actor);
		}
	}
	for (auto deadActor : deadActors)
	{
		delete deadActor;
	}
}

void Game::render()
{

	renderer.beginDraw();

	renderer.draw();

	renderer.endDraw();

}

int Game::trackTileToIndex(int tileCol, int tileRow)
{
	return (tileCol + TRACK_COLS * tileRow);
}

bool Game::isWallAtTileCoord(int trackTileCol, int trackTileRow) {
	int	trackIndex = trackTileToIndex(trackTileCol, trackTileRow);
	return	(trackGrid[trackIndex] == TRACK_WALL);
}

int Game::getTrackAtPixelCoord(float pixelX, float pixelY)
{
	
	float tileCol = pixelX / TRACK_W;
	float tileRow = pixelY / TRACK_H;

	//	we'll	use	Math.floor	to	round	down	to	the	nearest	whole	number
	tileCol = floor(tileCol);
	tileRow = floor(tileRow);


	//	first	check	whether	the	car	is	within	any	part	of	the	track	wall
	if (tileCol < 0 || tileCol >= TRACK_COLS ||
		tileRow < 0 || tileRow >= TRACK_ROWS) {
		
		return TRACK_WALL;	//	bail	out	of	function	to	avoid	illegal	array	position	usage
	}



	int trackIndex = trackTileToIndex(tileCol, tileRow);

	return	trackGrid[trackIndex];

}

void Game::drawText(const char* msg, int x, int y, int r, int g, int b, int size)
{
	SDL_Surface* surf;
	SDL_Texture* tex;
	TTF_Font* font = TTF_OpenFont("DEFTONE.ttf", size);
	SDL_Rect rect;
	SDL_Color color, bg;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = 255;

	bg.r = 255;
	bg.g = 0;
	bg.b = 255;
	bg.a = 100;
	
	surf = TTF_RenderText_Solid(font, msg, color);

	tex = SDL_CreateTextureFromSurface(renderer.toSDLRenderer(), surf);
	rect.x = x;
	rect.y = y;
	rect.w = surf->w;
	rect.h = surf->h;
	SDL_FreeSurface(surf);
	SDL_RenderCopy(renderer.toSDLRenderer(), tex, NULL, &rect);
	SDL_DestroyTexture(tex);

}


void Game::drawTrack()
{
	int	trackIndex = 0;
	float trackLeftEdgeX = 0;
	float trackTopEdgeY = 0;

	for (int eachRow = 0; eachRow < TRACK_ROWS; eachRow++)
	{
		trackLeftEdgeX = 0;

		for (int eachCol = 0; eachCol < TRACK_COLS; eachCol++)
		{
				int	trackTypeHere = trackGrid[trackIndex];

				
				if (trackTypeHere == TRACK_WALL)
				{
					auto wall = new Actor();
					auto wallSprite = new SpriteComponent(wall, Assets::getTexture("wall"));

					wall->setPosition(Vector2{ TILE_OFFSET + trackLeftEdgeX, TILE_OFFSET + trackTopEdgeY });
				}
				else if (trackTypeHere == TRACK_GOAL)
				{
					auto goal = new Actor();
					auto goalSprite = new SpriteComponent(goal, Assets::getTexture("goal"));

					goal->setPosition(Vector2{ TILE_OFFSET + trackLeftEdgeX, TILE_OFFSET + trackTopEdgeY });
				}
				else if (trackTypeHere == TRACK_TREE)
				{
					auto tree = new Actor();
					auto treeSprite = new SpriteComponent(tree, Assets::getTexture("tree"));

					tree->setPosition(Vector2{ TILE_OFFSET + trackLeftEdgeX, TILE_OFFSET + trackTopEdgeY });
				}
				else if (trackTypeHere == TRACK_FLAG)
				{
					auto flag = new Actor();
					auto flagSprite = new SpriteComponent(flag, Assets::getTexture("flag"));

					flag->setPosition(Vector2{ TILE_OFFSET + trackLeftEdgeX, TILE_OFFSET + trackTopEdgeY });
				}
				else
				{

					auto road = new Actor();
					auto roadSprite = new SpriteComponent(road, Assets::getTexture("road"));

					road->setPosition(Vector2{ TILE_OFFSET + trackLeftEdgeX, TILE_OFFSET + trackTopEdgeY });
				}
			
				trackIndex++;
				trackLeftEdgeX += TRACK_W;

		}

		trackTopEdgeY += TRACK_H;

	}

}

void Game::loop()
{

	
	Timer timer;
	float dt = 0;

	
	while (isRunning)
	{
		float dt = timer.computeDeltaTime() / 1000.0f;
		processInput();
		update(dt);
		
		render();
		
		timer.delayTime();
	}

}

void Game::unload()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!actors.empty())
	{
		delete actors.back();
	}

	// Resources
	Assets::clear();
}

void Game::close()
{
	renderer.close();
	window.close();
	SDL_Quit();
}

void Game::addActor(Actor* actor)
{
	if (isUpdatingActors)
	{
		pendingActors.emplace_back(actor);
	}
	else
	{
		actors.emplace_back(actor);
	}
}

void Game::removeActor(Actor* actor)
{
	// Erase actor from the two vectors
	auto iter = std::find(begin(pendingActors), end(pendingActors), actor);
	if (iter != end(pendingActors))
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, end(pendingActors) - 1);
		pendingActors.pop_back();
	}
	iter = std::find(begin(actors), end(actors), actor);
	if (iter != end(actors))
	{
		std::iter_swap(iter, end(actors) - 1);
		actors.pop_back();
	}
}
