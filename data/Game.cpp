// This is a Demo of the Irrlicht Engine (c) 2005-2009 by N.Gebhardt.
// This file is not documented.

#include "Game.h"

Game::Game(bool f, bool m, bool s, bool a, bool v, bool fsaa, video::E_DRIVER_TYPE d)
	: fullscreen(f), music(m), shadows(s), additive(a), vsync(v), aa(fsaa),
	driverType(d), device(0),
#ifdef USE_IRRKLANG
	irrKlang(0), ballSound(0), impactSound(0),
#endif
#ifdef USE_SDL_MIXER
	stream(0), ballSound(0), impactSound(0),
#endif
	currentScene(-2), backColor(0), statusText(0), inOutFader(0),
	quakeLevelMesh(0), quakeLevelNode(0), skyboxNode(0), model1(0), model2(0),
	campFire(0), metaSelector(0), mapSelector(0), sceneStartTime(0),
	timeForThisScene(0), m_player(), enemies_created()
{
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
}

Game::~Game()
{
	if (mapSelector)
		mapSelector->drop();

	if (metaSelector)
		metaSelector->drop();

#ifdef USE_IRRKLANG
	if (irrKlang)
		irrKlang->drop();
#endif
}
void Game::KeyProcessing()
{
	if (m_player)
	{
		if (IsKeyDown(KEY_KEY_W))
			m_player->PlayerMove(KEY_KEY_W);
		else if (IsKeyDown(irr::KEY_KEY_S))
			m_player->PlayerMove(KEY_KEY_S);
		if (IsKeyDown(irr::KEY_KEY_A))
			m_player->PlayerMove(KEY_KEY_A);
		else if (IsKeyDown(irr::KEY_KEY_D))
			m_player->PlayerMove(KEY_KEY_D);
		if (IsKeyDown(irr::KEY_SPACE))
		{
			if (device->getTimer()->getTime() - m_player->player_get_shot_timer() >= 800 && !m_player->GetDeathFlag())
			{
				m_player->player_set_shot_timer(device->getTimer()->getTime());
				m_bullets.push_back(std::make_shared < Bullet>(*m_player->PlayerShoot()));
				if (m_player->GetFlag() >= 5)
				{
					auto rot = m_player->GetRot();
					rot.Y -= 20;
					for (int i = 0; i < 2; i++)
					{
						m_bullets.push_back(std::make_shared <Bullet>(*new Bullet(device, m_player->GetPos(), rot)));
						rot.Y += 40;
					}
				}
				if (m_player->GetFlag() >= 12)
				{
					auto rot = m_player->GetRot();
					rot.Y -= 10;
					for (int i = 0; i < 2; i++)
					{
						m_bullets.push_back(std::make_shared <Bullet>(*new Bullet(device, m_player->GetPos(), rot)));
						rot.Y += 20;
					}
				}
				irrklang::ISound* snd = irrKlang->play2D("../media/ball.wav");
			}
		}
		if (!(IsKeyDown(irr::KEY_KEY_D) || IsKeyDown(irr::KEY_KEY_A)))
			m_player->PlayerMove(KEY_KEY_0);
	}
}

void Game::run()
{
	core::dimension2d<u32> resolution(RES_H, RES_W);

	irr::SIrrlichtCreationParameters params;
	params.DriverType = driverType;
	params.WindowSize = resolution;
	params.Bits = 32;
	params.Fullscreen = fullscreen;
	params.Stencilbuffer = shadows;
	params.Vsync = vsync;
	params.AntiAlias = aa ? 8 : 0;
	params.EventReceiver = this;

	device = createDeviceEx(params);
	if (!device)
		return;

	if (device->getFileSystem()->existFile("map-20kdm2.pk3"))
		device->getFileSystem()->addFileArchive("map-20kdm2.pk3");
	else
		device->getFileSystem()->addFileArchive("../media/map-20kdm2.pk3");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	device->setWindowCaption(L"Irrlicht Engine Demo");

	// set ambient light
	smgr->setAmbientLight(video::SColorf(0x00c0c0c0));

	wchar_t tmp[255];

	// draw everything

	s32 now = 0;
	s32 lastfps = 0;
	sceneStartTime = device->getTimer()->getTime();
	srand(device->getTimer()->getRealTime());
	while (device->run() && driver)
	{
		if (device->isWindowActive())
		{
#ifdef USE_IRRKLANG
			// update 3D position for sound engine
			scene::ICameraSceneNode* cam = smgr->getActiveCamera();
			if (cam && irrKlang)
				irrKlang->setListenerPosition(cam->getAbsolutePosition(), cam->getTarget());
#endif

			//// load next scene if necessary
			now = device->getTimer()->getTime();
			if (now - sceneStartTime > timeForThisScene && timeForThisScene != -1)
				switchToNextScene();

			createParticleImpacts();

			driver->beginScene(timeForThisScene != -1, true, backColor);
			if (m_enemies.size() == 0)
			{
				m_level += 1;
				CreateLevel(m_level);
				for (auto i : m_bullets)
					i->SetDeathCounter(0);
				enemies_created = true;
			}
			smgr->drawAll();
			guienv->drawAll();
			driver->endScene();
			if (enemies_created == true)
			{
				IntroEnemies();
				if (m_enemies.back()->GetPos().Z == last_enemy_pos)
					enemies_created = false;
			}
			else
			{
				KeyProcessing();
				move_bullets();
				move_enemies();
				enemies_shoot();
			}

			// write statistics
			const s32 nowfps = driver->getFPS();

			if (m_player)
			{
				swprintf(tmp, 255, L"%ls fps:%3d triangles:%0.3f mio/s points: %d stage: %d",
					driver->getName(), driver->getFPS(),
					driver->getPrimitiveCountDrawn(1) * (1.f / 1000000.f), m_player->GetFlag(), m_level);
			}
			statusText->setText(tmp);
			if (nowfps != lastfps)
			{
				device->setWindowCaption(tmp);
				lastfps = nowfps;
			}
		}
	}

	device->drop();
}


bool Game::OnEvent(const SEvent& event)
{
	if (!device)
		return false;

	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

	if (event.EventType == EET_KEY_INPUT_EVENT &&
		event.KeyInput.Key == KEY_ESCAPE &&
		event.KeyInput.PressedDown == false)
	{
		// user wants to quit.
		if (currentScene < 3)
			timeForThisScene = 0;
		else
			device->closeDevice();
	}
	return false;
}

void Game::MakeEnemies(int amt, int length, int line, int type)
{
	int l;
	if (amt == 1)
	{
		l = 0;
	}
	else
	{
		l = -1 * length / 2;
	}
	for (int i = 0; i < amt; i++)
	{
		m_enemies.push_back(std::make_shared<Enemy>(*new Enemy(device, core::vector3df(l, PLAYER_START_POS_Y, 600 + 75 * line), i, type)));
		last_enemy_pos = 200 +75*line;
		if (amt != 1)
		{
			l += length / (amt - 1);
		}
	}
}

void Game::switchToNextScene()
{
	currentScene++;
	if (currentScene > 3)
		currentScene = 1;

	scene::ISceneManager* sm = device->getSceneManager();
	scene::ISceneNodeAnimator* sa = 0;
	scene::ICameraSceneNode* camera = 0;

	camera = sm->getActiveCamera();
	if (camera)
	{
		sm->setActiveCamera(0);
		camera->remove();
		camera = 0;
	}

	switch (currentScene)
	{
	case -1: // loading screen
		timeForThisScene = 0;
		createLoadingScreen();
		break;

	case 0: // load scene
		timeForThisScene = 0;
		loadSceneData();
		break;

	case 3: // interactive, go around
	{
		timeForThisScene = -1;
		camera = sm->addCameraSceneNode(0, core::vector3df(100, 40, -80), m_player->GetPos());
		camera->setPosition(core::vector3df(0, PLAYER_START_POS_Y + 150, -150));
		camera->setFarValue(5000.0f);
	}
	break;
	}

	sceneStartTime = device->getTimer()->getTime();
}


void Game::loadSceneData()
{
	// load quake level

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* sm = device->getSceneManager();

	// Quake3 Shader controls Z-Writing
	sm->getParameters()->setAttribute(scene::ALLOW_ZWRITE_ON_TRANSPARENT, true);

	quakeLevelMesh = (scene::IQ3LevelMesh*)sm->getMesh("maps/20kdm2.bsp");

	if (quakeLevelMesh)
	{
		u32 i;

		//move all quake level meshes (non-realtime)
		core::matrix4 m;
		m.setTranslation(core::vector3df(-1300, -70, -1249));

		for (i = 0; i != scene::quake3::E_Q3_MESH_SIZE; ++i)
			sm->getMeshManipulator()->transform(quakeLevelMesh->getMesh(i), m);

		quakeLevelNode = sm->addOctreeSceneNode(
			quakeLevelMesh->getMesh(scene::quake3::E_Q3_MESH_GEOMETRY));
		if (quakeLevelNode)
		{
			//quakeLevelNode->setPosition(core::vector3df(-1300,-70,-1249));
			quakeLevelNode->setVisible(false);

			// create map triangle selector
			mapSelector = sm->createOctreeTriangleSelector(quakeLevelMesh->getMesh(0),
				quakeLevelNode, 128);
		}
	}

	// create sky box
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	skyboxNode = sm->addSkyBoxSceneNode(
		driver->getTexture("../media/sbox.jpg"),
		driver->getTexture("../media/sbox.jpg"),
		driver->getTexture("../media/sbox.jpg"),
		driver->getTexture("../media/sbox.jpg"),
		driver->getTexture("../media/sbox.jpg"),
		driver->getTexture("../media/sbox.jpg"));
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	// create meta triangle selector with all triangles selectors in it.
	metaSelector = sm->createMetaTriangleSelector();
	metaSelector->addTriangleSelector(mapSelector);

	m_player = std::make_shared<Player>(*new Player(device));
	m_player->SetInvincibleCounter(100);

	// load music

#ifdef USE_IRRKLANG
	if (music)
		startIrrKlang();
#endif
#ifdef USE_SDL_MIXER
	if (music)
		startSound();
#endif
}

void Game::createLoadingScreen()
{
	core::dimension2d<u32> size = device->getVideoDriver()->getScreenSize();

	device->getCursorControl()->setVisible(false);

	// setup loading screen

	backColor.set(255, 90, 90, 156);

	// create in fader

	inOutFader = device->getGUIEnvironment()->addInOutFader();
	inOutFader->setColor(backColor, video::SColor(0, 230, 230, 230));

	// loading text

	const int lwidth = size.Width - 20;
	const int lheight = 16;

	core::rect<int> pos(10, size.Height - lheight - 10, 10 + lwidth, size.Height - 10);

	device->getGUIEnvironment()->addImage(pos);
	statusText = device->getGUIEnvironment()->addStaticText(L"Loading...", pos, true);
	statusText->setOverrideColor(video::SColor(255, 205, 200, 200));

	// load bigger font

	device->getGUIEnvironment()->getSkin()->setFont(
		device->getGUIEnvironment()->getFont("../media/fonthaettenschweiler.bmp"));


	// set new font color

	device->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_BUTTON_TEXT,
		video::SColor(255, 100, 100, 100));
}

void Game::bullet_collision(std::shared_ptr<Bullet>bullet)
{
	scene::ISceneManager* sm = device->getSceneManager();

	core::vector3df end = bullet->GetPos();
	end.Y += BULLET_SIZE;

	core::triangle3df triangle;
	end.X += BULLET_SIZE;

	const core::line3d<f32> line1(bullet->GetPos(), end);

	// get intersection point with map
	scene::ISceneNode* hitNode;
	for (auto it = m_enemies.begin(); it < m_enemies.end(); it++)
	{
		auto a = *it;
		if (sm->getSceneCollisionManager()->getCollisionPoint(
			line1, a->GetSelector(), end, triangle, hitNode) && bullet->GetFlag() == 0)
		{
			a->Death();
			bullet->SetDeathCounter(0);
			m_enemies.erase(it);
			m_player->SetFlag(m_player->GetFlag() + 1 + a->GetFlag());
			irrklang::ISound* snd = irrKlang->play2D("../media/impact.wav");
			break;
		}
	}
	if (sm->getSceneCollisionManager()->getCollisionPoint(
		line1, m_player->GetSelector(), end, triangle, hitNode) && bullet->GetFlag() != 0)
	{
		if (m_player->GetFlag() < 1)
		{
			if (m_player->GetInvincibleCounter() <= 0)
				device->closeDevice();
		}
		else
		{
			m_player->SetFlag(0);
			m_player->SetInvincibleCounter(50);
		}
		bullet->SetDeathCounter(0);
	}
	end.X -= BULLET_SIZE * 2;
	const core::line3d<f32> line2(bullet->GetPos(), end);
	for (auto it = m_enemies.begin(); it < m_enemies.end(); it++)
	{
		auto a = *it;
		if (sm->getSceneCollisionManager()->getCollisionPoint(
			line1, a->GetSelector(), end, triangle, hitNode) && bullet->GetFlag() == 0)
		{
			a->Death();
			bullet->SetDeathCounter(0);
			m_enemies.erase(it);
			m_player->SetFlag(m_player->GetFlag() + 1);
			irrklang::ISound* snd = irrKlang->play2D("../media/impact.wav");
			break;
		}
	}
	if (sm->getSceneCollisionManager()->getCollisionPoint(
		line2, m_player->GetSelector(), end, triangle, hitNode) && bullet->GetFlag() == 1)
	{
		if (m_player->GetFlag() < 5)
		{
			if (m_player->GetInvincibleCounter() <= 0)
				device->closeDevice();
		}
		else
		{
			m_player->SetFlag(0);
			m_player->SetInvincibleCounter(50);
		}
		bullet->SetDeathCounter(0);
	}
}

void Game::enemies_shoot()
{
	auto now = device->getTimer()->getTime();
	for (const auto& a : m_enemies)
	{
		if (a && !a->GetDeathFlag() && now - a->enemy_get_shot_timer() >= ENEMY_SHOT_DELAY)
		{
			irrklang::ISound* snd = irrKlang->play2D("../media/ball.wav");
			a->enemy_set_shot_timer(now - rand() % (ENEMY_SHOT_DELAY / 2));
			auto rot = a->GetRot();
			switch (a->GetFlag())
			{
			case 0:
				m_bullets.push_back(std::make_shared <Bullet>(*new Bullet(device, a->GetPos(), rot, 1)));
				break;
			case 1:
				m_bullets.push_back(std::make_shared <Bullet>(*new Bullet(device, a->GetPos(), rot, 1)));
				rot.Y -= 20;
				for (int i = 0; i < 2; i++)
				{
					m_bullets.push_back(std::make_shared <Bullet>(*new Bullet(device, a->GetPos(), rot, 1)));
					rot.Y += 40;
				}
				break;
			case 2:
				m_bullets.push_back(std::make_shared <Bullet>(*new Bullet(device, a->GetPos(), rot, 2)));
				break;
			}
		}
	}
}

void Game::move_enemies()
{
	for (const auto& a : m_enemies)
	{
		if (a)
			a->MoveSequence();
	}
}

void Game::move_bullets()
{
	if (m_bullets.empty())
	{
		return;
	}
	for (auto it = m_bullets.begin(); it < m_bullets.end(); it++)
	{
		auto bullet = *it;
		if (m_bullets.front()->GetDeathCounter() <= 0)
		{
			bullet->Death();
			m_bullets.erase(it);
			break;
		}
		bullet->MoveBullet();
		bullet_collision(bullet);
	}

}

void Game::CreateLevel(const int level_num)
{
	switch (level_num)
	{
	case 1:
		MakeEnemies(1, 300, 0, 1);
		MakeEnemies(3, 300, 1, 2);
		break;
	case 2:
		MakeEnemies(2, 100, -1, 0);
		MakeEnemies(3, 150, 0, 0);
		MakeEnemies(4, 200, 1, 0);
		break;
	case 3:
		MakeEnemies(2, 100, 1, 1);
		MakeEnemies(2, 150, 0, 1);
		break;
	case 4:
		MakeEnemies(5, 300, -2, 0);
		MakeEnemies(7, 350, 1, 0);
		break;
	case 5:
		MakeEnemies(3, 200, -1, 0);
		MakeEnemies(4, 150, 0, 0);
		MakeEnemies(3, 200, 1, 1);
		break;
	}
}

void Game::createParticleImpacts()
{
	u32 now = device->getTimer()->getTime();
	scene::ISceneManager* sm = device->getSceneManager();

	for (s32 i = 0; i < (s32)Impacts.size(); ++i)
		if (now > Impacts[i].when)
		{
			// create smoke particle system
			scene::IParticleSystemSceneNode* pas = 0;

			pas = sm->addParticleSystemSceneNode(false, 0, -1, Impacts[i].pos);

			pas->setParticleSize(core::dimension2d<f32>(10.0f, 10.0f));

			scene::IParticleEmitter* em = pas->createBoxEmitter(
				core::aabbox3d<f32>(-5, -5, -5, 5, 5, 5),
				Impacts[i].outVector, 20, 40, video::SColor(50, 255, 255, 255), video::SColor(50, 255, 255, 255),
				1200, 1600, 20);

			pas->setEmitter(em);
			em->drop();

			pas->setMaterialFlag(video::EMF_LIGHTING, false);
			pas->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
			pas->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/smoke.bmp"));
			pas->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

			scene::ISceneNodeAnimator* anim = sm->createDeleteAnimator(2000);
			pas->addAnimator(anim);
			anim->drop();

			// play impact sound
#ifdef USE_IRRKLANG
			if (irrKlang)
			{
				irrklang::ISound* sound =
					irrKlang->play3D(impactSound, Impacts[i].pos, false, false, true);

				if (sound)
				{
					// adjust max value a bit to make to sound of an impact louder
					sound->setMinDistance(400);
					sound->drop();
				}
			}
#endif

#ifdef USE_SDL_MIXER
			if (impactSound)
				playSound(impactSound);
#endif

			// delete entry
			Impacts.erase(i);
			i--;
		}
}

void Game::IntroEnemies()
{
	for (auto obj : m_enemies)
	{
		if (device->getTimer()->getTime() - obj->enemy_get_move_anim_timer() >= TIME_FOR_ANIM && !obj->GetDeathFlag())
		{
			auto prev_cord = obj->GetPos();
			obj->Move(DOWN);
			irr::scene::ISceneNodeAnimator* anim = 0;
			anim = device->getSceneManager()->createFlyStraightAnimator(
				prev_cord, obj->GetPos(), TIME_FOR_ANIM, false);
			obj->GetModel()->addAnimator(anim);
			anim->drop();
			obj->enemy_set_move_anim_timer(device->getTimer()->getTime());
		}
	}
}

#ifdef USE_IRRKLANG
void Game::startIrrKlang()
{
	irrKlang = irrklang::createIrrKlangDevice();

	if (!irrKlang)
		return;

	// play music

	irrklang::ISound* bgsnd = irrKlang->play2D("../media/IrrlichtTheme.ogg", true, false, true);
	if (!bgsnd)
		bgsnd = irrKlang->play2D("IrrlichtTheme.ogg", true, false, true);

	if (bgsnd)
	{
		bgsnd->setVolume(0.6f); // 50% volume
		bgsnd->drop();
	}

	// preload both sound effects

	ballSound = irrKlang->getSoundSource("../media/ball.wav");
	impactSound = irrKlang->getSoundSource("../media/impact.wav");
}
#endif


#ifdef USE_SDL_MIXER
void Game::startSound()
{
	stream = NULL;
	ballSound = NULL;
	impactSound = NULL;

	SDL_Init(SDL_INIT_AUDIO);

	if (Mix_OpenAudio(22050, AUDIO_S16, 2, 128))
		return;

	stream = Mix_LoadMUS("../media/IrrlichtTheme.ogg");
	if (stream)
		Mix_PlayMusic(stream, -1);

	ballSound = Mix_LoadWAV("../media/ball.wav");
	impactSound = Mix_LoadWAV("../media/impact.wav");
}

void Game::playSound(Mix_Chunk* sample)
{
	if (sample)
		Mix_PlayChannel(-1, sample, 0);
}

void Game::pollSound(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
		;
}
#endif
