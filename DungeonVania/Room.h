#pragma once
#include <vector>
#include "structs.h"
#include "Texture.h"

class Room
{
	public:
	enum class Border
	{
		top
		,down
		,right
		,left,none
	};
	Room(float scale, const Point2f& position = Point2f{});
	virtual ~Room();
	bool CollisionDetection(const Rectf& actorShape)const;
	virtual void HandleCollision(Rectf& actorShape);
	virtual void Draw()const=0;
	std::vector<Point2f> getvertices();
	float GetBorder(Border destination)const;
	void SetDoCollision(bool doCollision);
protected:
	float m_Scale;
	std::vector <Point2f> m_Vertices;
	Point2f m_Position;
	bool m_DoCollision;

};

