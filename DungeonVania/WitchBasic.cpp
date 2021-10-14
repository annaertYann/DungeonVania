#include "pch.h"
#include "WitchBasic.h"
#include "utils.h"
WitchBasic::WitchBasic(float scale, int collomsIdle,const Point2f& spawnLocation)
	:Enemy::Enemy(scale, collomsIdle)
	, m_WitchSprite{ "Resources/Witch.png" }
	,m_SpellAccuSec{0.1f}
{
	m_Width = 32.f;
	m_Height = 32.f;
	m_SourceRect = Rectf{ 0,0,m_Width,m_Height };
	m_HitBox = Rectf{ m_DestRect.left + m_Scale * 40,m_DestRect.bottom,m_DestRect.width - m_Scale * 100,m_DestRect.height };
	m_DestRect.left = spawnLocation.x;
	m_DestRect.bottom = spawnLocation.y;
	m_DestRect.width = 150 * scale;
	m_DestRect.height = 100 * scale;
}

WitchBasic::~WitchBasic()
{
	
}
void WitchBasic::Draw()const
{
	m_Manager.Draw();
	if (m_IsGoingRight == true)
	{
		m_WitchSprite.Draw(m_DestRect, m_SourceRect);
	}
	else
	{
		glPushMatrix();
		glTranslatef(2 * m_DestRect.left + m_DestRect.width, 0, 0);
		glScalef(-1, 1, 1);
		m_WitchSprite.Draw(m_DestRect, m_SourceRect);
		glPopMatrix();
	}

	if (m_Health > 0)
	{
		utils::SetColor(Color4f{ 1,0,0,1 });
		utils::FillRect(Rectf{ m_DestRect.left,m_DestRect.bottom + m_DestRect.height * 1.1f,m_DestRect.width * m_Health,m_DestRect.height / 4 });
	}
}
void WitchBasic::Attack(float elapsedSec, float scale,const Point2f& destination)
{
	
	m_AttackAccuSec += elapsedSec;
	if (m_AttackAccuSec >= m_SpellAccuSec)
	{
		int temp{ rand() % 50 };

		if (temp == 1)
		{
			if (!m_IsinDeadAnimation)
			{
				m_FireBall.Play(0);
				m_AttackAccuSec = 0;
				m_MovementSpeed = 0;
				m_CurrentRow = 2;
				m_Colloms = 8;
				m_SourceRect.left = 0;
				m_AccuSecAnimation = 0;
				m_IsAttacking = true;
				m_Manager.AddSpell(SpellManager::SpellType::groundSlam, Point2f{ m_DestRect.left,m_DestRect.bottom }, scale, Vector2f{ destination }, Point2f{ 0,0 });
			}
		}

	}
}
