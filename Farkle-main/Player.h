#pragma once

#include <string>

class Player
{

private:
	std::string p_name;
	int p_score = 0;

public:
	void SetName(const std::string& name)
	{
		p_name = name;
	}
	void SetScore(int score)
	{
		p_score = score;
	}

	const std::string GetName()
	{
		return p_name;
	}
	int GetScore()
	{
		return p_score;
	}
};