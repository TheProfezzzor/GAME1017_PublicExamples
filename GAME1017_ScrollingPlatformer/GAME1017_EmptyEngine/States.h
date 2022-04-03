#pragma once
#ifndef _STATES_H_
#define _STATES_H_

#include "TiledLevel.h"
#include "GameObject.h"

class State // This is the abstract base class for all states
{
public:
	virtual void Enter() = 0; // = 0 means pure virtual - must be defined in subclass
	virtual void Update() = 0; 
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume(); 
	virtual ~State() {} // or = default;
	GameObject* GetGo(const std::string& s);
	vector<std::pair<std::string, GameObject*>>::iterator GetIt(const std::string& s);
protected: // Private but inherited
	State() {} // What does this prevent?
	vector<std::pair<std::string, GameObject*>> m_objects;
};

class TitleState : public State
{
public:
	TitleState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

class GameState : public State
{
public:
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();
	static bool m_bgScrollX;
	static bool m_bgScrollY;
private:
	// Background scrolling
	
};

#endif

