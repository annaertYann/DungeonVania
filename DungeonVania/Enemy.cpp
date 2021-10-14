#include "pch.h"
#include "Enemy.h"
#include "utils.h"
Enemy::Enemy(float scale,int collomsIdle)
	:m_DestRect{ 100,100,250 * scale,186 * scale }
	, m_SourceRect{}
	, m_MovementSpeed{ scale * 65 }
	, m_Health{ 1 }
	, m_IsHit{ false }
	, m_AccuSec{}
	, m_Width{ 100 }
	, m_Height{ 50 }
	, m_CurrentRow{ 1 }
	, m_Colloms{ 5 }
	, m_AccuSecAnimation{}
	, m_IsGoingRight{ false }
	, m_IsAttacking{ false }
	, m_AttackAccuSec{}
	, m_IsDead{ false }
	, m_Scale{ scale }
	, m_HitBox{}
	,m_CollomsIdle{collomsIdle}
	,m_HasMovementFreePass{}
	,m_IsinDeadAnimation{false}
	,m_PreviousPos{}
	,m_StopMoving{false}
	,m_FireBall{"Resources/Sound/fireball.mp3"}
	,m_Dying{"Resources/Sound/M Dying.mp3"}
	,m_Jump{"Resources/Sound/Jump.mp3"}
	,m_Laser{"Resources/Sound/laser.mp3"}
{
	m_SourceRect = Rectf{ 0,0,m_Width,m_Height };
	m_HitBox = Rectf{ m_DestRect.left + m_Scale * 40,m_DestRect.bottom,m_DestRect.width - m_Scale * 100,m_DestRect.height };
}
Enemy::~Enemy()
{

}
void Enemy::Update(float elapsedsec, const Point2f& destination)
{
	m_Manager.Update(elapsedsec);
	MoveEnemy(elapsedsec, destination);
	UpdateSprite(elapsedsec);
	if (m_MovementSpeed != 0)
	{
		//no attack when dead animation
		Attack(elapsedsec, m_Scale, destination);
	}
	UpdateHitBox();

}
void Enemy::UpdateSprite(float elapsedSec)
{
	float frameRate{0.1f};
	m_AccuSecAnimation += elapsedSec;
	if (m_AccuSecAnimation > frameRate)
	{
		m_SourceRect.left += m_Width;
		m_SourceRect.bottom = m_CurrentRow * m_Height;
		m_AccuSecAnimation = 0;
	}
	if (m_SourceRect.left >= m_Width * (m_Colloms - 1))
	{
		m_SourceRect.left = 0;
		m_AccuSecAnimation = 0;
		m_IsAttacking = false;
		m_HasMovementFreePass = false;
		m_MovementSpeed = 64 * m_Scale;
		m_CurrentRow = 1;
		m_SourceRect.bottom = m_CurrentRow * m_Height;
		m_Colloms = m_CollomsIdle;
		if (m_Health <= 0)
		{
			m_IsDead = true;
		}
	}

}
bool Enemy::IsHit(const Rectf& hitBox)
{
	if (utils::IsOverlapping(hitBox, m_HitBox) == true)
	{
			return true;
	
	}
	else
	{
		return false;
	}
}
float Enemy::GetHealth()
{
	return m_Health;
}
void Enemy::DoDamage(float damage)
{
	m_Health -= damage;
	m_IsHit = true;
	if (m_IsAttacking == false)
	{
		if (!m_IsinDeadAnimation)
		{
			m_CurrentRow = 3;
			m_Colloms = 3;
			m_SourceRect.left = 0;
		}
	}
	if (m_Health <= 0)
	{
		
		if (!m_IsinDeadAnimation)
		{
			m_Dying.Play(0);
			m_MovementSpeed = 0;
			m_CurrentRow = 4;
			m_Colloms = 6;
			m_SourceRect.left = 0;
			m_AccuSecAnimation = 0;
		}
		m_IsinDeadAnimation = true;
	}
}
bool Enemy::GetIsDead()
{
	return m_IsDead;
}
std::vector<Spell*>& Enemy::GetSpells()
{
	return m_Manager.GetSpells();
}
void Enemy::MoveEnemy(float elapsedsec,const Point2f& destination)
{
	m_PreviousPos = Point2f{m_DestRect.left,m_DestRect.bottom};
	if (m_StopMoving)
	{
		m_MovementSpeed = 0;
	}
		if (m_IsHit == false || m_HasMovementFreePass)
		{
			//no bugging in player
			if (abs(destination.x - m_DestRect.left) >= 5)
			{
				//move left or right
				if (destination.x > m_DestRect.left)
				{
					m_DestRect.left += m_MovementSpeed * elapsedsec;
					m_IsGoingRight = true;
				}
				else
				{
					m_DestRect.left -= m_MovementSpeed * elapsedsec;
					m_IsGoingRight = false;
				}

			}
			//no bugging in player
			if (abs(destination.y - m_DestRect.bottom) > 5)
			{
				//move up or down
				if (destination.y > m_DestRect.bottom)
				{
					m_DestRect.bottom += m_MovementSpeed * elapsedsec;
				}
				else
				{
					m_DestRect.bottom -= m_MovementSpeed * elapsedsec;
				}
			}
		}
		else
		{
			//stop moving when hit
			if (m_AccuSec < 0.1f)
			{
				m_AccuSec += elapsedsec;
			}
			else
			{
				m_AccuSec = 0;
				m_IsHit = false;
			}
		}
		if (m_StopMoving)
		{
			m_MovementSpeed = 64 * m_Scale;
		}
}
void Enemy::UpdateHitBox()
{
	m_HitBox = Rectf{ m_DestRect.left + m_Scale * 40,m_DestRect.bottom,m_DestRect.width - m_Scale * 100,m_DestRect.height };
}
Rectf Enemy::GetShape()
{
	return m_HitBox;
}
void Enemy::SetPreviousPos()
{
	m_DestRect.left = m_PreviousPos.x;
	m_DestRect.bottom = m_PreviousPos.y;
}
void Enemy::ChangeVolume(int newVolume)
{
	m_Laser.SetVolume(newVolume);
	m_Jump.SetVolume(newVolume);
	m_FireBall.SetVolume(newVolume);
	m_Dying.SetVolume(newVolume);


}