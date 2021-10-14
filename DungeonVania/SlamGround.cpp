#include "pch.h"
#include "SlamGround.h"
#include "utils.h"
SlamGround::SlamGround(const Point2f& pos, float scale, const Vector2f& velocity, const Point2f& PosInCamera, Texture* texture)
	:Spell::Spell(pos, scale, velocity, PosInCamera)
	,m_FireBallSprite{texture}
	,m_MoveCounter{}
	,m_Colloms{7}
	,m_Rows{7}
	,m_FrameRateAnimation{0.2f}
	, m_CurrentRow{}
	, m_HitBoxDiff{35*scale}
	
{
	m_SourceRect = Rectf{ 0,0,100,100 };
	m_DestRect = Rectf{ pos.x,pos.y,128 * scale,128 * scale };
	m_HitBox = Rectf{m_DestRect.left+m_HitBoxDiff,m_DestRect.bottom + m_HitBoxDiff,m_DestRect.width -2*m_HitBoxDiff,m_DestRect.height - 2*m_HitBoxDiff };
	m_PosInCamera = pos;
	m_MovementSpeed = 25*scale;

	
}

SlamGround::~SlamGround()
{
}
void SlamGround::Draw()const
{
	m_FireBallSprite->Draw(m_DestRect, m_SourceRect);
	
}
void SlamGround::Update(float elapsedSec)
{
	UpdateSprite(elapsedSec);
	UpdateSpell(elapsedSec);
}
Rectf SlamGround::GetShape()
{
	
	return m_HitBox;
}
bool SlamGround::ExplosionTime()
{
	return false;
}
bool SlamGround::GetExplosion()
{
	return false;
}
void SlamGround::UpdateSprite(float elapsedSec)
{
	m_AccuSec += elapsedSec;
	if (m_AccuSec>=m_FrameRateAnimation)
	{
		m_CurrentFrame++;
		if (m_CurrentFrame >= m_Colloms-1)
		{
			m_CurrentFrame = 0;
			m_CurrentRow++;

			if (m_CurrentRow == m_Rows&& m_CurrentFrame == m_Colloms - 3)
			{
				m_CurrentRow = 0;
				m_CurrentFrame = 0;
			}

		}
		m_SourceRect.left = m_CurrentFrame * m_SourceRect.width;
		m_SourceRect.bottom = m_CurrentRow * m_SourceRect.height;
	}
}
void SlamGround::UpdateSpell(float elapsedSec)
{
	m_MoveCounter += elapsedSec;
	m_MoveCounter = 0;
	float distanceX{ MousePos.x - m_PosInCamera.x };
	m_Velocity = distanceX * elapsedSec;
	m_Velocity = m_Velocity * elapsedSec;
	m_DestRect.left += m_Velocity * m_MovementSpeed;

	float distanceY{ MousePos.y - m_PosInCamera.y };
	m_Velocity = distanceY * elapsedSec;
	m_Velocity = m_Velocity * elapsedSec;
	m_DestRect.bottom += m_Velocity * m_MovementSpeed;

	m_HitBox = Rectf{ m_DestRect.left + m_HitBoxDiff,m_DestRect.bottom + m_HitBoxDiff,m_DestRect.width - 2 * m_HitBoxDiff,m_DestRect.height - 2 * m_HitBoxDiff };
}