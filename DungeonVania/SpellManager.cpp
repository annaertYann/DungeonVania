#include "pch.h"
#include "Spell.h"
#include "SpellManager.h"
#include "BasicBolt.h"
#include "SlamGround.h"
#include "Laser.h"
SpellManager::SpellManager()
	:m_pFireSprite{new Texture {"Resources/Fire.png"}}
	,m_pBoltSprite{new Texture{"Resources/Hero Knight/EnergyBall.png"}}
	,m_pBoltBoomSprite{new Texture{"Resources/Hero Knight/Boom.png"}}
	,m_pBoltBoomGreenSpirte{ new Texture{"Resources/Hero Knight/BoomGreen.png"} }
	,m_pBoltBoomRedSprite{ new Texture{"Resources/Hero Knight/BoomRed.png"} }
	,m_pBoltGreenSprite{ new Texture{"Resources/Hero Knight/EnergyBallGreen.png"} }
	,m_pBoltRedSprite{ new Texture{"Resources/Hero Knight/EnergyBallRed.png"} }
{
	
}
SpellManager::~SpellManager()
{
	for (size_t i{};i<m_pSpells.size();i++)
	{	
		delete m_pSpells[i];
		m_pSpells[i] = nullptr;
	}
	delete m_pFireSprite;
	m_pFireSprite = nullptr;
	delete m_pBoltSprite;
	m_pBoltSprite = nullptr;
	delete m_pBoltBoomSprite;
	m_pBoltBoomSprite = nullptr;
	delete m_pBoltBoomGreenSpirte;
	m_pBoltBoomGreenSpirte = nullptr;
	delete m_pBoltBoomRedSprite;
	m_pBoltBoomRedSprite = nullptr;
	delete m_pBoltRedSprite;
	m_pBoltRedSprite = nullptr;
	delete m_pBoltGreenSprite;
	m_pBoltGreenSprite = nullptr;
}
void SpellManager::Update(float elapsedsec)
{
	for (Spell* element : m_pSpells)
	{
		if (element!=nullptr)
		{
			element->Update(elapsedsec);
		}	
	}
}
void SpellManager::Draw()const
{
	for(Spell* element:m_pSpells)
	{
		if (element!=nullptr)
		{
			element->Draw();
		}
		
	}
}
void SpellManager::AddSpell(SpellManager::SpellType currentSpell, const Point2f& pos, float scale,const Vector2f& destination, const Point2f &PosInCamera)
{
	if (currentSpell == SpellManager::SpellType::basicBolt)
	{
		m_pSpells.push_back(new BasicBolt{pos,scale,destination,PosInCamera,m_pBoltSprite,m_pBoltBoomSprite});
	}
	if (currentSpell == SpellManager::SpellType::groundSlam)
	{
		m_pSpells.push_back(new SlamGround{ pos,scale,destination,PosInCamera,m_pFireSprite });
	}
	if (currentSpell == SpellManager::SpellType::laser)
	{
		m_pSpells.push_back(new Laser{ pos,scale,destination,PosInCamera });
	}
	if (currentSpell == SpellManager::SpellType::green)
	{
		m_pSpells.push_back(new BasicBolt{ pos,scale,destination,PosInCamera,m_pBoltGreenSprite,m_pBoltBoomGreenSpirte });
	}
	if (currentSpell == SpellManager::SpellType::red)
	{
		m_pSpells.push_back(new BasicBolt{ pos,scale,destination,PosInCamera,m_pBoltRedSprite,m_pBoltBoomRedSprite });
	}
}
std::vector<Spell*>& SpellManager::GetSpells()
{
	return m_pSpells;
}