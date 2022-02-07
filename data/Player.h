#pragma once
#include "Object.h"
#define PLAYER_SPEED 1

class Player : public Object
{
public:
	void PlayerMove(const irr::EKEY_CODE& event);
	Player(irr::IrrlichtDevice* device) : Object(device)
	{
		m_mesh = m_sm->getMesh("../media/spaceship_02.obj");
		m_model = m_sm->addMeshSceneNode(m_mesh);
		if (m_model)
		{
			m_model->setMaterialTexture(0, m_driver->getTexture("../media/texture/T_Ships_blue.png"));
			m_model->setPosition(irr::core::vector3df(0, PLAYER_START_POS_Y, 0));
			m_model->setMaterialFlag(irr::video::EMF_LIGHTING, true);
			m_model->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
			m_model->addShadowVolumeSceneNode();
		}
		m_speed = PLAYER_SPEED;
		m_objectSelector = m_sm->createTriangleSelector(m_model->getMesh(), m_model);
		m_model->setTriangleSelector(m_objectSelector);
		m_death_flag = 0;
		m_starting_pos = m_model->getPosition();
		m_pos = m_starting_pos;
		m_rot = m_model->getRotation();
	 }

private:
	irr::core::vector3df m_starting_pos;
	int dir;
};