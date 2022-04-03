#include"Enemy.h"

void Enemy::MoveSequence()
{
	if (m_device->getTimer()->getTime() - enemy_get_move_anim_timer() >= TIME_FOR_ANIM && !GetDeathFlag())
	{
		irr::scene::ISceneNodeAnimator* anim = 0;
		irr::core::vector3df prev_pos = GetPos();
		enemy_move();
		anim = m_device->getSceneManager()->createFlyStraightAnimator(
			prev_pos, GetPos(), TIME_FOR_ANIM, false);
		GetModel()->addAnimator(anim);
		anim->drop();
		enemy_set_move_anim_timer(m_device->getTimer()->getTime());
	}
}

void Enemy::enemy_move()
{
	if (GetPos().X - m_starting_pos.X < 200 && m_dir == LEFT)
	{
		Move(LEFT);
	}
	else if (GetPos().X - m_starting_pos.X >= 200 && m_dir == LEFT)
	{
		m_dir = RIGHT;
	}
	if (GetPos().X - m_starting_pos.X > -200 && m_dir == RIGHT)
	{
		Move(RIGHT);
	}
	else if (GetPos().X - m_starting_pos.X <= -200 && m_dir == RIGHT)
	{
		m_dir = LEFT;
	}
}

Bullet* Enemy::enemy_shoot()
{
	irr::core::vector3df bullet_pos = m_pos;
	return new Bullet(m_device, bullet_pos, m_rot, 1);
}

void Enemy::enemy_set_move_anim_timer(int time)
{
	m_move_anim_timer = time;
}

void Enemy::enemy_set_shot_timer(int time)
{
	m_shot_timer = time;
}

void Enemy::EnemyInit(irr::core::vector3df pos)
{
	m_mesh = m_sm->getMesh("../media/spaceship_02.obj");
	m_model = m_sm->addMeshSceneNode(m_mesh);
	if (m_model)
	{
		m_model->setMaterialTexture(0, m_driver->getTexture("../media/texture/T_Ships_red.png"));
		m_model->setPosition(pos);
		m_model->setRotation(irr::core::vector3df(0, 180, 0));
	}
	m_objectSelector = m_sm->createTriangleSelector(m_model->getMesh(), m_model);
	m_model->setTriangleSelector(m_objectSelector);
	m_starting_pos = m_model->getPosition();
	m_pos = m_starting_pos;
	m_rot = m_model->getRotation();
}