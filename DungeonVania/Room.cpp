#include "pch.h"
#include "Room.h"
#include "utils.h"
#include "Minotaur.h"
Room::Room(float scale , const Point2f &position)
	:m_Position{position}
	,m_Scale{scale}
	,m_DoCollision{true}
{
}
Room::~Room()
{
}
void Room::HandleCollision(Rectf &actorShape)
{
	if (m_DoCollision)
	{
		Point2f rayBegin1{ actorShape.left,actorShape.bottom+actorShape.height/2 };
		Point2f rayEnd1{ actorShape.left + actorShape.width,actorShape.bottom+ actorShape.height / 2 };
		utils::HitInfo tempInfo1{};
		if (utils::Raycast(m_Vertices, rayBegin1, rayEnd1, tempInfo1) == true)
		{
			float diffDistance{};
			if (tempInfo1.intersectPoint.x == m_Vertices[1].x)
			{
				diffDistance = actorShape.left;
				actorShape.left = tempInfo1.intersectPoint.x - actorShape.width - 1;
				diffDistance -= actorShape.left;
			}
			else
			{
				diffDistance = actorShape.left;
				actorShape.left = tempInfo1.intersectPoint.x + 1;
				diffDistance -= actorShape.left;
			}


		}


		Point2f rayBegin{ actorShape.left+ actorShape.width / 2,actorShape.bottom + actorShape.height };
		Point2f rayEnd{ actorShape.left+actorShape.width/2,actorShape.bottom };
		utils::HitInfo tempInfo{};
		if (utils::Raycast(m_Vertices, rayBegin, rayEnd, tempInfo) == true)
		{
			float diffDistance{};
			if (tempInfo.intersectPoint.y == m_Vertices[3].y)
			{
				diffDistance = actorShape.bottom;
				actorShape.bottom = tempInfo.intersectPoint.y - actorShape.height - 1;
				diffDistance -= actorShape.bottom;
			}
			else
			{
				diffDistance = actorShape.bottom;
				actorShape.bottom = tempInfo.intersectPoint.y + 1;
				diffDistance -= actorShape.bottom;
			}

		}
	}
	
}
bool Room::CollisionDetection(const Rectf& actorShape)const
{
	Point2f rayBegin{ actorShape.left,actorShape.bottom + actorShape.height };
	Point2f rayEnd{ actorShape.left+actorShape.width,actorShape.bottom };
	utils::HitInfo tempInfo{};
	if (utils::Raycast(m_Vertices, rayBegin, rayEnd, tempInfo) == true)
	{

		return true;
	}
	else
	{
		return false;
	}
}
float Room::GetBorder(Border destination)const
{
	switch (destination)
	{
	case Room::Border::top:
		return m_Vertices[2].y;
		break;
	case Room::Border::down:
		return m_Vertices[0].y;

		break;
	case Room::Border::right:
		return m_Vertices[2].x;
		break;
	case Room::Border::left:
		return m_Vertices[0].x;
		break;
	
	}
	return 0;
}
std::vector<Point2f> Room::getvertices()
{
	return m_Vertices;
}
void Room::SetDoCollision(bool doCollision)
{
	m_DoCollision = doCollision;
}