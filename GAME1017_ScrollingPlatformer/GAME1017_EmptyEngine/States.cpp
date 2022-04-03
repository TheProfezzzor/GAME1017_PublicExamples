#include "States.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "Engine.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "Primitives.h"
#include "Button3.h"
#include "PlatformPlayer.h"

#include <iostream>
using namespace std;

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

void State::Resume(){}

GameObject* State::GetGo(const std::string& s)
{ // Using 'std' just to show origin.
	auto it = GetIt(s);
	if (it != m_objects.end())
		return it->second;
	else return nullptr;
}

vector<std::pair<std::string, GameObject*>>::iterator State::GetIt(const std::string& s)
{ 
	auto it = std::find_if(m_objects.begin(), m_objects.end(),
		[&](const std::pair<std::string, GameObject*>& element)
		{
			return element.first == s;
		});
	return it;
}

// Begin TitleState
TitleState::TitleState(){}

void TitleState::Enter()
{
	TEMA::Load("Img/button.png", "play");
	m_objects.push_back(pair<string, GameObject*>("play",
		new PlayButton({ 0, 0, 400, 100 }, { 412, 350, 200, 50 }, "play")));
}

void TitleState::Update()
{
	for (auto const& i : m_objects) // For each
	{
		i.second->Update();
		if (STMA::StateChanging()) return;
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 64, 0, 16, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	for (auto const& i : m_objects)
		i.second->Render();
	State::Render();
}

void TitleState::Exit()
{
	TEMA::Unload("play");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
	m_objects.shrink_to_fit();
}
// End TitleState

// Begin GameState
GameState::GameState(){}

void GameState::Enter() // Used for initialization.
{
	TEMA::Load("Img/Tiles.png", "tiles");
	// Load player image and give it a string key of "player"
	m_objects.push_back(pair<string, GameObject*>("level", new TiledLevel(
		24, 32, 32, 32, "Dat/Tiledata.txt", "Dat/Level1.txt", "tiles")));
	m_objects.push_back(pair<string, GameObject*>("player", new PlatformPlayer(
		{ 0,0,0,0 }, {128, 128, 64, 64})));
}

void GameState::Update()
{
	PlatformPlayer* pObj = static_cast<PlatformPlayer*>(GetGo("player"));
	SDL_FRect* pBound = pObj->GetDst();
	TiledLevel* pLevel = static_cast<TiledLevel*>(GetGo("level"));
	vector< vector<Tile*> >& m_level = pLevel->GetTiles();
	// Scroll tiles.
	m_bgScrollX = m_bgScrollY = false;
	if (pObj->GetDst()->x > WIDTH * 0.7f)
	{
		if (pObj->GetVelX() > 0 && m_level[0][pLevel->GetCols() - 1]->GetDst()->x > WIDTH - 32)
		{
			m_bgScrollX = true;
			pLevel->UpdateTiles((float)pObj->GetVelX(), true);
		}
	}
	else if (pObj->GetVelX() < 0 && pObj->GetDst()->x < WIDTH * 0.3f)
	{
		if (m_level[0][0]->GetDst()->x < 0)
		{
			m_bgScrollX = true;
			pLevel->UpdateTiles((float)pObj->GetVelX(), true);
		}
	}
	if (pObj->GetVelY() > 0 && pObj->GetDst()->y > HEIGHT * 0.7f)
	{
		if (m_level[pLevel->GetRows() - 1][0]->GetDst()->y > HEIGHT - 32)
		{
			m_bgScrollY = true;
			pLevel->UpdateTiles((float)pObj->GetVelY());
		}
	}
	else if (pObj->GetVelY() < 0 && pObj->GetDst()->y < HEIGHT * 0.3f)
	{
		if (m_level[0][0]->GetDst()->y < 0)
		{
			m_bgScrollY = true;
			pLevel->UpdateTiles((float)pObj->GetVelY());
		}
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_X))
	{
		STMA::ChangeState(new TitleState());
		return;
	}
	for (auto const& i : m_objects)
	{
		i.second->Update();
		if (STMA::StateChanging()) return;
	}
	
	// Check collision.
	for (unsigned int i = 0; i < pLevel->GetObstacles().size(); i++)
	{
		SDL_FRect* pTile = pLevel->GetObstacles()[i]->GetDst();
		if (COMA::AABBCheck(*pBound, *pTile))
		{
			// If colliding with top side of tile.
			if ((pBound->y + pBound->h) - (float)pObj->GetVelY() <= pTile->y)
			{
				pObj->StopY();
				pObj->SetY(pTile->y - pBound->h);
				pObj->SetGrounded(true);
			}
			// If colliding with bottom side of tile.
			else if (pBound->y - (float)pObj->GetVelY() >= (pTile->y + pTile->h))
			{
				pObj->StopY();
				pObj->SetY(pTile->y + pTile->h);
			}
			// If colliding with left side of tile.
			else if ((pBound->x + pBound->w) - (float)pObj->GetVelX() <= pTile->x)
			{
				pObj->StopX();
				pObj->SetX(pTile->x - pBound->w);
			}
			// If colliding with right side of tile.
			else if (pBound->x - (float)pObj->GetVelX() >= (pTile->x + pTile->w))
			{
				pObj->StopX();
				pObj->SetX(pTile->x + pTile->w);
			}
		}
	}
	
}

void GameState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	for (auto const& i : m_objects)
		i.second->Render();
	if ( dynamic_cast<GameState*>(STMA::GetStates().back()) ) 
		State::Render();
}

void GameState::Exit()
{
	for (auto& i : m_objects)
	{
		delete i.second; 
		i.second = nullptr;
	}
	m_objects.clear();
	m_objects.shrink_to_fit();
}

void GameState::Resume(){}
// End GameState

bool GameState::m_bgScrollX = false;
bool GameState::m_bgScrollY = false;