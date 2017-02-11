// DnDTextRun.cpp : PROGRAMMED BY PHILIP MICHAEL
// This simulates DnD in a very simple text-based game.
// Your goal is to find the almighty Holy Grail which lies at the end of the dungeon.
// You will encounter enemies at random, and must fight them! (Encounters randomly based on 50/50 odds.)
// After each battle, your HP (hit points) will be fully restored. 
// Attack values are randomly decided (6-sided dice rolls) and enemy HPs are anywhere between 10 to 50 with the boss having 50 to 100HP. 
// Each value of a die roll will damage the enemy by that exact amount. The player has a maximum of 100 HP.
// The boss can deal 1-10 damage at random! Beware!

#include "stdafx.h"
#include <iostream>
#include <string>
#include <random>
#include <ctime>

using namespace std;


void populateEnemies(bool (&dungeon)[100], default_random_engine randGen) {
	//POPULATES THE DUNGEON WITH ENEMIES
	//True = enemy there, false = enemy not there.
	//By default, the last cell (99) has the boss.

	uniform_int_distribution<int> randVal (0, 1); //0 or 1 to determine false/true respectively.
	int hasEnemy = 0; //0 because by default we have no enemy

	for (int i = 0; i < sizeof(dungeon); i++) {
		//LOOP TO POPULATE
		hasEnemy = randVal(randGen);
		if ( i == 99 ){
			dungeon[i] = true;
		}
		else if (hasEnemy == 1) {
			dungeon[i] = true;
		}
		else {
			dungeon[i] = false;
		}
	}
	return; //Exit
}

bool battleHandler(int &playerHP, string playerName, bool isBoss) {
	// Handles a battle. We pass the  player and enemy HPs, and their respective rolls.
	//Reset HP at end if player won.

	//VAR FOR ENEMY HP
	int enemyHP, enemyRoll, playerRoll;

	//RAND VARS
	default_random_engine randomGenerator(time(NULL));
	uniform_int_distribution<int> dieRoll(1, 6); //PLAYER ROLL DAMAGE
	uniform_int_distribution<int> enDMG(1, 6); //ENEMY DAMAGE
	uniform_int_distribution<int> bossDMG(1, 10); //BOSS DAMAGE
	uniform_int_distribution<int> enHP(10, 50); //NORMAL ENEMY HP
	uniform_int_distribution<int> bossHP(50, 100); //BOSS HP

	//FORMAT
	cout << "\n=========================\n\tBATTLE\n=========================\n";

	//DETERMINE ENEMY HP
	if (!isBoss) {
		enemyHP = enHP(randomGenerator); //Randomly assign HP from 10 to 50 based on RNG.
		cout << "This enemy has " << enemyHP << "HP!" << endl;
	}
	else {
		//This is the boss. His HP is from 50 to 100.
		enemyHP = bossHP(randomGenerator);
		cout << "\n********************\n!!! BOSS BATTLE !!!\n********************\n";
		cout << "\nOh my! The boss has " << enemyHP << "HP! Be careful " << playerName << "!" << endl;
	}

	while (enemyHP > 0 && playerHP > 0) {
		//LOOP UNTIL ENEMY IS DEAD OR PLAYER IS DEAD

		//--- PLAYER TURN ---//
		cout << "\nIt's your turn!\nReady to roll the die?" << endl;
		system("PAUSE");
		playerRoll = dieRoll(randomGenerator); //The amount of damage the player will deal
		cout << "You rolled a " << playerRoll << endl;
		enemyHP -= playerRoll;
		if( enemyHP > 0)
			cout << "You've dealt " << playerRoll << " damage to the enemy! It now has " << enemyHP << "HP remaining!\n";
		else {
			cout << "You've dealt " << playerRoll << " damage to the enemy!\n**It has been defeated!**\n";
			return true;
		}
		//--- END PLAYER TURN ---//

		//--- ENEMY TURN ---//
		if (!isBoss) {
			cout << "\nEnemy turn!" << endl;
			enemyRoll = enDMG(randomGenerator); //Determine its damage
			playerHP -= enemyRoll;
			if (playerHP > 0)
				cout << "The enemy dealt " << enemyRoll << " damage to your HP!\nYou now have " << playerHP << "HP remaining!" << endl;
			else {
				cout << "The enemy dealt " << enemyRoll << " damage to your HP!\n**You have been defeated! :( **" << endl;
				return false;
			}
		}
		else {
			cout << "\nIt's the boss' turn!" << endl;
			enemyRoll = bossDMG(randomGenerator); //Determine its damage
			playerHP -= enemyRoll;
			if (playerHP > 0)
				cout << "The boss dealt " << enemyRoll << " damage to your HP!\nYou now have " << playerHP << "HP remaining!" << endl;
			else {
				cout << "The boss dealt " << enemyRoll << " damage to your HP!\n**You have been defeated! :( **" << endl;
				return false;
			}
		}
		//--- END ENEMY TURN ---//
		system("PAUSE"); //Pause for a moment
	}
}

int main()
{
	//----- VARIABLES -----//
	default_random_engine randomGenerator(time(NULL));
	uniform_int_distribution<int> dieRoll(1, 6);
	int dungeonLength = 100;
	bool dungeon [100]; //true = enemy present; false = enemy not present. Cell 100 is boss and grail.
	populateEnemies(dungeon, randomGenerator); //Populates the dungeon with randomly determined enemies.
	int currPos = 0;
	int diceRoll;
	bool viewHelp = true;
	char inputDecide;
	string playerName = "";
	int playerHP = 100;
	bool finishedGame, won = false;
	//----- END VARS -----//

	cout << "Welcome to DnDTextRun!\n";
	cout << "The rules are simple.\nYour objective is to find the almighty Holy Grail at the end of the dungeon!" << endl;

	do {
		cout << "Do you wish to view the HELP menu with instructions? (Y/N) \n";
		cin >> inputDecide; //This will only read the first char

		if (toupper(inputDecide) == 'Y') {
			//DISPLAY HELP
			cout << "OK!\nThe Holy Grail is at the very end of your adventure through this dark dungeon.\nOn your journey, many enemies will appear and fight you.\n";
			cout << "To attack enemies, you must roll a six-sided die. \nThe value rolled will determine the amount of damage you inflict on the enemy.\n";
			cout << "Be careful! Enemies can also attack! This game is turn-based. \nTheir damage depends, again, on a die roll of their own.\n";
			cout << "Enemy HP (hit points) must be brought down to 0 in order to defeat them and move on through the dungeon.\n";
			cout << "Enemy HP will vary between 10 to 50, but bosses can have up to 100HP! \nYou, on the other hand, always have 100HP.\n";
			cout << "Your HP will full heal after each encounter.\nAt the start of your turn, you will roll a die to determine how many cells you move by.\n";
			cout << "If an enemy is on the cell you land on, a battle will begin. \nWhen you reach the end, fight the boss and acquire the Grail for victory!" << endl;

			viewHelp = false;
		}
		else if (toupper(inputDecide) == 'N') {
			//Move onto setup
			cout << "OK! Now onto the setup!" << endl;
			viewHelp = false;
		}
		else {
			//Repeat since unknown input.
			cout << "Unknown input. Please try again! :)" << endl;
		}
	} while (viewHelp);

	cout << "\nWhat is your name?\n";
	cin >> playerName;
	cout << "Awesome, " << playerName << "! The dungeon is made of " << dungeonLength << " cells." << endl;

	//-- BEGIN GAME --//
	do {
		cout << "\nLet's move! Ready to roll the die?\n";
		system("PAUSE");


		diceRoll = dieRoll(randomGenerator);
		cout << "You rolled a " << diceRoll << endl;
		currPos += diceRoll;

		//PROCESS FOR CELL OUTPUT
		if (currPos <= sizeof(dungeon)) {
			cout << "You are now at cell #" << currPos << endl;
		}
		else {
			cout << "You're at the last cell! Find the grail, quick!\n" << endl;
		}
		
		//CHECK FOR BATTLES
		if (dungeon[currPos] && currPos < dungeonLength) {
			//If our current position in the dungeon returns true, it means we have a battle!
			if (battleHandler(playerHP, playerName, false)) {
				cout << "\nYou did it! Now onto the next room!\n" << endl;
				finishedGame = false;
				playerHP = 100;
			}
			else {
				finishedGame = true;
				won = false;
			}
		}
		else if (currPos >= dungeonLength) {
			cout << "\nThe Grail is in your sights! Defeat the final boss to claim your reward!\n" << endl;
			//FINAL BOSS
			//CALL BATTLE WITH TRUE
			if (battleHandler(playerHP, playerName, true)) {
				cout << "\nYES! The Grail is in front of you! Quick, grab it!\n";
				finishedGame = true;
				won = true;
			}
			else {
				finishedGame = true;
				won = false;
			}
		}
		else {
			cout << "Hmm... No beast in sights! Keep going!\n" << endl;
			finishedGame = false;
		}
	} while (!finishedGame);
	//-- END GAME --//

	//--- HANDLE POST-GAME PROCESSING ---//
	if (!won) {
		cout << "\nThe fairies have been able to revive you! Fear not, there is always next time! Best of luck, " << playerName << "!" << endl;
	}
	else {
		cout << "\nThe Grail i yours!! Congratulations on conquering the darkest depths of this dungeon, " << playerName << "!" << endl;
	}
	//--- END POST-GAME PROCESSING ---//

	system("PAUSE");
    return 0;
}

