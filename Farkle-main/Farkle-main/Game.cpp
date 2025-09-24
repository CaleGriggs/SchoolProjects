#include "Game.h"
#include <iostream>
#include <time.h>
#include <array>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream> 

using namespace std;


int points, tempPoints = 0;
string choice = "";
vector<int> diceRoll(6);


/*
	Print Rules
*/
void Game::Rules() {
	string line;
	ifstream rules;
	rules.open("rules.txt");		 //opens rules file

	if (rules.is_open()) {
		while (getline(rules, line)) {		// reads each line of rules.txt and outputs it to the console
			std::cout << line << "\n";
		}
		rules.close();
	}
	else {
		std::cout << "Unable to open file";
	}
}

/*
	Roll the dice
	Returns vector<int>
*/
vector<int> Game::DiceRoll(vector<int> roll) {
	for (unsigned int i = 0; i < roll.size(); i++) {
		roll[i] = rand() % 6 + 1;;
		cout << roll[i];
		if (i != roll.size() - 1) {
			std::cout << ", ";
		}
	}
	cout << endl;
	sort(roll.begin(), roll.end(), greater<int>());
	return roll;
}

/*
	Calculate Points
	Returns int
*/
int Game::CalculatePoints(vector<int> diceRoll) {
	int calPoints = 0;
	
	array<int, 6> numOfNums = { 0,0,0,0,0,0 };		//for counting duplicate dice
	int straight = 0;								//for finding dice straights

	for (unsigned int i = 0; i < diceRoll.size(); i++) {
		numOfNums.at(diceRoll.at(i) - 1)++;
	}
	for (int i = 0; i < 6; i++) {
		if (numOfNums[i] >= 1) {
			straight++;
		}
	}

	// Straight 5 dice(1-5) is 500
	if (straight == 5 && numOfNums[5] == 0) {
		cout << "You got a 1-5 straight! That's 500 points!" << endl;
		calPoints += 500;
		if (numOfNums[0] == 2) {
			calPoints += 100;
		}
		return calPoints;
	}
	// Straight 5 dice(2-6) is 750
	if (straight == 5 && numOfNums[0] == 0) {
		cout << "You got a 2-6 straight! That's 750 points!" << endl;
		calPoints += 750;
		if (numOfNums[4] == 2) {
			calPoints += 50;
		}
		return calPoints;
	}
	// Straight 6 dice is 1500
	if (straight == 6) {
		cout << "You got a 6 dice straight! That's 1500 points!" << endl;
		calPoints += 1500;
		return calPoints;
	}

	// 1s are 100
	if (numOfNums[0] % 3 != 0) {
		calPoints = calPoints + (numOfNums[0] * 100);
	}
	// 5s are 50
	if (numOfNums[4] % 3 != 0) {
		calPoints = calPoints + (numOfNums[4] * 50);
	}
	// 3 of a kind: die *= 100;
	for (unsigned int i = 0; i < numOfNums.size(); i++) {
		if (numOfNums[i] >= 3) {
			// 1s get multiplied by 1000 
			if (i == 0) {
				calPoints += (i + 1) * 1000;
			}
			else {
				calPoints += (i + 1) * 100;
			}
			// doubled for 6 of a kind
			if (numOfNums[i] == 6) {
				if (i == 0) {
					calPoints += (i + 1) * 1000;
				}
				else {
					calPoints += (i + 1) * 100;
				}
			}

		}
	}
	return calPoints;
}

/*
	The Actual Game
*/
void Game::RunGame() {
	srand((unsigned int)time(0));				// Seed for random number generation
	bool gameOver = false;
	int numPlayers = 0, winningScore = 10000, endGame = -1;
	unsigned int  turn = 0;

	cout << "=============================================\n============= Welcome to Farkle =============\n=============================================\n";
	cout << "\n\nHow many players?" << endl << endl;
	cin >> numPlayers;

	vector<Player> playersList(numPlayers);		// Vector containing players

	for (int i = 0; i < numPlayers; i++) {		// Each player enters their name 
		string name = "";
		std::cout << "Player " << i + 1 << ", what is your name?" << endl;
		std::cin >> name;
		Player player;
		player.SetName(name);
		playersList.at(i) = player;
	}

	//Custom score option
	cout << "Would you like to change the winning score? Currently playing to " << winningScore << " points. Y/N" << endl;
	cin >> choice;
	if (choice == "Y" || choice == "y" || choice == "1") {
		cout << "Enter the score you want to play to: \nFor a quick game, 4000 is recommended\nFor a medium length game 6000 is recommended" << endl;
		cin >> winningScore;
		while (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Please enter a number, nothing else.";
			cin >> winningScore;
		}
	}
	cout << "\nYou're playing to " << winningScore << "!" << endl;
	std::cout << "You'll need to earn " << floor(winningScore * 0.1f) << " points in a single round to enter the game.";
	/*
	Game Loop
	*/
	while (gameOver != true) {
		diceRoll = { 0,0,0,0,0,0 };
		tempPoints = 0;
		points = 0;
		Player& currentPlayer = playersList.at(turn);				//accesses the info of the player who's turn it is
		string playerName = currentPlayer.GetName();
		std::cout << "\n\n\n" << "It's " << playerName << "'s turn." << endl << endl;

		//Choice Menu
		std::cout << "Type Y to roll the dice." << endl;
		std::cout << "Type R to veiw the rules." << endl;
		std::cout << "Type S to see your points." << endl;
		std::cout << "Type Q to quit the game now." << endl;
		std::cout << "Enter Choice:" << endl;
		std::cin >> choice;
		if (choice == "Y" || choice == "y" || choice == "1") {
			std::cout << "\nHere's your roll: ";
			diceRoll = DiceRoll(diceRoll);							//fresh roll for this turn
			tempPoints += CalculatePoints(diceRoll);				//non-final point count
			std::cout << "Score for this roll: " << tempPoints << endl;
		}
		else if (choice == "Q") {									//Quits the game
			endGame = -2;
			break;
		}
		else if (choice == "R" || choice == "r") {					//prints the rules
			Rules();
			continue;
		}
		else if (choice == "S" || choice == "s") {					//prints the player's score
			cout << endl << playerName << "'s score is: " << currentPlayer.GetScore() << endl;
			continue;
		}
		else {
			cout << "Wrong input, try again." << endl;
			continue;
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		/*
		Reroll loop
		*/
		vector<int> finalDice;				//old sentimental dice that you'd like to keep around for a while														
		while (true) {
			vector<int> newDice;			//freshly rolled dice

			if (tempPoints == 0) { //Rolls no point dice on first roll
				cout << "\nRules are rules, no points means it's the next player's turn!" << endl;
				//cycles to next player's turn
				//turn++;
				points = 0;
				tempPoints = 0;
				/*if (turn >= playersList.size()) {
					turn = 0;
				}*/
				break;
			}

			if (diceRoll.size() == 1 && diceRoll[0] != 5 && diceRoll[0] != 1) {
				cout << "You can't reroll anymore dice." << endl;
				points += tempPoints;
				cout << "You're final score for this turn is: " << points << endl;
				points = 0;
				tempPoints = 0;
				break;
			}
			if (diceRoll.size() == 0) {
				diceRoll = finalDice;
				finalDice.clear();
			}

			//Choice Menu
			cout << "\nWould you like to reroll? Y/N" << endl;
			std::cin >> choice;
			if (choice == "Q") {														//Quits the game
				endGame = -2;
				break;
			}
			else if (choice == "R" || choice == "r") {									//prints the rules
				Rules();
				continue;
			}
			else if (choice == "S" || choice == "s") {									//prints the player's score
				cout << endl << playerName << "'s score is: " << currentPlayer.GetScore() << endl;
				continue;
			}
			if (choice == "Y" || choice == "y" || choice == "1") {						//I want to reroll
			lable:
				string str = "";

				while (true) {
					cin.clear();
					cin.ignore();
					std::cout << "Type the dice that you want to keep." << endl;
					getline(std::cin, str);															//get line of input
					str.erase(remove(str.begin(), str.end(), ' '), str.end());						//remove spaces from input string
					if (str.length() > diceRoll.size()) {											//check if input isn't too many numbers
						cout << "You entered too many numbers, try agian." << endl;
						continue;
					}
					else if (str.length() == 10000) {													//check if input is blank
						cout << "No input." << endl;
						continue;
					}
					else {
						for (char& c : str) {															//for each character in str
							int j = c - '0';															//subtracts ASCII values to reassign to j
							if (find(diceRoll.begin(), diceRoll.end(), j) != diceRoll.end()) {
								vector<int>::iterator itr = find(diceRoll.begin(), diceRoll.end(), j);	//tries to find j in roll
								if (itr != diceRoll.end()) {											//if j is in roll, erase it from roll, push_back to newDice
									diceRoll.erase(itr);
									newDice.push_back(j);
								}
							}
							else {
								cout << j << " Isn't a number you can keep." << endl;
								goto lable;	//Jumps back to label just after "I want to reroll" line
							}

						}

						//Check if dice kept were point dice
						tempPoints = CalculatePoints(newDice);
						if (tempPoints == 0) {
							cout << "You must keep at least one point die." << endl;
							if (!finalDice.empty()) {
								finalDice.pop_back();
							}
							break;
						}
						else {
							finalDice.insert(finalDice.end(), newDice.begin(), newDice.end());		//append new dice to finalDice
							newDice.clear();														//clear newDice
						}

						points += tempPoints;
						std::cout << "\nDice you're keeping: " << endl;
						for (unsigned int i = 0; i < finalDice.size(); i++) {						//display dice being kept
							cout << finalDice[i];
							if (i != finalDice.size() - 1) {
								std::cout << ", ";
							}
						}
						cout << endl;
						break;
					}
				}
				if (finalDice.size() == 6) {
					cout << "\nAll your dice are point dice, that means you get to keep the points you have and roll all 6 dice again." << endl
						<< "Here are your new dice: ";
					diceRoll = finalDice;
					finalDice.clear();
					newDice.clear();
					diceRoll = DiceRoll(diceRoll);
					tempPoints = CalculatePoints(diceRoll);
					std::cout << "Score for this roll: " << tempPoints << endl
						<< "Current total: " << points + tempPoints << endl;
					continue;
				}
				for (unsigned int i = 0; i < diceRoll.size(); i++) {
					newDice.push_back(diceRoll.at(i));
				}
				cout << "Your new dice: " << endl;
				newDice = DiceRoll(newDice);

				tempPoints = CalculatePoints(newDice);
				std::cout << "Score for this roll: " << tempPoints << endl;

				if (tempPoints == 0) {													//if no new point dice were rolled
					std::cout << endl
						<< "Farkle! " << endl
						<< "No new point dice were rolled. No points were earned for this round.\n" << endl;
					points = 0;
					tempPoints = 0;
					break;
				}
				else {
					//points += tempPoints;
					cout << "Your total score is " << points + tempPoints << endl;
					diceRoll = newDice;
				}
			}
			else if (choice == "N" || choice == "n" || choice == "2") {					//I don't want to reroll
				points += tempPoints;
				tempPoints = 0;
				break;
			}
			else {
				std::cout << "Wrong input. Must type 'Y' or 'N'." << endl;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////


		//Begins end game countdown, last turn for all other players
		if (endGame != -1) {
			if (endGame == -2) {
				cout << "Quitting the game" << endl;
				break;
			}
			endGame--;
		}

		//Declaring the victor
		if (endGame == 0) {
			points = 0;
			int winner = 0;
			for (int i = 0; i < numPlayers; i++) {
				if (playersList.at(i).GetScore() >= points) {
					points = playersList.at(i).GetScore();
					winner = i;
				}
			}
			std::cout << "\nCongrats " << playersList.at(winner).GetName() << ", you won with " << points << " points!" << endl;
			gameOver = true;
		}

		//Etering the game
		if (points + currentPlayer.GetScore() < floor(winningScore * 0.1f)) {
			points = 0;
			currentPlayer.SetScore(0);
			std::cout << "You still need to earn " << floor(winningScore * 0.1f) << " points in a single round to enter the game." << endl;
		}

		//Sets player's new score
		currentPlayer.SetScore(points + currentPlayer.GetScore());
		std::cout << "Your total score: " << currentPlayer.GetScore() << endl;

		//Starts the final round for all players
		if (currentPlayer.GetScore() >= winningScore && endGame == -1) {
			endGame = numPlayers;
			std::cout << "Final round! " << playerName << " is about to win!" << endl;
		}

		//cycles to next player's turn
		turn++;
		if (turn >= playersList.size()) {
			turn = 0;
		}
	}
}