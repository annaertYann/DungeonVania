#include "pch.h"
#include "Minotaur.h"
#include "utils.h"
#include "Texture.h"
#include <math.h>
#include "Enemy.h"
MinoTaur::MinoTaur(float scale)
	:Enemy::Enemy(scale,5)
	,m_MinotaurSprite{"Resources/Minotaur.png"}
	,m_SpellAccuSec{0.5f}
{
	m_CollomsIdle = 5;
	m_SourceRect = Rectf{0,0,m_Width,m_Height};
	m_HitBox = Rectf{m_DestRect.left+m_Scale*40,m_DestRect.bottom,m_DestRect.width-m_Scale*100,m_DestRect.height};
}
MinoTaur::~MinoTaur()
{

}
void MinoTaur::Draw()const
{
	m_Manager.Draw();
	if (m_IsGoingRight==true)
	{
		m_MinotaurSprite.Draw(m_DestRect, m_SourceRect);
	}
	else
	{
		glPushMatrix();
		glTranslatef(2 * m_DestRect.left + m_DestRect.width, 0, 0);
		glScalef(-1, 1, 1);
		m_MinotaurSprite.Draw(m_DestRect, m_SourceRect);
		glPopMatrix();
	}

	if (m_Health > 0)
	{
		utils::SetColor(Color4f{ 1,0,0,1 });
		utils::FillRect(Rectf{ m_DestRect.left,m_DestRect.bottom + m_DestRect.height * 1.1f,m_DestRect.width * m_Health,m_DestRect.height / 4 });
	}
}
void MinoTaur::Attack(float elapsedSec, float scale, const Point2f& destination)
{
	
	m_AttackAccuSec += elapsedSec;
	if (m_AttackAccuSec >= m_SpellAccuSec)
	{
		int temp{ rand() % 50 };
	
		if (temp == 1)
		{
			m_FireBall.Play(0);
			m_AttackAccuSec = 0;
			m_CurrentRow = 2;
			m_Colloms = 9;
			m_MovementSpeed = 0;
			m_SourceRect.left = 0;
			m_AccuSecAnimation = 0;
			m_IsAttacking = true;
			m_Manager.AddSpell(SpellManager::SpellType::groundSlam, Point2f{ m_DestRect.left,m_DestRect.bottom }, scale, Vector2f{ destination }, Point2f{ 0,0 });
		}
	
	}
}