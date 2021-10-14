#include "pch.h"
#include "Dwarf.h"
Dwarf::Dwarf(float scale, int collomsIdle, const Point2f& spawnLocation)
	:Enemy::Enemy(scale, 8)
	, m_DwarfSprite{ "Resources/Dwarf.png" }
{
	m_Width = 32;
	m_Height = 32;
	m_SourceRect = Rectf{ 0,0,m_Width,m_Height };
	m_HitBox = Rectf{ m_DestRect.left + m_Scale * 40,m_DestRect.bottom,m_DestRect.width - m_Scale * 100,m_DestRect.height };
	m_DestRect.left = spawnLocation.x;
	m_DestRect.bottom = spawnLocation.y;
	m_DestRect.width = 150 * scale;
	m_DestRect.height = 100 * scale;
}
Dwarf::~Dwarf()
{
}
void Dwarf::Draw()const
{
	m_Manager.Draw();
	if (m_IsGoingRight == true)
	{
		m_DwarfSprite.Draw(m_DestRect, m_SourceRect);
	}
	else
	{
		glPushMatrix();
		glTranslatef(2 * m_DestRect.left + m_DestRect.width, 0, 0);
		glScalef(-1, 1, 1);
		m_DwarfSprite.Draw(m_DestRect, m_SourceRect);
		glPopMatrix();
	}
	if (m_Health > 0)
	{
		utils::SetColor(Color4f{ 1,0,0,1 });
		utils::FillRect(Rectf{ m_DestRect.left,m_DestRect.bottom + m_DestRect.height * 1.1f,m_DestRect.width * m_Health,m_DestRect.height / 4 });
	}
}
void  Dwarf::Attack(float elapsedSec, float scale, const Point2f& destination)
{
	m_AttackAccuSec += elapsedSec;
	if (m_AttackAccuSec >= 5)
	{
		m_Jump.Play(0);
		m_AttackAccuSec = 0;
		m_CurrentRow = 3;
		m_Colloms = 7;
		m_SourceRect.left = 0;
		m_AccuSecAnimation = 0;
		m_IsAttacking = true;
		m_HasMovementFreePass = true;
		m_MovementSpeed = 300 * scale;
	}

}