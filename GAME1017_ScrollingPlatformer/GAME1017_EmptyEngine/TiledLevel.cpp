#include "TiledLevel.h"
#include <sstream>

TiledLevel::TiledLevel(const unsigned short r, const unsigned short c, const int w, const int h, const char* tileData, const char* levelData, const char* tileKey) : m_tileKey(tileKey)
{
	ifstream inFile(tileData); // Create the 16 prototype Tiles.
	if (inFile.is_open())
	{
		char key;
		int x, y;
		bool obs, haz;
		while (!inFile.eof())
		{
			inFile >> key >> x >> y >> obs >> haz;
			m_tiles.emplace(key, new Tile({x * w, y * h, w, h}, {0.0f, 0.0f, (float)w, (float)h}, obs, haz));
		}
	}
	inFile.close();
	GetRowsAndColumns(levelData);
	inFile.open(levelData); // Create the whole tiled level now from text file.
	if (inFile.is_open())
	{
		char key;
		m_level.resize(m_rows); // Sets size of the first dimension. We can only use [] after a size is set.
		for (unsigned short row = 0; row < m_rows; row++)
		{
			m_level[row].resize(m_cols);
			for (unsigned short col = 0; col < m_cols; col++)
			{
				inFile >> key;
				m_level[row][col] = m_tiles[key]->Clone(); // Common prototype method.
				m_level[row][col]->SetXY((float)(col * w), (float)(row * h));
				if (m_level[row][col]->IsObstacle())
					m_obstacles.push_back(m_level[row][col]);
			}
		}
	}
	inFile.close();
}

TiledLevel::~TiledLevel()
{
	// Clear the tiled m_level of clones. This also would account for those in m_obstacles.
	for (unsigned short row = 0; row < m_rows; row++)
	{
		for (unsigned short col = 0; col < m_cols; col++)
		{
			delete m_level[row][col];
			m_level[row][col] = nullptr;
		}
	}
	m_level.clear();
	m_obstacles.clear();
	// Clear the original tile prototypes.
	for (map<char, Tile*>::iterator i = m_tiles.begin(); i != m_tiles.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	m_tiles.clear();
}

void TiledLevel::Render()
{
	for (unsigned short row = 0; row < m_rows; row++)
	{
		for (unsigned short col = 0; col < m_cols; col++)
		{
			SDL_RenderCopyF(Engine::Instance().GetRenderer(), TEMA::GetTexture(m_tileKey),
				m_level[row][col]->GetSrc(), m_level[row][col]->GetDst());
		}
	}
}

void TiledLevel::UpdateTiles(float scroll, bool x)
{
	for (int row = 0; row < m_rows; row++)
	{
		for (int col = 0; col < m_cols; col++)
		{
			if (x)
				m_level[row][col]->GetDst()->x -= scroll;
			else
				m_level[row][col]->GetDst()->y -= scroll;
		}
	}
}

void TiledLevel::GetRowsAndColumns(const char* levelData)
{
	ifstream inFile(levelData); // Create the 16 prototype Tiles.
	std::string line;
	std::stringstream s;
	m_rows = m_cols = 0;
	char value;
	while (!inFile.eof())
	{
		m_rows++;
		getline(inFile, line); 
		s << line; 
		while (s >> value) m_cols++;  //while there's something in the line, increase the number of columns
	}
	inFile.close();
}
