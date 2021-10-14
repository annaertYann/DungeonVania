#include "pch.h"
#include "StartScreen.h"
StartScreen::StartScreen(Window window)
	:m_BackgroundImage{ "Resources/StartScreen.jpg" }
	, m_NewButtonImage{ "Resources/NewGame.png" }
	, m_Window{ window }
	, m_NewGameShape{ (window.width / 2) - window.width / 16,(window.height / 2) + window.height / 8,window.width / 8,window.height / 8 }
	, m_LoadGameShape{ (window.width / 2) - window.width / 16,(window.height / 2) ,window.width / 8,window.height / 16 }
	,m_IsLoadGameSelected{}
	,m_IsNewGameSelected{}
	,m_loadgame{"Resources/Hud/loadgame.png"}
	,m_NewButtonFireImage{"Resources/NewGameFire.png"}
	, m_NewClicked{}
	, m_Female{ "Gender?", "Resources/Hud/DIN-Light.otf",50,Color4f{1,0,0,1} }
	,m_Yes{ "Female", "Resources/Hud/DIN-Light.otf",50,Color4f{1,0,0,1} }
	,m_NO{"Male", "Resources/Hud/DIN-Light.otf",50,Color4f{1,0,0,1} }
	, m_yesSelected{}
	, m_NoSelected{}
{
	m_YesButton = Rectf{ m_NewGameShape.left - m_Window.width / 10,m_NewGameShape.bottom - m_Window.height / 10,m_NewGameShape.width / 2,m_NewGameShape.height };

	m_NoButton = Rectf{ m_NewGameShape.left + 2 * m_Window.width / 10,m_NewGameShape.bottom - m_Window.height / 10,m_NewGameShape.width / 2,m_NewGameShape.height };
}

StartScreen::~StartScreen()
{
	
}
void StartScreen::Draw()const
{
	m_BackgroundImage.Draw(Rectf{ 0,0,m_Window.width,m_Window.height }, Rectf{ 0,0,m_BackgroundImage.GetWidth(),m_BackgroundImage.GetHeight() });
	
	if (m_NewClicked)
	{
		m_Female.Draw(m_NewGameShape);
		utils::SetColor(Color4f{1,1,1,1});
		if (m_yesSelected)
		{
			utils::SetColor(Color4f{0,1,0,1 });
		}

		utils::FillRect(Rectf{ m_NewGameShape.left - m_Window.width / 10,m_NewGameShape.bottom - m_Window.height / 10,m_NewGameShape.width / 2,m_NewGameShape.height });

		utils::SetColor(Color4f{ 1,1,1,1 });
		if (m_NoSelected)
		{
			utils::SetColor(Color4f{ 0,1,0,1 });
		}
		utils::FillRect(Rectf{ m_NewGameShape.left + 2 * m_Window.width / 10,m_NewGameShape.bottom - m_Window.height / 10,m_NewGameShape.width / 2,m_NewGameShape.height });


		utils::SetColor(Color4f{ 0,0,0,1 });
		utils::DrawRect(Rectf{ m_NewGameShape.left - m_Window.width / 10,m_NewGameShape.bottom - m_Window.height / 10,m_NewGameShape.width / 2,m_NewGameShape.height });
		utils::DrawRect(Rectf{ m_NewGameShape.left + 2 * m_Window.width / 10,m_NewGameShape.bottom - m_Window.height / 10,m_NewGameShape.width / 2,m_NewGameShape.height });
		m_Yes.Draw(Rectf{m_NewGameShape.left-m_Window.width/10,m_NewGameShape.bottom-m_Window.height/10,m_NewGameShape.width/2,m_NewGameShape.height});
		m_NO.Draw(Rectf{ m_NewGameShape.left + 2*m_Window.width / 10,m_NewGameShape.bottom - m_Window.height / 10,m_NewGameShape.width / 2,m_NewGameShape.height });
	}
	else
	{
		if (m_IsNewGameSelected)
		{
			m_NewButtonFireImage.Draw(m_NewGameShape, Rectf{ 0,0,m_NewButtonFireImage.GetWidth(),m_NewButtonFireImage.GetHeight() });
		}
		else
		{
			m_NewButtonImage.Draw(m_NewGameShape, Rectf{ 0,0,m_NewButtonImage.GetWidth(),m_NewButtonImage.GetHeight() });
		}
		m_loadgame.Draw(m_LoadGameShape, Rectf{ 0,0,m_loadgame.GetWidth(),m_loadgame.GetHeight() });
		if (m_IsLoadGameSelected)
		{
			utils::DrawRect(m_LoadGameShape);
		}
	}


}
void StartScreen::SetNewClicked()
{
	m_NewClicked = true;
}
genders StartScreen::IsFemaleClicked(const Point2f& mousePos)
{
	if (m_NewClicked)
	{
		if (utils::IsPointInRect(mousePos, m_YesButton))
		{
			return genders::female;
		}
		else	if (utils::IsPointInRect(mousePos, m_NoButton))

		{
			return genders::male;
		}
		else
		{
			return genders::nothing;
		}
	}
	else
	{
		return genders::nothing;
	}
}
bool StartScreen::IsNewGameClicked(const Point2f& mousePos)
{
	if (!m_NewClicked)
	{


		if (utils::IsPointInRect(mousePos, m_NewGameShape))
		{
			m_NewClicked = true;
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}
bool StartScreen::IsLoadGameClicked(const Point2f& mousePos)
{
	if (utils::IsPointInRect(mousePos, m_LoadGameShape))
	{
		return true;
	}
	else
	{
		return false;
	}
}
void StartScreen::OverlappingButtons(const Point2f &mousePos)
{
	if (utils::IsPointInRect(mousePos, m_NewGameShape))
	{
		m_IsNewGameSelected = true;
	}
	else
	{
		m_IsNewGameSelected = false;
	}

	if (utils::IsPointInRect(mousePos, m_LoadGameShape))
	{
		m_IsLoadGameSelected = true;
	}
	else
	{
		m_IsLoadGameSelected = false;
	}


	if (m_NewClicked)
	{
		if (utils::IsPointInRect(mousePos, m_YesButton))
		{
			m_yesSelected = true;
		}
		else
		{
			m_yesSelected = false;
		}
		if (utils::IsPointInRect(mousePos, m_NoButton))
		{
			m_NoSelected = true;
		}
		else
		{
			m_NoSelected = false;
		}
	}
	




}