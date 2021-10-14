#include "pch.h"
#include "Laser.h"

Laser::Laser(const Point2f& pos, float scale, const Vector2f& velocity, const Point2f& PosInCamera)
	:Spell::Spell(pos, scale, velocity, PosInCamera)
	,m_LaserSprite{"Resources/Laser.png"}
	,m_LaserParticle{"Resources/LaserParticle.png"}
	, m_Angle{}
	,m_InitialPos{ pos.x + 50 * scale,pos.y + 46 * scale }
	, m_IsGoingLeft{ false }
	,m_DestRectParticle{ pos.x + 45 * scale,pos.y + 43 * scale ,25*scale,25*scale}
	,m_SourceRectParticle{0,0,50,15}
	,m_HitBoxDiff{50*scale}
	,m_LaserGrowthSpeed{2000}
	,m_LaserMoveSpeed{150}
{
	m_SourceRect = Rectf{ 0,0,50,10 };
	m_DestRect = Rectf{ pos.x,pos.y,0,200 * scale };
	m_HitBox = Rectf{ m_DestRect.left,m_DestRect.bottom+m_HitBoxDiff,m_DestRect.width,m_DestRect.height-m_HitBoxDiff*1.5f };
	m_PosInCamera = pos;
	m_MovementSpeed = 0;
	m_Width = 50;
	m_PosInCamera = pos;
	m_CurrentType = SpellType::laser;


}

Laser::~Laser()
{
}
void Laser::Draw()const 
{
	
	if (m_IsGoingLeft == true)
	{
		glPushMatrix();
		glTranslatef(2 * m_DestRect.left + m_DestRect.width, 0, 0);
		glScalef(-1, 1, 1);
		m_LaserSprite.Draw(m_DestRect, m_SourceRect);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(2 *m_DestRectParticle.left + m_DestRectParticle.width, 0, 0);
		glScalef(-1, 1, 1);
		m_LaserParticle.Draw(m_DestRectParticle, m_SourceRectParticle);
		glPopMatrix();
	}
	else
	{
		m_LaserSprite.Draw(m_DestRect, m_SourceRect);
		m_LaserParticle.Draw(m_DestRectParticle, m_SourceRectParticle);
	}

}
void Laser::Update(float elapsedSec)
{
	m_HitBox = Rectf{ m_DestRect.left,m_DestRect.bottom + m_HitBoxDiff,m_DestRect.width,m_DestRect.height - m_HitBoxDiff };
	UpdateSprite(elapsedSec);
	MoveLaser(elapsedSec);
}
Rectf Laser::GetShape() 
{
	return m_HitBox;
}
bool Laser::ExplosionTime()
{
	return false;
}
bool Laser::GetExplosion() 
{
	return false;
}
void Laser::UpdateSprite(float elapsedSec)
{
	m_AccuSec += elapsedSec;
	if (m_AccuSec >= 0.2)
	{
		m_CurrentFrame++;
		m_SourceRect.left = m_CurrentFrame * m_Width;
		m_SourceRectParticle.left = m_CurrentFrame * m_Width;
		m_AccuSec = 0;
	}
	if (m_CurrentFrame * m_Width == m_LaserSprite.GetWidth())
	{
		m_CurrentFrame = 0;
	}




}
void Laser::MoveLaser(float elapsedSec)
{
	if (MousePos.x>m_InitialPos.x)
	{
		m_IsGoingLeft = false;
		m_DestRect.width += m_LaserGrowthSpeed * elapsedSec;
		m_DestRect.left -= m_LaserMoveSpeed * elapsedSec;
	}
	else
	{
		m_IsGoingLeft = true;
		m_DestRect.width += m_LaserGrowthSpeed* elapsedSec;
		m_DestRect.left -= m_LaserGrowthSpeed * elapsedSec;
		m_DestRect.left += m_LaserMoveSpeed * elapsedSec;
	}
	
	
}