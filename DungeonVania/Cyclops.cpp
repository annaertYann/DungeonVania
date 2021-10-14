#include "pch.h"
#include "Cyclops.h"
#include "utils.h"
Cyclops::Cyclops(float scale)
	:Enemy::Enemy(scale,12)
	,m_CyclopsSprite{"Resources/Cyclops.png"}
	,m_SpellTimer{0.1f}
	,m_SecondAttackAccuSec{}
	,m_Scale{scale}
	,m_IsFirstAttack{false}
	,m_FirstAttackAccuSec{}
{
}
Cyclops::~Cyclops()
{
	
}
void Cyclops::Draw()const
{

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
	m_Manager.Draw();
}
void Cyclops::Attack(float elapsedSec, float scale, const Point2f& destination)
{
	m_SecondAttackAccuSec += elapsedSec;
	if (m_SecondAttackAccuSec >= m_SpellTimer&&m_IsAttacking==false&&m_IsFirstAttack==false)
	{
		int attackChance{ rand() % 200 };
		if (attackChance == 1)
		{
			m_Laser.Play(0);
			m_SecondAttackAccuSec = 0;
			m_CurrentRow = 2;
			m_Colloms = 6;
			m_SourceRect.left = 0;
			m_AccuSecAnimation = 0;
			m_IsAttacking = true;
			m_Manager.AddSpell(SpellManager::SpellType::laser, Point2f{ m_DestRect.left,m_DestRect.bottom }, scale, Vector2f{ destination }, Point2f{ 0,0 });
		}
		attackChance = rand() % 100;
		if (attackChance == 1)
		{
			m_Laser.Play(0);
			m_IsAttacking = true;
			m_SecondAttackAccuSec = 0;
			m_CurrentRow = 2;
			m_Colloms = 6;
			m_SourceRect.left = 0;
			m_AccuSecAnimation = 0;
			m_IsFirstAttack = true;
			m_Manager.AddSpell(SpellManager::SpellType::groundSlam, Point2f{ m_DestRect.left,m_DestRect.bottom }, scale, Vector2f{ destination }, Point2f{ 0,0 });
		}
	}

	if (m_IsFirstAttack)
	{
		m_FirstAttackAccuSec += elapsedSec;
		if (m_FirstAttackAccuSec>0.1)
		{
			m_Manager.AddSpell(SpellManager::SpellType::groundSlam, Point2f{ m_DestRect.left,m_DestRect.bottom }, scale, Vector2f{ destination }, Point2f{ 0,0 });
			m_FirstAttackAccuSec = 0;
		}
		if (m_SecondAttackAccuSec>5)
		{
			m_SecondAttackAccuSec = 0;
			m_IsFirstAttack = false;
			m_IsAttacking = false;
		}
	}
}
void Cyclops::UpdateSprite(float elapsedSec)
{
	m_AccuSecAnimation += elapsedSec;
	float frameRate{0.1f};
	if (m_IsAttacking == false)
	{
		if (m_AccuSecAnimation > frameRate)
		{
			m_SourceRect.left += m_Width;
			m_SourceRect.bottom = m_CurrentRow * m_Height;
			m_AccuSecAnimation = 0;
		}
		if (m_SourceRect.left >= m_Width * m_Colloms - 1)
		{
			m_SourceRect.left = 0;
			m_AccuSecAnimation = 0;
			if (m_Health <= 0)
			{
				m_IsDead = true;
			}
		}
	}
	else 
	{
		if (m_AccuSecAnimation > frameRate )
		{
			m_SourceRect.left += m_Width;
			if (m_SourceRect.left >= m_Width * m_Colloms - 1)
			{
				m_SourceRect.left -= m_Width;
				if (m_Health <= 0)
				{
					m_IsAttacking = false;
				}
			}
			m_SourceRect.bottom = m_CurrentRow * m_Height;
			m_AccuSecAnimation = 0;
		}
	}
}
void Cyclops::Update(float elapsedsec, const Point2f& destination)
{
	m_Manager.Update(elapsedsec);
	UpdateSprite(elapsedsec);
	if (m_MovementSpeed != 0||m_IsHit == false)
	{
		if (m_IsAttacking == false)
		{
			MoveEnemy(elapsedsec, destination);
			if (m_IsHit == false)
			{
				m_CurrentRow = 1;
			}
		}
		else
		{
			m_CurrentRow = 2;
			m_SecondAttackAccuSec += elapsedsec;
			if (m_SecondAttackAccuSec >= 5)
			{
				m_AccuSecAnimation = 0;
				m_IsAttacking = false;
				m_SourceRect.left = 0;
				m_CurrentRow = 1;
				for (Spell*& element : m_Manager.GetSpells())
				{
					if (element != nullptr)
					{
						if (element->GetType() == Spell::SpellType::laser)
						{
							delete element;
							element = nullptr;
						}
					}
				}
			}
		}
	}
		if (m_MovementSpeed != 0 )
		{
			//no attack when dead animation
			Attack(elapsedsec, m_Scale, destination);
		}
		UpdateHitBox();
}