#pragma once
#ifndef _PLATFORMPLAYER_H_
#define _PLATFORMPLAYER_H_

#include "GameObject.h"
#define GRAVITY 4.0
#define JUMPFORCE 50.0

enum PlayerState { STATE_IDLING, STATE_RUNNING, STATE_JUMPING };

class PlatformPlayer : public AnimatedSpriteObject
{
public:
	PlatformPlayer(SDL_Rect s, SDL_FRect d);
	void Update();
	void Render();
	// Getters and setters.
	void Stop();
	void StopX();
	void StopY();
	void SetAccelX(double a);
	void SetAccelY(double a);
	void SetX(float x);
	void SetY(float y);
	void SetGrounded(bool g);
	bool IsGrounded();
	double GetVelX();
	double GetVelY();

private:
	bool m_isGrounded, m_isFacingLeft;
	double m_accelX, m_accelY,
		m_velX, m_velY,
		m_maxVelX, m_maxVelY,
		m_drag, m_grav;
	PlayerState m_state;
};

#endif