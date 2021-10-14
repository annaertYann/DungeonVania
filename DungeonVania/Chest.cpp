#include "pch.h"
#include "Chest.h"
Chest::Chest(Texture* texture, const Point2f& pos, float scale)
	:m_pChestSprite{ texture }
	, m_DestRect{ pos.x,pos.y,250 * scale,250 * scale }
	, m_SourceRect{ 0,768,64,64 }
	, m_IsHit{ false }
	, m_MaxFrames{ 5 }
	, m_AccuSec{}
	, m_IsOpen{ false }
	,m_CoinDestRect{ pos.x,pos.y+m_DestRect.height/1.5f,250 * scale,250 * scale }
	,m_CoinSourceRect{0,448,64,64}
	,m_Scale{scale}
	,m_IsCoinDone{false}
	
{
}

Chest::~Chest()
{
}
void Chest::Draw()const
{
	m_pChestSprite->Draw(m_DestRect, m_SourceRect);
	if (m_IsOpen&&!m_IsCoinDone)
	{
		m_pChestSprite->Draw(m_CoinDestRect, m_CoinSourceRect);
	}
}
void Chest::update(float elapsedSec)
{
	if (!m_IsOpen)
	{
		if (m_IsHit)
		{
			m_AccuSec += elapsedSec;
			if (m_AccuSec > 0.4)
			{
				m_SourceRect.left += m_SourceRect.width;
				m_AccuSec += elapsedSec;
			}
			if (m_SourceRect.left == m_SourceRect.width * (m_MaxFrames - 1))
			{
				m_IsOpen = true;
				m_AccuSec = 0;
				
			}
		}
	}
	if (!m_IsCoinDone)
	{
		if (m_IsOpen)
		{
			m_AccuSec += elapsedSec;
			m_CoinDestRect.bottom += 50 * m_Scale * elapsedSec;
			if (m_AccuSec > 2)
			{
				m_IsCoinDone = true;
			}
		}
	}
}
void Chest::ChestHit()
{
	m_IsHit = true;
}
Rectf Chest::GetShape()
{
	return m_DestRect;
}