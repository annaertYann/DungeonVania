#pragma once
#include "Texture.h"
#include "utils.h"
enum class genders
{
	female, male, nothing
};
class StartScreen
{
public:
	StartScreen(Window window);
	~StartScreen();
	
	void Draw()const;
	bool IsNewGameClicked(const Point2f& mousePos);
	bool IsLoadGameClicked(const Point2f& mousePos);
	genders IsFemaleClicked(const Point2f& mousePos);
	void OverlappingButtons(const Point2f& mousePos);
	void SetNewClicked();
private:
	bool m_NewClicked;
	bool m_IsNewGameSelected;
	bool m_IsLoadGameSelected;
	bool m_yesSelected;
	bool m_NoSelected;
	Rectf m_NoButton;
	Rectf m_YesButton;
	Texture m_Female;
	Texture m_Yes;
	Texture m_NO;
	Texture m_BackgroundImage;
	Texture m_loadgame;
	Texture m_NewButtonFireImage;
	Texture m_NewButtonImage;
	Rectf m_NewGameShape;
	Rectf m_LoadGameShape;
	Window m_Window;
};

