#pragma once
#include "Texture.h"
#include "Hero.h"
#include "SoundEffect.h"
enum class ButtonClicked
{
	play,quit,nothing,money,sound
};
class Hud
{
public:
	Hud(Window window,int volume);
	~Hud();
	


	void Draw()const;
	void Update(float elapsedSec, const Point2f& posCamera, int Coin, float Health,bool isInOption);
	ButtonClicked InteractWithOptions(const Point2f& mousepos,bool isclicked,bool MoneyOn,Point2f cameraPos);
	bool InteractWithoptionicon(const Point2f mousepos, Point2f cameraPos);
	void InteractWithShop(Hero& hero,const Point2f& mousepos);
	void SetIsShopOpen(bool isOpen);
	void SetCurrentColor(Hero::BoltType boltType);
	void ChangeVolume(int volume);
	void SetSaved();
	void SetSettings(bool newSoundOn, bool newCoinsOn);
private:
	bool m_Saved;
	Texture m_SavedTexture;
	float m_SavedAccuSec;
	Rectf m_OptionIconButton;
	Texture m_OptionIcon;
	SoundEffect m_Buy;
	bool m_MoneyOn;
	bool m_SoundOn;
	bool m_PlaySelected;
	bool m_QuitSelected;
	bool m_OptionsSelected;
	void DrawOptions()const;
	int m_CoinCounterNumber;
	bool m_IsShopOpen;
	bool m_IsInOption;
	Rectf m_PlayButton;
	Rectf m_OptionButton;
	Texture m_BackgroundImage;
	Rectf m_QuitButton;
	Texture m_CoinCounterImage;
	Texture m_OptionButtons;
	Texture m_CoinSprite;
	Texture m_OptionButtonsImage;
	Texture m_HealthBarSprite;
	Texture m_IconsSprite;
	Texture m_PriceNumberImage;
	bool m_OptionClicked;
	Hero::BoltType m_CurrentColor;
	Window m_Window;
	Point2f m_PosCamera;
	Rectf m_HealthDestRect;
	Rectf m_CoinSourceRect;
	float m_Health;
	float m_AccuSecCoin;
	float m_AccuSecCoinAnnimation;
	float m_AccuSec;
	float m_InvincibleScale;
};

