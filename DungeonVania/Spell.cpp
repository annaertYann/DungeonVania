#include "pch.h"
#include "Spell.h"
Spell::Spell(const Point2f& pos,float scale,const Vector2f& MousePos, const Point2f &PosInCamera)
	:m_DestRect{pos.x,pos.y,0,0}
	, m_SourceRect{}
	,m_CurrentFrame{}
	, m_Scale{ scale }
	,MousePos{MousePos}
	,m_Velocity{}
	,m_MovementSpeed{}
	,m_PosInCamera{PosInCamera}
	,m_Width{}
	,m_Height{}
	,m_AccuSec{}
	,m_HitBox{}
	,m_CurrentType{}
	, m_Dead{}
{
}

Spell::~Spell()
{
}
Spell::SpellType Spell::GetType()
{
	return m_CurrentType;
}
bool Spell::GetDead()
{
	return m_Dead;
}