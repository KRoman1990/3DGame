// This is a Demo of the Irrlicht Engine (c) 2006 by N.Gebhardt.
// This file is not documented.

#ifndef __C_DEMO_H_INCLUDED__
#define __C_DEMO_H_INCLUDED__

#define USE_IRRKLANG
//#define USE_SDL_MIXER

#include <irrlicht.h>
#include <list>
#include <vector>
#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"

#ifdef _IRR_WINDOWS_
#include <windows.h>
#endif

#include <memory>

using namespace irr;

// audio support

#ifdef USE_IRRKLANG
#include <irrKlang.h>	// problem here? go to http://www.ambiera.com/irrklang and download
						// the irrKlang library or undefine USE_IRRKLANG at the beginning
						// of this file.
#ifdef _IRR_WINDOWS_
#pragma comment (lib, "irrKlang.lib")
#endif
#endif
#ifdef USE_SDL_MIXER
# include <SDL/SDL.h>
# include <SDL/SDL_mixer.h>
#endif

#define SHOT_LENGTH 600
#define RES_W 600
#define RES_H 800

const int CAMERA_COUNT = 7;

class Game : public IEventReceiver
{
public:

	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	Game(bool fullscreen, bool music, bool shadows, bool additive, bool vsync, bool aa, video::E_DRIVER_TYPE driver);

	~Game();

	void run();

	virtual bool OnEvent(const SEvent& event);

private:
	void CreateLevel(const int level_num);
	void MakeEnemies(int amt, int length, int line, int type);
	void KeyProcessing();
	void createLoadingScreen();
	void loadSceneData();
	void switchToNextScene();
	void bullet_collision(std::shared_ptr<Bullet>bullet);
	void createParticleImpacts();
	void move_player(const SEvent& event);
	void move_enemies();
	void move_bullets();
	void enemies_shoot();
	void IntroEnemies();

	bool fullscreen;
	bool music;
	bool shadows;
	bool additive;
	bool vsync;
	bool aa;
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool enemies_created;
	video::E_DRIVER_TYPE driverType;
	IrrlichtDevice* device;

#ifdef USE_IRRKLANG
	void startIrrKlang();
	irrklang::ISoundEngine* irrKlang;
	irrklang::ISoundSource* ballSound;
	irrklang::ISoundSource* impactSound;
#endif

#ifdef USE_SDL_MIXER
	void startSound();
	void playSound(Mix_Chunk*);
	void pollSound();
	Mix_Music* stream;
	Mix_Chunk* ballSound;
	Mix_Chunk* impactSound;
#endif

	struct SParticleImpact
	{
		u32 when;
		core::vector3df pos;
		core::vector3df outVector;
	};

	int currentScene;
	int m_level = 0;
	int last_enemy_pos = 0;

	std::shared_ptr<Player> m_player;
	std::vector<std::shared_ptr<Enemy>> m_enemies;
	std::vector<std::shared_ptr<Bullet>> m_bullets;

	video::SColor backColor;

	gui::IGUIStaticText* statusText;
	gui::IGUIInOutFader* inOutFader;

	scene::IQ3LevelMesh* quakeLevelMesh;
	scene::ISceneNode* quakeLevelNode;
	scene::ISceneNode* skyboxNode;
	scene::IMeshSceneNode* model1;
	scene::IAnimatedMeshSceneNode* model2;
	scene::IMeshSceneNode* model3;
	scene::IParticleSystemSceneNode* campFire;

	scene::IMetaTriangleSelector* metaSelector;
	scene::ITriangleSelector* mapSelector;
	scene::ITriangleSelector* objectSelector;

	s32 sceneStartTime;
	s32 timeForThisScene;

	core::array<SParticleImpact> Impacts;
};

#endif

