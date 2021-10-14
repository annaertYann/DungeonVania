#include "pch.h"
#include "Santa.h"
#include "utils.h"
Santa::Santa(float scale)
	:Enemy::Enemy(scale, 8)
	, m_CyclopsSprite{ "Resources/Santa.png" }
{
}
Santa::~Santa()
{
}
void  Santa::Draw()const
{
	m_Manager.Draw();
	if (m_IsGoingRight == true)
	{
		m_CyclopsSprite.Draw(m_DestRect, m_SourceRect);
	}
	else
	{
		glPushMatrix();
		glTranslatef(2 * m_DestRect.left + m_DestRect.width, 0, 0);
		glScalef(-1, 1, 1);
		m_CyclopsSprite.Draw(m_DestRect, m_SourceRect);
		glPopMatrix();
	}
	if (m_Health > 0)
	{
		utils::SetColor(Color4f{ 1,0,0,1 });
		utils::FillRect(Rectf{ m_DestRect.left,m_DestRect.bottom + m_DestRect.height * 1.1f,m_DestRect.width * m_Health,m_DestRect.height / 4 });
	}
}
void  Santa::Attack(float elapsedSec, float scale, const Point2f& destination)
{
	m_AttackAccuSec += elapsedSec;
	if (m_AttackAccuSec >= 5)
	{
		m_Jump.Play(0);
			m_AttackAccuSec = 0;
			m_CurrentRow = 2;
			m_Colloms = 8;
			m_SourceRect.left = 0;
			m_AccuSecAnimation = 0;
			m_IsAttacking = true;
			m_HasMovementFreePass = true;
			m_MovementSpeed = 300 * scale;
	}
}