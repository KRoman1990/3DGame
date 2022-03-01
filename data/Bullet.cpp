#include "Bullet.h"

void Bullet::MoveBullet()
{
	irr::scene::ISceneNodeAnimator* anim = 0;
	irr::core::vector3df prev_pos = GetPos();

	m_pos.Z += m_speed * sin((m_rot.Y + 90) * (3.14/180));
	anim = m_device->getSceneManager()->createFlyStraightAnimator(
		prev_pos, GetPos(), TIME_FOR_ANIM, false);
	GetModel()->setRotation(GetRot());
	GetModel()->addAnimator(anim);
	anim->drop();
	m_death_counter--;
}