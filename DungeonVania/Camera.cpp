#include "pch.h"
#include "Camera.h"
Camera::Camera(Point2f screenCenter)
	:m_ScreenCenter(screenCenter)
	, m_HorizontalBorder(screenCenter.x/2)
	,m_VerticalBorder(screenCenter.y/2)
	,m_CameraBorderLock{false}
	,m_CameraPos{}
{
}

Camera::~Camera()
{
}
void Camera::MoveUp(Point2f &posincamera, float distance,Rectf actorShape, float topLevelBoundaries)
{
	if (posincamera.y >= m_ScreenCenter.y + m_VerticalBorder - actorShape.height)
	{
		if (m_CameraBorderLock)
		{
			if (actorShape.bottom < topLevelBoundaries - ((m_ScreenCenter.y * 2) - posincamera.y))
			{
				glTranslatef(0, -distance, 0);
				posincamera.y -= distance;
				m_CameraPos.y += distance;
			}
		}
		else
		{
			glTranslatef(0, -distance, 0);
			posincamera.y -= distance;
			m_CameraPos.y += distance;
		}
	}

}
void  Camera::MoveDown(Point2f &posincamera, float distance, Rectf actorShape, float botLevelBoundaries)
{
	if (posincamera.y <= m_ScreenCenter.y - m_VerticalBorder)
	{
		if (m_CameraBorderLock)
		{
			if (actorShape.bottom > botLevelBoundaries + posincamera.y)
			{
				glTranslatef(0, +distance, 0);
				posincamera.y += distance;
				m_CameraPos.y -= distance;
			}
		}
		else
		{
			glTranslatef(0, +distance, 0);
			posincamera.y += distance;
			m_CameraPos.y -= distance;
		}
	}
}
void  Camera::MoveRight(Point2f &posincamera, float distance, Rectf actorShape,  float rightLevelBoundaries)
{
	if (posincamera.x >= m_ScreenCenter.x + m_HorizontalBorder-actorShape.width)
	{
		if (m_CameraBorderLock)
		{


			if (actorShape.left < rightLevelBoundaries - ((m_ScreenCenter.x * 2) - posincamera.x))
			{
				glTranslatef(-distance, 0, 0);
				posincamera.x -= distance;
				m_CameraPos.x += distance;
			}
		}
		else
		{
			glTranslatef(-distance, 0, 0);
			posincamera.x -= distance;
			m_CameraPos.x += distance;
		}
	}
}
void  Camera::MoveLeft(Point2f& posincamera, float distance, Rectf actorShape,  float LeftLevelBoundaries)
{
	if (posincamera.x <= m_ScreenCenter.x - m_HorizontalBorder)
	{
		if (m_CameraBorderLock)
		{
			if (actorShape.left > LeftLevelBoundaries + posincamera.x)
			{
				glTranslatef(distance, 0, 0);
				posincamera.x += distance;
				m_CameraPos.x -= distance;
			}
		}
		else
		{
			glTranslatef(distance, 0, 0);
			posincamera.x += distance;
			m_CameraPos.x -= distance;
		}
		

	}
}
void Camera::SetCameraBorderLock(bool IsLocked)
{
	m_CameraBorderLock = IsLocked;
}
Point2f Camera::GetCameraPos()const
{
	return m_CameraPos;
}
void Camera::SetCameraPos(Point2f cameraPos)
{
	m_CameraPos = cameraPos;
}