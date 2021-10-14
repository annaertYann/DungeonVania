#include "pch.h"
#include "Barrel.h"
#include "utils.h"
Barrel::Barrel(Texture* texture, const Point2f& pos,float scale)
	:m_DestRect{ pos.x,pos.y,200*scale,200*scale }
	,m_SourceRect{0,128,64,64}
	,m_pBarrelSprite{texture}
	, m_IsDead{ false }
	,m_IsHit{false}
	,m_MaxFrames{7}
	,m_AccuSec{}
{
	//choose wich box
	if (rand() % 2 == 0)
	{
		m_SourceRect.bottom += 128;
	}
}
Barrel::~Barrel()
{
}
void Barrel::Draw()const
{
	m_pBarrelSprite->Draw(m_DestRect, m_SourceRect);
}
void Barrel::update(float elapsedSec)
{
	float frameRate{0.1f};
	if (m_SourceRect.left >=( m_MaxFrames-1) * m_SourceRect.width)
	{
		m_IsDead = true;
	}
	else
		if (m_IsHit&&!m_IsDead)
		{
			m_AccuSec += elapsedSec;
			if (m_AccuSec>frameRate)
			{
				m_SourceRect.left += m_SourceRect.width;
				m_AccuSec = 0;
			}
		}
}
bool Barrel::HitBarrelReturnDead(const Rectf& actorShape)
{
	if (utils::IsOverlapping(Rectf{ m_DestRect.left+m_DestRect.width/4,m_DestRect.bottom+m_DestRect.height/4,m_DestRect.width-m_DestRect.width/2,m_DestRect.height-m_DestRect.height/2}, actorShape))
	{
		m_IsHit = true;
	}
	return m_IsDead;
}
Rectf Barrel::GetShape()
{
	return Rectf{ m_DestRect.left + m_DestRect.width / 4,m_DestRect.bottom + m_DestRect.height / 4,m_DestRect.width - m_DestRect.width / 2,m_DestRect.height - m_DestRect.height / 2 };
}