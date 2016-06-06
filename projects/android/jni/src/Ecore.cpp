#include "Ecore.h"
#include "ETexture.h"

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Ecore::Ecore() :
gWindow(NULL),
gRenderer(NULL),
gFont(NULL),
boxTexture(this),
textTexture(this)
{
}

Ecore::~Ecore()
{
	close();
}

inline bool Ecore::handleEvent(SDL_Event *e)
{
	bool result = true;
	//Handle events on queue
	while (SDL_PollEvent(e) != 0)
	{
		//User requests quit
		if (e->type == SDL_QUIT)
		{
			result = false;
		}
		else if (e->type == SDL_KEYDOWN)
		{
			switch (e->key.keysym.sym)
			{
			case SDLK_a:
				break;
			}
		}
	}
	return result;
}

void Ecore::Start()
{
	//Start up SDL and create window
	if (!init())
	{
		//printf("Failed to initialize!\n");
		ERROR("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			//printf("Failed to load media!\n");
			ERROR("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			
			//Flip type
			SDL_RendererFlip flipType = SDL_FLIP_NONE;
			
			/* Time management */
			const Uint32 MAX_FRAMESKIP = 5;			/* Maximun count of skipping frame rendering */
			const Uint32 UPDATE_TICKS_PER_SECOND = 100;	/* Tick time for each update function. it affects game speed */
			const Uint32 RENDER_TICKS_PER_SECOND = 64;	/* Tick time for each render function. it affects FPS */
			Uint32 t = 0;
			const Uint32 SKIP_UPDATE_TICKS = 1000 / UPDATE_TICKS_PER_SECOND;
			const Uint32 SKIP_RENDER_TICKS = 1000 / RENDER_TICKS_PER_SECOND;
			
			Uint32 accumulator = 0;
			Uint32 prevTime = SDL_GetTicks();
			Uint32 currentTime = prevTime;
			Uint32 nextUpdateTick = prevTime;
			Uint32 nextRenderTick = prevTime;
			Uint32 frameTime = 0;
			Uint32 prevCalculated = prevTime;
			int loops = 0;
			int updated = 0;
			int rendered = 0;
			double alpha = 0.0;

			INFO("Start !!");
			// While application is running
			while (!quit)
			{
				/* Check Event */
				if (handleEvent(&e) == false) {
					quit = true;
				}

				currentTime = SDL_GetTicks();
				frameTime = currentTime - prevTime;	/* elapsed time from last frame rendering */
				accumulator += frameTime;
				
				//SDL_Log("Prev: %f / Curr: %f / frameTime: %f", prevTime, currentTime, frameTime);
				//INFO("Prev: %d / Curr: %d / frameTime: %d", prevTime, currentTime, frameTime);

				//frameTime = SDL_min(frameTime, maxFrameTime);
				//while (frameTime > 0.0)	{
				//while (accumulator > 0.0)	{
				loops = 0;
				while (currentTime > nextUpdateTick && loops < MAX_FRAMESKIP)
				{
					//const Uint32 deltaTime = SDL_min(accumulator, dt);

					//frameTime -= deltaTime;
					//accumulator -= deltaTime;
					//accumulator -= frameTime;

					// minus value of Uint32
					//accumulator -= SKIP_UPDATE_TICKS;
					//t += deltaTime;

					/* Update Sprites */
					//Update(currentTime, deltaTime);
					//alpha = accumulator / SKIP_UPDATE_TICKS;
					Update(currentTime);

					/* Set next tick time */
					nextUpdateTick += SKIP_UPDATE_TICKS;

					currentTime = SDL_GetTicks();

					loops++;

					//SDL_Log("t: %f / frameTime: %f / dt: %f", t, frameTime, dt);
					INFO("Curr [%d] / next [%d] / loops [%d]", currentTime, nextUpdateTick, loops);
#if 0
					INFO("acc [%d] / t: [%d] / frameTime: [%d] / dt: [%d]",
						accumulator, t, frameTime, dt);
#endif
				}
				updated += loops;

				/* Calculate alpha accumulator */
				alpha = float(currentTime + SKIP_UPDATE_TICKS - nextUpdateTick) / float(SKIP_UPDATE_TICKS);
				//SDL_Log("Render !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				//INFO("Render !! / Updated : [%d] times", loops);
#if 1
				/* Limit frames */
				//currentTime = SDL_GetTicks();
				//if (currentTime - prevTime <= 1000 / 60) {
				if (currentTime - prevCalculated > 1000) {
					//SDL_Delay(1000 / 60);
					INFO("Updated: [%d] / Rendered [%d]", updated, rendered);
					updated = 0;
					rendered = 0;
					//continue;
					prevCalculated = currentTime;
				}
#endif
				prevTime = currentTime;

				currentTime = SDL_GetTicks();
				if (currentTime <= nextRenderTick) {
					//nextRenderTick += SKIP_RENDER_TICKS;
					continue;
				}
				nextRenderTick += SKIP_RENDER_TICKS;
				//INFO("currentTime: [%d] / nextRenderTick [%d]", currentTime, nextRenderTick);

				/* Render screen */
				Render(currentTime);

				rendered++;

				/* Put a delay to lower CPU usage */
				SDL_Delay(5);
			}
		}
	}

	/* Free resources and close SDL */
	close();

	return ;
}

void Ecore::Render(Uint32 currentTime)
{
	static double d_fps = 0.0;
	static Uint32 prevTime = 0;
	static unsigned int drawed_frames = 0;

	char str[256] = { 0, };

	// Update latest position
	Update(currentTime);

	// Increase frame counter
	drawed_frames++;

	// Calculate FPS
	currentTime = SDL_GetTicks();
	if (currentTime - prevTime >= 1000) {
		d_fps = (double)drawed_frames / (double)(currentTime - prevTime) * 1000.0;
		prevTime = currentTime;
		drawed_frames = 1;
	}
	
	// Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(gRenderer);

	// Render arrow
	boxTexture.draw();

	// Render Text
	static SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
	static SDL_Color bgColor = { 0x0, 0x0, 0x0 };
	SDL_snprintf(str, 256, "FPS: %0.2f", d_fps);
	if (!textTexture.loadFromRenderedText(str, textColor, bgColor))
	{
		//printf("Failed to render text texture!\n");
		ERROR("Failed to render text texture!\n");
	}
	//textTexture.render(SCREEN_WIDTH - textTexture.getWidth() - 10, 10);
	textTexture.render(50, 10);

	// Render Color
	//colorTexture.draw();
	//colorTexture.render_resize(64, 64, 0, drawed_frames / 10, 0, SDL_FLIP_NONE);

	//Update screen
	SDL_RenderPresent(gRenderer);
}

void Ecore::Update(Uint32 currentTime)
{
	//boxTexture.calculate(elapsedTime, deltaTime);
	boxTexture.calculate(currentTime);
}

bool Ecore::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		ERROR("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			//printf("Warning: Linear texture filtering not enabled!");
			ERROR("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial",
				SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			//printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			ERROR("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			/* VSync: SDL_RENDERER_PRESENTVSYNC */
			gRenderer = SDL_CreateRenderer(gWindow, -1,
					SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				//printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				ERROR("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					//printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					ERROR("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					//printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					ERROR("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool Ecore::loadMedia()
{
	//Loading success flag
	bool success = true;

	// Load Font
	//Open the font
	gFont = TTF_OpenFont("consola.ttf", 28);
	if (gFont == NULL)
	{
		//printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		ERROR("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	//Load arrow
	if (!boxTexture.loadFromFile("kachan.png"))
	{
		//printf("Failed to load box texture!\n");
		ERROR("Failed to load box texture!\n");
		success = false;
	}

	return success;
}

void Ecore::close()
{
	//Free loaded images
	boxTexture.free();

	//Free fonts
	textTexture.free();

	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

SDL_Renderer* Ecore::getRenderer()
{
	return gRenderer;
}

TTF_Font* Ecore::getFont()
{
	return gFont;
}