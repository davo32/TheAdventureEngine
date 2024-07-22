#pragma once
#include "PlayerStats.h"

class Player
{
public:
	PlayerStats GetStatLimits() { return PlayerStatLimits; }
	//Getters
	int GetHealth() { return CurrentHealth; }
	int GetMana() { return CurrentMana; }
	int GetExp() { return CurrentExp; }
	int GetLevel() { return CurrentLevel; }

	//Setters
	void SetHealth(int newHealth);
	void SetMana(int newMana);
	void AddExp(int ExpToAdd);
	void GainLevel();

private:
	PlayerStats PlayerStatLimits;
	int CurrentHealth = 100;
	int CurrentMana = 50;
	int TotalExpGained = 0;
	int CurrentExp = 0;
	int CurrentLevel = 1;
	int ExpToLevelModifier = 2;
};

