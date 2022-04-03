#pragma once
#ifndef _TILEDLEVEL_H_
#define _TILEDLEVEL_H_

#include <fstream>
#include <map>
#include <vector>
#include "Engine.h"
#include "GameObject.h"
#include "TextureManager.h"

class Tile : public SpriteObject // Inline.
{
public:
	Tile(SDL_Rect s, SDL_FRect d, bool o = false, bool h = false)
		:SpriteObject(s, d), m_obstacle(o), m_hazard(h) {}
	Tile* Clone() { return new Tile(m_src, m_dst, m_obstacle, m_hazard); }
	void Update() {}
	void Render() {} // Tiles are rendered in TiledLevel.
	bool IsObstacle() { return m_obstacle; }
	void SetXY(float x, float y) { m_dst.x = x; m_dst.y = y; }

private:
	bool m_obstacle, m_hazard;
};

class TiledLevel : public GameObject // Like a manager of Tiles.
{
public:
	TiledLevel(const unsigned short r, const unsigned short c, const int w, const int h,
		const char* tileData, const char* levelData, const char* tileKey);
	~TiledLevel(); // Destructor because Tiles are created within this class dynamically.
	void Update() {}
	void Render();
	vector<Tile*>& GetObstacles() { return m_obstacles; }
	vector< vector<Tile*> >& GetTiles() { return m_level; }
	int GetRows() { return m_rows; }
	int GetCols() { return m_cols; }
	void UpdateTiles(float scroll, bool x = false);

private:
	const char* m_tileKey; // For TextureManager.
	int m_rows, m_cols;
	void GetRowsAndColumns(const char* levelData);
	map<char, Tile*> m_tiles; // 16 prototype Tile objects.
	vector< vector<Tile*> > m_level; // A 2D vector. Outer vector = rows, inner vector = columns.
	vector<Tile*> m_obstacles; // A 1D vector for collision.
};

#endif

