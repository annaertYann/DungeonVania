#include "pch.h"
#include "Pillar.h"
#include "utils.h"
Pillar::Pillar(const Point2f& pos, Texture* ptexture, float scale)
	:m_DestRect{ pos.x,pos.y,88 * scale,160 * scale }
	, m_pPillarImage{ ptexture }
	,m_IsRemoved{}
{
	m_Vertices.push_back(Point2f{pos.x,pos.y});
	m_Vertices.push_back(Point2f{pos.x+m_DestRect.width,pos.y});
	m_Vertices.push_back(Point2f{ pos.x + m_DestRect.width,pos.y+m_DestRect.height/3 });
	m_Vertices.push_back(Point2f{ pos.x,pos.y + m_DestRect.height/3 });
	m_Vertices.push_back(Point2f{ pos.x,pos.y });
}

Pillar::~Pillar()
{
}
void Pillar::Draw()const
{
	if (!m_IsRemoved)
	{
		m_pPillarImage->Draw(m_DestRect, Rectf{ 0,0,m_pPillarImage->GetWidth(),m_pPillarImage->GetHeight() });
	}
}
void Pillar::HandleCollosion(Hero& hero,std::vector<Enemy*> &enemy)
{
	if (!m_IsRemoved)
	{
		Rectf& actorShape{ hero.GetShape() };
		Point2f rayBegin1{ actorShape.left,actorShape.bottom + actorShape.height / 2 };
		Point2f rayEnd1{ actorShape.left + actorShape.width,actorShape.bottom + actorShape.height / 2 };
		utils::HitInfo tempInfo1{};
		if (utils::Raycast(m_Vertices, rayBegin1, rayEnd1, tempInfo1) == true)
		{
			hero.FixPosInCamera();
		}
		Point2f rayBegin{ actorShape.left + actorShape.width / 2,actorShape.bottom + actorShape.height };
		Point2f rayEnd{ actorShape.left + actorShape.width / 2,actorShape.bottom };
		utils::HitInfo tempInfo{};
		if (utils::Raycast(m_Vertices, rayBegin, rayEnd, tempInfo) == true)
		{
			hero.FixPosInCamera();
		}
		for (Spell*& element : hero.GetManager().GetSpells())
		{
			if (element != nullptr)
			{
				if (utils::IsOverlapping(element->GetShape(), Rectf{ m_DestRect.left,m_DestRect.bottom,m_DestRect.width,m_DestRect.height / 3 }))
				{
					if (element->ExplosionTime())
					{
						delete element;
						element = nullptr;
					}
				}
			}
		}
		for (Enemy*& element : enemy)
		{
			if (element != nullptr)
			{
				for (Spell*& element1 : element->GetSpells())
				{
					if (element1 != nullptr)
					{
						if (utils::IsOverlapping(element1->GetShape(), Rectf{ m_DestRect.left,m_DestRect.bottom,m_DestRect.width,m_DestRect.height / 3 }))
						{

							delete element1;
							element1 = nullptr;

						}
					}
				}
				Rectf actorShape{ element->GetShape() };
				Point2f rayBegin1{ actorShape.left,actorShape.bottom + actorShape.height / 2 };
				Point2f rayEnd1{ actorShape.left + actorShape.width,actorShape.bottom + actorShape.height / 2 };
				utils::HitInfo tempInfo1{};
				if (utils::Raycast(m_Vertices, rayBegin1, rayEnd1, tempInfo1) == true)
				{
					element->SetPreviousPos();
				}
				Point2f rayBegin{ actorShape.left + actorShape.width / 2,actorShape.bottom + actorShape.height };
				Point2f rayEnd{ actorShape.left + actorShape.width / 2,actorShape.bottom };
				utils::HitInfo tempInfo{};
				if (utils::Raycast(m_Vertices, rayBegin, rayEnd, tempInfo) == true)
				{
					element->SetPreviousPos();
				}
			}
		}
	}
}
Rectf Pillar::GetShape()
{
	return m_DestRect;
}
void Pillar::SetIsRemoved()
{
	m_IsRemoved = true;
}