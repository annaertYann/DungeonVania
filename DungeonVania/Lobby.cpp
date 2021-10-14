#include "pch.h"
#include "Lobby.h"
#include "Room.h"
#include "utils.h"
Lobby::Lobby(float scale, const Point2f& pos, int unlockedDoors)
	:Room::Room(scale, pos)
	, m_Lobby0Image{"Resources/Lobby0.png"}
	, m_Lobby1Image{ "Resources/Lobby1.png" }
	,m_Lobby2Image{ "Resources/Lobby2.png" }
	, m_Lobby3Image{ "Resources/Lobby3.png" }
	,m_Door1Shape{ scale*320,scale * 672,scale * 128,scale * 128}
	,m_Door2Shape{ scale * 576,scale * 672,scale * 128,scale * 128 }
	,m_Door3Shape{ scale * 832,scale * 672,scale * 128,scale * 128 }
	,m_Scale{scale}
	, m_UnlockedDoors{ unlockedDoors}
	,m_ShopSprite{"Resources/Interactibles.png"}
	,m_ShopCurrentFrame{0}
	,m_AccuSec{0}
	,m_AccuSecTimingShop{0}
	,m_ShopShape{}
	,m_Save{"Resources/savepoint.png"}
	,m_SavePoint{}
	,m_Throphy{"Resources/throphy.png"}
	,m_Won{false}
{
	//[0]=bottomleft
	//[1]=bottomright
	//[2]=topright
	//[3]=topleft
	m_Vertices.push_back(Point2f{ 0,0 });
	m_Vertices.push_back(Point2f{ m_Lobby0Image.GetWidth() * scale,0 });
	m_Vertices.push_back(Point2f{ m_Lobby0Image.GetWidth() * scale,m_Lobby0Image.GetHeight() * m_Scale });
	m_Vertices.push_back(Point2f{ 0,m_Lobby0Image.GetHeight() * scale });
	m_ShopShape = Rectf{ 540 * m_Scale,300 * m_Scale,200 * m_Scale,200 * m_Scale };
	m_SavePoint = Rectf{ 100 * m_Scale,300 * m_Scale,100 * m_Scale,100 * m_Scale };
}
Lobby::~Lobby()
{

}
void Lobby::Draw()const
{
	
	switch (m_UnlockedDoors)
	{
	case 0:
		m_Lobby0Image.Draw(Rectf{ 0,0,m_Scale * m_Lobby0Image.GetWidth(),m_Scale * m_Lobby0Image.GetHeight() }, Rectf{ 0,0,m_Lobby0Image.GetWidth(),m_Lobby0Image.GetHeight() });
		break;
	case 1:
		m_Lobby1Image.Draw(Rectf{ 0,0,m_Scale * m_Lobby0Image.GetWidth(),m_Scale * m_Lobby0Image.GetHeight() }, Rectf{ 0,0,m_Lobby0Image.GetWidth(),m_Lobby0Image.GetHeight() });
		break;
	case 2:
		m_Lobby2Image.Draw(Rectf{ 0,0,m_Scale * m_Lobby0Image.GetWidth(),m_Scale * m_Lobby0Image.GetHeight() }, Rectf{ 0,0,m_Lobby0Image.GetWidth(),m_Lobby0Image.GetHeight() });
		break;
	case 3:
		m_Lobby3Image.Draw(Rectf{ 0,0,m_Scale * m_Lobby0Image.GetWidth(),m_Scale * m_Lobby0Image.GetHeight() }, Rectf{ 0,0,m_Lobby0Image.GetWidth(),m_Lobby0Image.GetHeight() });
		break;

	default:
		break;
	}
	m_Save.Draw(m_SavePoint, Rectf{0,0,m_Save.GetWidth(),m_Save.GetHeight()});
	//utils::SetColor(Color4f{ 1,0,0,1 });
	//utils::DrawRect(m_Door1);
	//utils::DrawRect(m_Door2);
	//utils::DrawRect(m_Door3);
	m_ShopSprite.Draw(m_ShopShape, Rectf{float (m_ShopCurrentFrame)*64,512,64,64});
	if (m_Won)
	{
		m_Throphy.Draw(Rectf{ m_ShopShape.left*1.1f,m_ShopShape.bottom/2,m_ShopShape.width/2,m_ShopShape.height/2 }, Rectf{ 0,0,m_Throphy.GetWidth(),m_Throphy.GetHeight() });
	}
}
int Lobby::IsOnDoor(const Rectf& actorshape)
{
	if (utils::IsOverlapping(actorshape,m_Door1Shape) && m_UnlockedDoors == 1)
	{
		return 1;
	}

	if (utils::IsOverlapping(actorshape, m_Door2Shape)&&m_UnlockedDoors==2)
	{
		return 2;
	}
	if (utils::IsOverlapping(actorshape, m_Door1Shape) && m_UnlockedDoors == 2)
	{
		return 1;
	}

	if (utils::IsOverlapping(actorshape, m_Door3Shape) && m_UnlockedDoors == 3)
	{
		return 3;
	}
	if (utils::IsOverlapping(actorshape, m_Door2Shape) && m_UnlockedDoors == 3)
	{
		return 2;
	}
	if (utils::IsOverlapping(actorshape, m_Door1Shape) && m_UnlockedDoors == 3)
	{
		return 1;
	}
	return 0;
}
void Lobby::Update(float elapsedSec)
{
	float frameRate{0.1f};
	m_AccuSecTimingShop += elapsedSec;
	if (m_AccuSecTimingShop > 1)
	{
		m_AccuSec += elapsedSec;
		if (m_AccuSec > frameRate)
		{
			m_ShopCurrentFrame++;
			m_AccuSec = 0;
			if (m_ShopCurrentFrame >= 2)
			{
				m_ShopCurrentFrame = 0;
				m_AccuSecTimingShop = 0;
			}
		}
	}
}
bool Lobby::IsInShop(const Rectf& actorShape)
{
	if (utils::IsOverlapping(actorShape, m_ShopShape))
	{
		return true;

	}
	else
	{
		return false;
	}
}
bool Lobby::IsInSavePoint(const Rectf& actorShape)
{
	if (utils::IsOverlapping(actorShape, m_SavePoint))
	{
		return true;

	}
	else
	{
		return false;
	}
}
void Lobby::SetWon()
{
	m_Won = true;
}