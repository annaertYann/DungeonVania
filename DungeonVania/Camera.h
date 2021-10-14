#pragma once
#include "structs.h"
class Camera
{
public:
	Camera(Point2f screenCenter);
	~Camera();


	void MoveUp(Point2f& posincamera,float distance, Rectf actorShape, float topLevelBoundaries);
	void MoveDown(Point2f &posincamera, float distance, Rectf actorShape, float  botLevelBoundaries);
	void MoveRight(Point2f &posincamera, float distance, Rectf actorShape, float  rightLevelBoundaries);
	void MoveLeft(Point2f&posincamera, float distance, Rectf actorShape, float leftLevelBoundaries);
	void SetCameraBorderLock(bool isLocked);
	Point2f GetCameraPos()const;
	void SetCameraPos(Point2f cameraPos);
private:
	const Point2f m_ScreenCenter;
	Point2f m_CameraPos;
	float m_VerticalBorder;
	float m_HorizontalBorder;
	bool m_CameraBorderLock;
};

