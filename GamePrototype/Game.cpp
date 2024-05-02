#include "pch.h"
#include "Game.h"
#include "Player.h"
#include "Hamburger.h"
#include "Salad.h"
#include "Item.h"
#include <iostream>

const int Game::m_MAX_LIVES{ 10 };

Game::Game( const Window& window ) 
	:BaseGame{ window }
	, m_pPlayer{ new Player{Point2f{GetViewPort().width / 2, (GetViewPort().height - 40.f) / 2}}}
	, m_HamburgerSpawnTimer{0.f}
	, m_SaladSpawnTimer{0.f}
	//, m_Points{ 0 }
	, m_Lives{ m_MAX_LIVES }
	, m_GameArea{0.f, 0.f, GetViewPort().width, GetViewPort().height - 40.f}
	, m_Playing{ true }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
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
}

void Game::Update( float elapsedSec )
{
	if (!m_Playing) return;

	m_HamburgerSpawnTimer -= elapsedSec;
	m_SaladSpawnTimer -= elapsedSec;

	SpawnItems();

	for (const Item* pItem : m_pHamburgers)
	{
		const Hamburger* pHamburger{ static_cast<const Hamburger*>(pItem) };
		m_pPlayer->ApplyForce(pHamburger->GetForce(m_pPlayer->GetPosition()));
	}
	
	m_pPlayer->Update(elapsedSec, m_GameArea);

	if (CheckConsumeItems(m_pHamburgers))
	{
		m_Lives -= 2;
		if (m_Lives <= 0) m_Playing = false;
	}
	if (CheckConsumeItems(m_pSalads)) 
	{
		++m_Lives;
		if (m_Lives > m_MAX_LIVES) m_Lives = m_MAX_LIVES;
	}
}

void Game::Draw( ) const
{
	ClearBackground( );

	if (m_Playing) m_pPlayer->Draw();

	for (const Item* pHamburger : m_pHamburgers)
	{
		pHamburger->Draw();
	}

	for (const Item* pSalad : m_pSalads)
	{
		pSalad->Draw();
	}

	DrawUI();
	//m_pMap->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{

	if (!m_Playing) return;

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

		m_pHamburgers.push_back(new Hamburger{ mousePos});
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

void Game::DrawUI() const
{
	const Rectf UIRect{ 0.f, m_GameArea.height, m_GameArea.width, GetViewPort().height - m_GameArea.height };

	const float margin{ 30.f };
	const float size{ 15.f };

	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
	utils::FillRect(UIRect);

	const Point2f heartsPos{ margin, UIRect.bottom + UIRect.height / 2 };
	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	for (int heartNr{}; heartNr < m_MAX_LIVES; ++heartNr)
	{
		const Point2f pos{ heartsPos.x + heartNr * margin, heartsPos.y };
		if (heartNr >= m_Lives)
			utils::DrawEllipse(pos.x, pos.y, size / 2, size / 2);
		else
			utils::FillEllipse(pos.x, pos.y, size / 2, size / 2);
	}
}

void Game::SpawnItems()
{
	const int saladSpawnRadius{ 400 };
	if (m_SaladSpawnTimer <= 0.f && m_pSalads.size() < 5)
	{
		Point2f pos{};
		pos.x = float(rand() % saladSpawnRadius + m_GameArea.width / 2 - saladSpawnRadius / 2);
		pos.y = float(rand() % saladSpawnRadius + m_GameArea.height / 2 - saladSpawnRadius / 2);
		Salad* pSalad{ new Salad{pos} };
		m_pSalads.push_back(pSalad);

		m_SaladSpawnTimer = float(rand() % 30) / 10 + 2.f;
	}

	const int hamburgerSpawnRadius{ 500 };
	if (m_HamburgerSpawnTimer <= 0.f)
	{
		Point2f pos{};
		pos.x = float(rand() % hamburgerSpawnRadius + m_GameArea.width / 2 - hamburgerSpawnRadius / 2);
		pos.y = float(rand() % hamburgerSpawnRadius + m_GameArea.height / 2 - hamburgerSpawnRadius / 2);
		Hamburger* pHamburger{ new Hamburger{pos} };
		m_pHamburgers.push_back(pHamburger);

		m_HamburgerSpawnTimer = float(rand() % 20) / 10 + 0.5f;
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
