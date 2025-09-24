#pragma once

#include "Player.h"
#include <vector>


using namespace std;

class Game
{
private:
	Player p_name;

	int CalculatePoints(vector<int> diceRoll);
	vector<int> DiceRoll(vector<int> roll);
	void Rules();
public:

	void RunGame();	
};