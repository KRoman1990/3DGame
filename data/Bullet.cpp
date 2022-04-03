#include "Bullet.h"

void Bullet::MoveBullet()
{
	irr::scene::ISceneNodeAnimator* anim = 0;
	irr::core::vector3df prev_pos = GetPos();
	switch (m_flag)
	{
	case 1:
		m_pos.Z += m_speed * cos((m_rot.Y) * (3.14 / 180));
		m_pos.X += m_speed * sin((m_rot.Y) * (3.14 / 180));
		break;
	case 2:
		m_pos.Z += m_speed * cos((m_rot.Y) * (3.14 / 180));
		m_pos.X += m_speed * sin((m_pos.Z) * (3.14 / 180));
		break;
	}
	m_pos.Z += m_speed * cos((m_rot.Y) * (3.14/180));
	m_pos.X += m_speed * sin((m_rot.Y) * (3.14/180));
	anim = m_device->getSceneManager()->createFlyStraightAnimator(
		prev_pos, GetPos(), TIME_FOR_ANIM, false);
	GetModel()->setRotation(GetRot());
	GetModel()->addAnimator(anim);
	anim->drop();
	m_death_counter--;
}

void Bullet::BulletInit(irr::core::vector3df pos, irr::core::vector3df rot)
{
	m_mesh = m_sm->getMesh("../media/bullet.obj");
	m_model = m_sm->addMeshSceneNode(m_mesh);
	if (m_model)
	{
		m_model->setMaterialTexture(0, m_driver->getTexture("../media/texture/space_props_red.png"));
		m_model->setPosition(pos);
		m_model->setRotation(rot);
		m_model->setScale(irr::core::vector3df(20, 20, 20));
	}
	m_objectSelector = m_sm->createTriangleSelector(m_model->getMesh(), m_model);
	m_model->setTriangleSelector(m_objectSelector);
	m_starting_pos = m_model->getPosition();
	m_pos = m_starting_pos;
	m_rot = m_model->getRotation();
}