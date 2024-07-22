#include "Player.h"

void Player::SetHealth(int newHealth)
{
	PlayerStatLimits.MaxHealth = newHealth;
	CurrentHealth = newHealth;
}

void Player::SetMana(int newMana)
{
	PlayerStatLimits.MaxMana = newMana;
	CurrentMana = newMana;
}

void Player::AddExp(int ExpToAdd)
{
	if (CurrentExp >= PlayerStatLimits.ExpToLevel)
	{
		GainLevel();
	}
	CurrentExp += ExpToAdd;
}

void Player::GainLevel()
{
	CurrentLevel += 1;
	TotalExpGained += CurrentExp;
	CurrentExp = 0;
	PlayerStatLimits.ExpToLevel *= ExpToLevelModifier;
}
