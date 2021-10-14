#pragma once
#include <vector>
#include"Vector2f.h"
#include "BasicBolt.h"
#include "SlamGround.h"
class Spell;
class SpellManager
{
public:
	enum class SpellType
	{
		basicBolt
		,groundSlam
		,laser
		,red
		,green
	};
	SpellManager();
	~SpellManager();
	SpellManager(const SpellManager& other) = delete;
	SpellManager& operator=(const SpellManager& rhs) = delete;
	SpellManager(const SpellManager&& other) = delete;
	SpellManager& operator=(const SpellManager&& rhs) = delete;
	void Update(float elapsedsec);
	void Draw()const;
	void AddSpell(SpellType currentSpell, const Point2f& pos, float scale, const Vector2f& destination,const Point2f& PosInCamera);
	std::vector<Spell*>& GetSpells();
	Texture* m_pFireSprite;
	Texture* m_pBoltSprite;
	Texture* m_pBoltRedSprite;
	Texture* m_pBoltGreenSprite;
	Texture* m_pBoltBoomSprite;
	Texture* m_pBoltBoomRedSprite;
	Texture* m_pBoltBoomGreenSpirte;
private:
	std::vector <Spell* >m_pSpells;
};
