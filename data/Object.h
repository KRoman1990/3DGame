#pragma once

#include <irrlicht.h>

#ifdef _IRR_WINDOWS_
#include <windows.h>
#endif
#define TIME_FOR_ANIM 100

enum directions
{
	UP = 0,
	LEFT = 1,
	DOWN = 2,
	RIGHT = 3
};
#define BASE_SPEED 10
#define PLAYER_START_POS_Y 800

class Object
{
public:
	void Move(int dir);
	void SetRotation(int number) { m_rot.Z = number; m_model->setRotation(m_rot); }
	void Death() {
		m_objectSelector = nullptr;  m_model->remove(); m_death_flag = 1;
	}
	Object(){}
	irr::core::vector3df GetPos() { return m_pos; }
	irr::core::vector3df GetRot() { return m_rot; }
	irr::scene::ITriangleSelector* GetSelector() { return m_objectSelector; }
	irr::scene::IMeshSceneNode* GetModel() { return m_model; }
	bool GetDeathFlag() { return m_death_flag; }

protected:
	Object(irr::IrrlichtDevice* device)
	{
		m_device = device;
		m_driver = m_device->getVideoDriver();
		m_sm = m_device->getSceneManager();
	}

	int m_speed;

	irr::scene::IMesh* m_mesh;
	irr::core::vector3df m_pos;
	irr::core::vector3df m_rot;
	irr::scene::IMeshSceneNode* m_model;
	irr::scene::ITriangleSelector* m_objectSelector;
	irr::IrrlichtDevice* m_device;
	irr::video::IVideoDriver* m_driver;
	irr::scene::ISceneManager* m_sm;
	bool m_death_flag;
};