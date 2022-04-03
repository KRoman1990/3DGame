#include"Object.h"

void Object::Move(int dir)
{
	switch (dir)
	{
	case UP:
		m_pos.Z += m_speed;
		break;
	case LEFT:
		m_pos.X += m_speed;
		m_rot.Z = -15;
		break;
	case DOWN:
		m_pos.Z -= m_speed;
		break;
	case RIGHT:
		m_pos.X -= m_speed;
		m_rot.Z = 15;
		break;
	}
}