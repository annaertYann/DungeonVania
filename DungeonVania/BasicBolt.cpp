#include "pch.h"
#include "BasicBolt.h"
#include "Spell.h"
#include "utils.h"
#include <math.h>
BasicBolt::BasicBolt(const Point2f& pos, float scale,const Vector2f& destination, const Point2f &PosInCamera, Texture* bolt, Texture* boom)
	:Spell::Spell(pos, scale,destination,PosInCamera)
	,m_pBasicBoltSprite{bolt}
	, m_InitialPos{ pos }
	,m_IsGoingLeft{}
	,m_pBasicBoltBoomSprite{boom }
	, m_IsDead{ false }
	,m_IsExplosion{false}
	,m_SourceRectExplosion{0,0,128,128}
	,m_DeadTime{0}

	
{
	m_Width = 128;
	m_Height = 128;
	m_SourceRect = Rectf{0,0,128,128};
	m_DestRect.width = scale * m_Width;
	m_DestRect.height = m_Height* scale;
	m_MovementSpeed = 100 * scale;


}
BasicBolt::~BasicBolt()
{
	
}
void BasicBolt::Draw()const 
{
	if (m_IsExplosion == false)
	{
		if (m_IsGoingLeft == true)
		{
			glPushMatrix();
			glTranslatef(2 * m_DestRect.left + m_DestRect.width, 0, 0);
			glScalef(-1, 1, 1);
			m_pBasicBoltSprite->Draw(m_DestRect, m_SourceRect);
			glPopMatrix();
		}
		else
		{
			m_pBasicBoltSprite->Draw(m_DestRect, m_SourceRect);
			
		}
	}
	else
	{
		m_pBasicBoltBoomSprite->Draw(m_DestRect, m_SourceRectExplosion);
	}

	
}
void BasicBolt::Update(float elapsedSec)
{
	m_AccuSec += elapsedSec;
	if (m_IsExplosion==false)
	{
		MoveBolt(elapsedSec);
		UpdateSprite(elapsedSec);
	}
	else
		//exloding
	{
		UpdateExplosionSprite(elapsedSec);
	}
	
	
}
void BasicBolt::MoveBolt(float elapsedSec)
{
	float distanceX{ MousePos.x - (m_PosInCamera.x+32) };
	m_Velocity = distanceX * elapsedSec;
	m_Velocity = m_Velocity * elapsedSec;
	m_DestRect.left += m_Velocity * m_MovementSpeed;
	if (m_Velocity > 0)
	{
		m_IsGoingLeft = false;
	}
	else
	{
		m_IsGoingLeft = true;
	}

	float distanceY{ MousePos.y - m_PosInCamera.y };
	m_Velocity = distanceY * elapsedSec;
	m_Velocity = m_Velocity * elapsedSec;
	m_DestRect.bottom += m_Velocity * m_MovementSpeed;
}
Rectf BasicBolt::GetShape()
{
	return m_DestRect;
}
bool BasicBolt::ExplosionTime()
{
	m_IsExplosion = true;
	return m_IsDead;
}
bool BasicBolt::GetExplosion()
{
	return m_IsExplosion;
}
void BasicBolt::UpdateSprite(float elapsedSec)
{
	if (m_AccuSec >= 0.1)
	{
		m_CurrentFrame++;
		m_SourceRect.left = m_CurrentFrame * m_Width;
		m_AccuSec = 0;
	}
	if (m_CurrentFrame * m_Width == m_pBasicBoltSprite->GetWidth())
	{
		m_CurrentFrame = 0;
	}
}
void BasicBolt::UpdateExplosionSprite(float elapsedSec)
{
	if (m_AccuSec >= 0)
	{
		
		m_DeadTime++;
		m_SourceRectExplosion.bottom = m_DeadTime * m_Height;
		m_AccuSec = 0;
	}
	if (m_DeadTime * m_Height >= m_pBasicBoltBoomSprite->GetHeight())
	{
		m_DeadTime = 8;
		m_IsDead = true;
		m_Dead = true;
		m_CurrentFrame = 0;
	}
	
}
