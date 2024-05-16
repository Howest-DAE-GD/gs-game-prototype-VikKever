#include "pch.h"
#include "Game.h"
#include "Player.h"
#include "Hamburger.h"
#include "Salad.h"
#include "Item.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Texture.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }
	, m_pPlayer{ new Player{Point2f{GetViewPort().width / 2, (GetViewPort().height - 40.f) / 2}}}
	, m_HamburgerSpawnTimer{0.f}
	, m_SaladSpawnTimer{0.f}
	//, m_Points{ 0 }
	, m_Health{ 1.f }
	, m_GameArea{0.f, 0.f, GetViewPort().width, GetViewPort().height - 40.f}
	, m_GameOver{ false }
	, m_PlayTime{ -3.f }
	, m_CountdownNr{ 4 }
	, m_pCountdownText{ nullptr }
	, m_pScoreText{ nullptr }
	, m_pGameOverText{ nullptr }
	, m_MousePos{}
	, m_DrawHamburgerTemp{false}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	SpawnItems();
}

void Game::Cleanup( )
{
	delete m_pPlayer;

	for (const Item* pHamburger : m_pHamburgers)
	{
		delete pHamburger;
	}

	for (const Item* pSalad : m_pSalads)
	{
		delete pSalad;
	}

	delete m_pCountdownText;
	if (m_pScoreText != nullptr)
	{
		delete m_pScoreText;
		delete m_pGameOverText;
	}
}

void Game::Update( float elapsedSec )
{
	if (m_GameOver) return;

	m_PlayTime += elapsedSec;

	if (m_PlayTime < 0.f)
	{
		UpdateCountdown();
		return;
	}

	m_HamburgerSpawnTimer -= elapsedSec;
	m_SaladSpawnTimer -= elapsedSec;

	SpawnItems();

	for (const Item* pItem : m_pHamburgers)
	{
		const Hamburger* pHamburger{ static_cast<const Hamburger*>(pItem) };
		m_pPlayer->ApplyForce(pHamburger->GetForce(m_pPlayer->GetPosition()));
	}
	
	m_pPlayer->Update(elapsedSec, m_GameArea, ScaleToDifficulty(100.f, 200.f));

	if (CheckConsumeItems(m_pHamburgers))
	{
		m_Health -= 0.1f;
		m_pPlayer->Stun();
	}
	if (CheckConsumeItems(m_pSalads)) 
	{
		m_Health += 0.15f;
		if (m_Health > 1.f) m_Health = 1.f;
	}

	if (!m_pPlayer->IsStunned()) DepleteHealth(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );

	if (m_DrawHamburgerTemp) Hamburger::Draw(m_MousePos, 0.3f);

	for (const Item* pHamburger : m_pHamburgers)
	{
		pHamburger->Draw();
	}

	for (const Item* pSalad : m_pSalads)
	{
		pSalad->Draw();
	}

	DrawHealthBar();
	
	if (m_PlayTime < 0.f) DrawCenterText(m_pCountdownText, 50);

	if (!m_GameOver) m_pPlayer->Draw();
	else
	{
		DrawCenterText(m_pGameOverText, 50.f);
		DrawCenterText(m_pScoreText, 0.f);
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	if (m_GameOver && e.keysym.sym == SDLK_r) Reset();

	std::cout << "Difficulty: " << GetDifficulty() << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	m_MousePos = Point2f{ float(e.x), float(e.y) };

	for (int idx{}; idx < m_pHamburgers.size(); ++idx)
	{
		if (utils::IsPointInCircle(m_MousePos, m_pHamburgers[idx]->GetHitbox()) && !m_GameOver && m_PlayTime > 0.f)
		{
			m_DrawHamburgerTemp = false;
			static_cast<Hamburger*>(m_pHamburgers[idx])->SetHighlighted(true);
			return;
		}
		else static_cast<Hamburger*>(m_pHamburgers[idx])->SetHighlighted(false);
	}
	if (m_PlayTime > 0.f && !m_GameOver) m_DrawHamburgerTemp = true;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	if (m_GameOver || m_PlayTime < 0.f) return;

	if (e.button == SDL_BUTTON_LEFT)
	{
		Point2f mousePos{ float(e.x), float(e.y) };
		if (!utils::IsPointInRect(mousePos, m_GameArea)) return;

		for (int idx{}; idx < m_pHamburgers.size(); ++idx)
		{
			if (utils::IsPointInCircle(mousePos, m_pHamburgers[idx]->GetHitbox()))
			{
				delete m_pHamburgers[idx];
				m_pHamburgers.erase(m_pHamburgers.begin() + idx);
				return;
			}
		}

		if (!utils::IsPointInCircle(mousePos, m_pPlayer->GetHitbox()))
		{
			m_pHamburgers.push_back(new Hamburger{ mousePos });
			m_DrawHamburgerTemp = false;
		}
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.f, 0.f, 0.f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::Reset()
{
	m_GameOver = false;
	m_PlayTime = -3.f;
	m_CountdownNr = 4;
	m_Health = 1.f;
	m_SaladSpawnTimer = 0.f;
	m_HamburgerSpawnTimer = 0.f;
	m_pPlayer->SetPosition(Point2f{ m_GameArea.width / 2, m_GameArea.height / 2 });

	for (const Item* pHamburger : m_pHamburgers)
	{
		delete pHamburger;
	}
	m_pHamburgers.clear();

	for (const Item* pSalad : m_pSalads)
	{
		delete pSalad;
	}
	m_pSalads.clear();
}

void Game::DrawHealthBar() const
{
	const Rectf UIRect{ 0.f, m_GameArea.height, m_GameArea.width, GetViewPort().height - m_GameArea.height };

	const float margin{ 10.f };
	const Rectf healthBarRect{
		UIRect.left + margin, UIRect.bottom + margin,
		UIRect.width - margin * 2, UIRect.height - margin * 2
	};

	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
	utils::FillRect(UIRect);

	Rectf healthBarFill{ healthBarRect };
	healthBarFill.width = healthBarRect.width * m_Health;

	if (m_pPlayer->IsStunned()) utils::SetColor(Color4f{ 0.7f, 0.f, 0.f, 1.f });
	else utils::SetColor(Color4f{ 0.f, 0.7f, 0.f, 1.f });
	utils::FillRect(healthBarFill);

	utils::SetColor(Color4f{ 0.f, 0.7f, 0.f, 1.f });
	utils::DrawRect(healthBarRect, 1.f);
}

void Game::SpawnItems()
{
	const int saladSpawnRadius{ 400 };
	const float minSaladSpawnTime{ ScaleToDifficulty(4.f, 0.5f) };
	if (m_SaladSpawnTimer <= 0.f && m_pSalads.size() < 5)
	{
		Point2f pos{};
		pos.x = float(rand() % saladSpawnRadius + m_GameArea.width / 2 - saladSpawnRadius / 2);
		pos.y = float(rand() % saladSpawnRadius + m_GameArea.height / 2 - saladSpawnRadius / 2);
		Salad* pSalad{ new Salad{pos} };
		m_pSalads.push_back(pSalad);

		m_SaladSpawnTimer = float(rand() % 30) / 10 + minSaladSpawnTime;
	}

	const int hamburgerSpawnRadius{ 500 };
	const float safeRadius{ 110.f };
	const float minHamburgerSpawnTime{ ScaleToDifficulty(3.5f, 0.4f) };
	if (m_HamburgerSpawnTimer <= 0.f)
	{
		Point2f pos{};
		do
		{
			pos.x = float(rand() % hamburgerSpawnRadius + m_GameArea.width / 2 - hamburgerSpawnRadius / 2);
			pos.y = float(rand() % hamburgerSpawnRadius + m_GameArea.height / 2 - hamburgerSpawnRadius / 2);
			std::cout << "I";
		} while (utils::GetDistance(pos, m_pPlayer->GetPosition()) < safeRadius);

		Hamburger* pHamburger{ new Hamburger{pos} };
		m_pHamburgers.push_back(pHamburger);

		m_HamburgerSpawnTimer = float(rand() % 20) / 10 + minHamburgerSpawnTime;

		std::cout << std::endl;
	}
}

bool Game::CheckConsumeItems(std::vector<Item*>& items)
{
	for (int idx{}; idx < items.size(); ++idx)
	{
		if (utils::IsOverlapping(items[idx]->GetHitbox(), m_pPlayer->GetHitbox()))
		{
			delete items[idx];
			items.erase(items.begin() + idx);
			return true;
		}
	}
	return false;
}

void Game::UpdateCountdown()
{
	if (-floor(m_PlayTime) < m_CountdownNr)
	{
		--m_CountdownNr;
		if (m_pCountdownText != nullptr) delete m_pCountdownText;
		m_pCountdownText = GetText(std::to_string(m_CountdownNr));
	}
}

void Game::DepleteHealth(float elapsedSec)
{
	const float healthDepletionRate{ ScaleToDifficulty(0.f, 0.07f)};
	m_Health -= elapsedSec * healthDepletionRate;
	if (m_Health <= 0)
	{
		m_GameOver = true;
		std::ostringstream scoreText;
		scoreText << "You stayed healthy for " << std::fixed << std::setprecision(1) << m_PlayTime << "s.";
		m_pScoreText = GetText(scoreText.str(), 30);
		m_pGameOverText = GetText("GAME OVER");
	}
}

Texture* Game::GetText(const std::string& text, int size)
{
	return new Texture{ text, "Bond Story.ttf", size, Color4f{1.f, 1.f, 1.f, 1.f} };
}

void Game::DrawCenterText(const Texture* pText, float vertOffset) const
{
	const Point2f drawPos{
		m_GameArea.width / 2 - pText->GetWidth() / 2,
		m_GameArea.height / 2 - pText->GetHeight() / 2 + vertOffset
	};
	pText->Draw(drawPos);
}

float Game::GetDifficulty()
{
	const double difficultyTime{ 90 }; // seconds before the difficulty reaches 0.5
	const double base{pow(0.5, 1/difficultyTime)};
	return float(1.0 - pow(base, m_PlayTime));
}

float Game::ScaleToDifficulty(float valEasy, float valHard)
{
	const float difficulty{ GetDifficulty() };
	return valEasy * (1 - difficulty) + valHard * difficulty;
}
