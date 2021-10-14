#include "pch.h"
#include "Hud.h"
#include "utils.h"
Hud::Hud(Window window,int volume)
	:m_CoinCounterImage{ std::to_string(m_CoinCounterNumber), "Resources/Hud/DIN-Light.otf",50,Color4f{1,0,0,1} }
	, m_CoinSprite{ "Resources/Hud/MonedaD.png" }
	, m_CoinCounterNumber{ 0 }
	, m_Window{ window }
	, m_PosCamera{}
	,m_Health{}
	,m_HealthBarSprite{"Resources/Hud/HealthBar.png"}
	,m_IconsSprite{"Resources/Hud/Icons.png"}
	,m_AccuSec{}
	,m_InvincibleScale{1}
	,m_PriceNumberImage{"Resources/Hud/Price.png"}
	,m_IsShopOpen{false}
	,m_CurrentColor{Hero::BoltType::blue}
	,m_IsInOption{false}
, m_CoinSourceRect{}
, m_AccuSecCoin{}
, m_AccuSecCoinAnnimation{}
,m_HealthDestRect{   }
,m_OptionButtons{"Resources/Hud/buttons.png"}
,m_PlaySelected{}
,m_QuitSelected{}
,m_OptionsSelected{}
,m_BackgroundImage{ "Resources/StartScreen.jpg" }
,m_OptionClicked{false}
,m_OptionButtonsImage{"Resources/Hud/optionsbuttons.png"}
,m_MoneyOn{ true }
,m_SoundOn{true}
,m_OptionIcon{"Resources/Hud/icon.png"}
,m_Buy{"Resources/Sound/Buy.mp3"}
,m_OptionIconButton{}
,m_SavedTexture{"Resources/saved.png"}
,m_SavedAccuSec{}
,m_Saved{}
{
	m_CoinSourceRect = Rectf{ 0,0,m_CoinSprite.GetWidth() / 5,m_CoinSprite.GetHeight() };
	m_HealthDestRect = Rectf{ m_PosCamera.x,m_PosCamera.y + m_Window.height * 9 / 10,m_Window.width / 10,m_Window.height / 20 };
	m_PlayButton =Rectf{ m_PosCamera.x+m_Window.width/2-m_OptionButtons.GetWidth()/2,m_PosCamera.y + m_Window.height * 7 / 10,m_Window.width / 5,m_Window.height / 10 };
	m_OptionButton = Rectf{ m_PosCamera.x + m_Window.width / 2 - m_OptionButtons.GetWidth() / 2,m_PosCamera.y + m_Window.height * 5 / 10,m_Window.width / 5,m_Window.height / 10 };
	m_QuitButton = Rectf{ m_PosCamera.x + m_Window.width / 2 - m_OptionButtons.GetWidth() / 2,m_PosCamera.y + m_Window.height * 3 / 10,m_Window.width / 5,m_Window.height / 10 };
	m_Buy.SetVolume(volume);
	m_OptionIconButton = Rectf{m_PosCamera.x,m_PosCamera.y+ m_Window.height-m_Window.height/10,m_Window.height / 10 ,m_Window.height / 10 };
}

Hud::~Hud()
{
}
void Hud::Draw()const
{
	if (!m_IsInOption)
	{
		//optionicon
		m_OptionIcon.Draw(m_OptionIconButton, Rectf{0,0,m_OptionIcon.GetWidth(),m_OptionIcon.GetHeight()});
		//Coin
		int CoinFrames{ 5 };
		m_CoinSprite.Draw(Rectf{ m_PosCamera.x,m_PosCamera.y + m_Window.height * 8 / 10,m_Window.width / 20,m_Window.height / 20 }, m_CoinSourceRect);
		m_CoinCounterImage.Draw(Rectf{ m_PosCamera.x + m_Window.width / 20,m_PosCamera.y + m_Window.height * 8 / 10,m_Window.width / 20,m_Window.height / 20 });
		//hud
		utils::SetColor(Color4f{ 1,0,0,1 });
		utils::FillRect(Rectf{ m_PosCamera.x + 20,m_PosCamera.y + 5 + m_Window.height * 9 / 10,(m_Health / 100) * (m_Window.width / 10 - 20),m_Window.height / 20 - 10 });
		utils::SetColor(Color4f{ 1,1,1,m_InvincibleScale });
		utils::FillRect(Rectf{ m_PosCamera.x + 20,m_PosCamera.y + 5 + m_Window.height * 9 / 10,(m_Health / 100) * (m_Window.width / 10 - 20),m_Window.height / 20 - 10 });

		m_HealthBarSprite.Draw(m_HealthDestRect, Rectf{ 0,0,m_HealthBarSprite.GetWidth(),m_HealthBarSprite.GetHeight() });

		Rectf iconPos{ m_PosCamera.x + 2 * m_Window.width / 20,m_PosCamera.y + m_Window.height * 8 / 10,m_Window.width / 20,m_Window.height / 20 };
		if (m_CurrentColor == Hero::BoltType::blue)
		{
			m_IconsSprite.Draw(iconPos, Rectf{ 0,0,32,32 });
		}

		if (m_CurrentColor == Hero::BoltType::red)
		{

			m_IconsSprite.Draw(iconPos, Rectf{ 64,0,32,32 });
		}
		if (m_CurrentColor == Hero::BoltType::green)
		{
			m_IconsSprite.Draw(iconPos, Rectf{ 32,0,32,32 });
		}


		if (m_IsShopOpen)
		{

			m_IconsSprite.Draw(Rectf{ (m_Window.width / 2) - 2 * m_Window.width / 20,m_Window.height / 2 + m_Window.height / 20,m_Window.width / 20,m_Window.height / 20 }, Rectf{ 0,0,32,32 });
			m_IconsSprite.Draw(Rectf{ m_Window.width / 2,m_Window.height / 2 + m_Window.height / 20,m_Window.width / 20,m_Window.height / 20 }, Rectf{ 32,0,32,32 });
			m_IconsSprite.Draw(Rectf{ (m_Window.width / 2) + 2 * m_Window.width / 20,m_Window.height / 2 + m_Window.height / 20,m_Window.width / 20,m_Window.height / 20 }, Rectf{ 64,0,32,32 });



			if (m_CurrentColor == Hero::BoltType::blue)
			{
				utils::SetColor(Color4f{ 0,0,0,0.6f });
				utils::FillRect(Rectf{ (m_Window.width / 2) - 2 * m_Window.width / 20,m_Window.height / 2 + m_Window.height / 20,m_Window.width / 20,m_Window.height / 20 });
			}
			else
			{
				m_PriceNumberImage.Draw(Rectf{ (m_Window.width / 2) - 2 * m_Window.width / 20,m_Window.height / 2 + m_Window.height / 10,m_Window.width / 20,m_Window.height / 20 }, Rectf{ 0,0,64,64 });
			}



			if (m_CurrentColor == Hero::BoltType::red)
			{

				utils::SetColor(Color4f{ 0,0,0,0.6f });
				utils::FillRect(Rectf{ (m_Window.width / 2) + 2 * m_Window.width / 20,m_Window.height / 2 + m_Window.height / 20,m_Window.width / 20,m_Window.height / 20 });

			}
			else
			{
				m_PriceNumberImage.Draw(Rectf{ (m_Window.width / 2) + 2 * m_Window.width / 20,m_Window.height / 2 + m_Window.height / 10,m_Window.width / 20,m_Window.height / 20 }, Rectf{ 0,0,64,64 });
			}
			if (m_CurrentColor == Hero::BoltType::green)
			{

				utils::SetColor(Color4f{ 0,0,0,0.6f });
				utils::FillRect(Rectf{ m_Window.width / 2,m_Window.height / 2 + m_Window.height / 20,m_Window.width / 20,m_Window.height / 20 });
			}
			else
			{


				m_PriceNumberImage.Draw(Rectf{ (m_Window.width / 2) ,m_Window.height / 2 + m_Window.height / 10,m_Window.width / 20,m_Window.height / 20 }, Rectf{ 0,0,64,64 });
			}
		}
	}
	else
	{
		DrawOptions();
	}

	if (m_Saved)
	{
		m_SavedTexture.Draw(Rectf{ m_Window.width / 2, m_Window.height / 2, m_Window.width / 20, m_Window.height / 20 }, Rectf{0,0,m_SavedTexture.GetWidth(),m_SavedTexture.GetHeight()});
	}

}
void Hud::DrawOptions()const
{
	//background
	utils::SetColor(Color4f{ 0,0,0,0.9f });
	float distance{m_Window.width/20};
	m_BackgroundImage.Draw(Rectf{ m_PosCamera.x, m_PosCamera.y, m_Window.width, m_Window.height }, Rectf{0,0,m_BackgroundImage.GetWidth(),m_BackgroundImage.GetHeight()});
	utils::FillRect(m_PosCamera.x + distance * 6, m_PosCamera.y + distance, m_Window.width - distance * 12, m_Window.height - distance * 2);
	utils::SetColor(Color4f{ 1,1,1,1 });
	utils::DrawRect(m_PosCamera.x+distance*6, m_PosCamera.y+distance, m_Window.width- distance*12, m_Window.height- distance*2);

	//coins
	m_CoinSprite.Draw(Rectf{ m_PosCamera.x+ m_Window.width / 20,m_PosCamera.y + m_Window.height * 8 / 10,m_Window.width / 20,m_Window.height / 20 }, m_CoinSourceRect);
	m_CoinSprite.Draw(Rectf{ m_PosCamera.x+m_Window.width-2*m_Window.width/20,m_PosCamera.y + m_Window.height * 8 / 10,m_Window.width / 20,m_Window.height / 20 }, m_CoinSourceRect);

	if (!m_OptionClicked)
	{


		//buttons1
		m_OptionButtons.Draw(m_QuitButton, Rectf{ 0,0,m_OptionButtons.GetWidth(),m_OptionButtons.GetHeight() / 3 });
		m_OptionButtons.Draw(m_OptionButton, Rectf{ 0,m_OptionButtons.GetHeight() / 3 * 2,m_OptionButtons.GetWidth(),m_OptionButtons.GetHeight() / 3 });
		m_OptionButtons.Draw(m_PlayButton, Rectf{ 0,m_OptionButtons.GetHeight() / 3,m_OptionButtons.GetWidth(),m_OptionButtons.GetHeight() / 3 });
	}

	//options
	else
	{
		m_OptionButtonsImage.Draw(m_QuitButton, Rectf{ 0,0,m_OptionButtons.GetWidth(),m_OptionButtons.GetHeight() / 3 });

	
		if (m_MoneyOn) 
		{
			utils::SetColor(Color4f{ 0,1,0,1 });
		}
		else
		{
			utils::SetColor(Color4f{ 1,0,0,1 });
		}
		utils::FillRect(Rectf{ m_QuitButton.left + m_QuitButton.width,m_QuitButton.bottom,m_QuitButton.width / 4,m_QuitButton.height });
		utils::SetColor(Color4f{ 1,1,1,1 });
		utils::DrawRect(Rectf{ m_QuitButton.left + m_QuitButton.width,m_QuitButton.bottom,m_QuitButton.width / 4,m_QuitButton.height });




		m_OptionButtonsImage.Draw(m_OptionButton, Rectf{ 0,m_OptionButtons.GetHeight() / 3 ,m_OptionButtons.GetWidth(),m_OptionButtons.GetHeight() / 3 });

		if (m_SoundOn)
		{
			utils::SetColor(Color4f{ 0,1,0,1 });
		}
		else
		{
			utils::SetColor(Color4f{ 1,0,0,1 });
		}
		utils::FillRect(Rectf{ m_QuitButton.left + m_QuitButton.width,m_OptionButton.bottom,m_QuitButton.width / 4,m_QuitButton.height });
		utils::SetColor(Color4f{ 1,1,1,1 });
		utils::DrawRect(Rectf{ m_QuitButton.left + m_QuitButton.width,m_OptionButton.bottom,m_QuitButton.width / 4,m_QuitButton.height });





		m_OptionButtonsImage.Draw(m_PlayButton, Rectf{ 0,m_OptionButtons.GetHeight() / 3*2,m_OptionButtons.GetWidth(),m_OptionButtons.GetHeight() / 3 });
	}
	
	//selection
	utils::SetColor(Color4f{ 1,0,0,1 });
	if (m_PlaySelected)
	{
		utils::DrawRect(m_PlayButton);

	}
	if (m_QuitSelected)
	{
		utils::DrawRect(m_QuitButton);
		

	}
	if (m_OptionsSelected)
	{
		utils::DrawRect(m_OptionButton);
	}

}
void Hud::Update(float elapsedSec, const Point2f& posCamera, int Coin, float Health,bool isInOption)
{
	if (m_Saved)
	{
		m_SavedAccuSec += elapsedSec;
		if (m_SavedAccuSec >= 1.5f)
		{
			m_SavedAccuSec = 0;
			m_Saved = false;
		}
	}
	m_PlayButton = Rectf{ m_PosCamera.x + m_Window.width / 2 - m_OptionButtons.GetWidth() / 4,m_PosCamera.y + m_Window.height * 7 / 10,m_Window.width / 5,m_Window.height / 10 };
	m_OptionButton = Rectf{ m_PosCamera.x + m_Window.width / 2 - m_OptionButtons.GetWidth() / 4,m_PosCamera.y + m_Window.height * 5 / 10,m_Window.width / 5,m_Window.height / 10 };
	m_QuitButton = Rectf{ m_PosCamera.x + m_Window.width / 2 - m_OptionButtons.GetWidth() / 4,m_PosCamera.y + m_Window.height * 3 / 10,m_Window.width / 5,m_Window.height / 10 };
	m_IsInOption = isInOption;
	int CoinFrames{ 5 };
	float frameRate{ 0.1f };
	m_PosCamera = posCamera;
	m_HealthDestRect = Rectf{ m_PosCamera.x,m_PosCamera.y + m_Window.height * 9 / 10,m_Window.width / 10,m_Window.height / 20 };
	m_OptionIconButton = Rectf{ m_PosCamera.x+m_Window.width-m_Window.width/20,m_PosCamera.y + m_Window.height - m_Window.height / 20,m_Window.height / 20 ,m_Window.height / 20 };
	m_AccuSecCoin += elapsedSec;
	int frameratecoin{5};
	if (m_IsInOption)
	{
		frameratecoin = 0;
	}
	if (m_AccuSecCoin >= frameratecoin)
	{

		m_AccuSecCoinAnnimation += elapsedSec;
		if (m_AccuSecCoinAnnimation > frameRate)
		{
			m_AccuSecCoinAnnimation = 0;
			m_CoinSourceRect.left += m_CoinSprite.GetWidth() / CoinFrames;
			if (m_CoinSourceRect.left >= (m_CoinSprite.GetWidth() / CoinFrames) * 5)
			{
				m_AccuSecCoin = 0;
				m_CoinSourceRect.left = 0;
			}
		}
		
	}
	
	
	if (m_CoinCounterNumber != Coin)
	{
		m_CoinCounterNumber = Coin;
		m_CoinCounterImage = Texture{ std::to_string(m_CoinCounterNumber), "Resources/Hud/DIN-Light.otf",100,Color4f{0,0,0,1} };
	}
	if (m_Health != Health)
	{
		m_Health = Health;
		m_InvincibleScale = 1;
	}
	if (m_InvincibleScale > 0)
	{
		m_AccuSec += elapsedSec;
		if (m_AccuSec >= frameRate)
		{
			m_InvincibleScale -= frameRate;
			m_AccuSec = 0;
		}
	}

}
bool Hud::InteractWithoptionicon(const Point2f mousepos, Point2f cameraPos)
{
	if (utils::IsPointInRect(Point2f{ mousepos.x + cameraPos.x,mousepos.y + cameraPos.y }, m_OptionIconButton))
	{
		if (m_IsInOption)
		{
			m_IsInOption = false;
			return false;
		}
		else
		{
			m_IsInOption = true;
			return true;
		}
		
	}
	return false;
}
void Hud::InteractWithShop(Hero& hero,const Point2f& mousePos) 
{
	
	if (m_IsShopOpen)
	{
		if (utils::IsPointInRect(mousePos,Rectf{ (m_Window.width / 2) - 2 * m_Window.width / 20,m_Window.height / 2 + m_Window.height / 20,m_Window.width / 20,m_Window.height / 20 }))
		{
			hero.SkipShootingOnce();
			if (hero.GetCoins() >= 2)
			{
				m_Buy.Play(0);
				hero.SetCurrentType(Hero::BoltType::blue);
				hero.SetCoins(hero.GetCoins() - 2);
			}
			
		}
		if (utils::IsPointInRect(mousePos, Rectf{ m_Window.width / 2,m_Window.height / 2 + m_Window.height / 20,m_Window.width / 20,m_Window.height / 20 }))
		{
			hero.SkipShootingOnce();
			if (hero.GetCoins() >= 2)
			{
				m_Buy.Play(0);
				hero.SetCurrentType(Hero::BoltType::green);
				hero.SetCoins(hero.GetCoins() - 2);
			}
		}
		if (utils::IsPointInRect(mousePos, Rectf{ (m_Window.width / 2) + 2 * m_Window.width / 20,m_Window.height / 2 + m_Window.height / 20,m_Window.width / 20,m_Window.height / 20 }))
		{
			hero.SkipShootingOnce();
			if (hero.GetCoins() >= 2)
			{
				m_Buy.Play(0);
				hero.SetCurrentType(Hero::BoltType::red);
				hero.SetCoins(hero.GetCoins() - 2);
			}
		}
	}
}
ButtonClicked Hud::InteractWithOptions(const Point2f& mousepos, bool isclicked,bool moneyOn,Point2f cameraPos)
{
	m_PlaySelected = false;
	m_QuitSelected = false;
	m_OptionsSelected = false;
	
	if (utils::IsPointInRect(Point2f{ mousepos.x+cameraPos.x,mousepos.y+cameraPos.y}, m_PlayButton))
		{
			m_PlaySelected = true;
		
			if (isclicked)
			{
				if (!m_OptionClicked)
				{
					m_IsInOption = false;
					return ButtonClicked::play;
				}
				else
				{
					m_OptionClicked = false;
				}
			}
		}
		else
			if (utils::IsPointInRect(Point2f{ mousepos.x + cameraPos.x,mousepos.y + cameraPos.y }, m_QuitButton))
			{
				m_QuitSelected = true;
				if (!m_OptionClicked)
				{
					if (isclicked)
					{
						return ButtonClicked::quit;
					}
				}
				else
				{
					if (isclicked)
					{
						m_MoneyOn = moneyOn;
						return ButtonClicked::money;
					}
				}
			}
			else
				if (utils::IsPointInRect(Point2f{ mousepos.x + cameraPos.x,mousepos.y + cameraPos.y }, m_OptionButton))
				{
					m_OptionsSelected = true;
					if (!m_OptionClicked)
					{
						if (isclicked)
						{
							m_OptionClicked = true;
							
						}
					}
					else
					{
						if (isclicked)
						{
							if (m_SoundOn)
							{
								m_SoundOn = false;
							}
							else
							{
								m_SoundOn = true;
							}
							return ButtonClicked::sound;
						}
					}
				}
	
	return ButtonClicked::nothing;

}
void Hud::SetIsShopOpen(bool x)
{
	m_IsShopOpen = x;
}
void Hud::SetCurrentColor(Hero::BoltType x)
{
	m_CurrentColor = x;
}
void Hud::ChangeVolume(int volume)
{
	m_Buy.SetVolume(volume);
}
void Hud::SetSaved()
{
	m_Saved = true;
}
void Hud::SetSettings(bool newSoundOn,bool newCoinsOn)
{
	m_SoundOn = newSoundOn;
	m_MoneyOn = newCoinsOn;
}