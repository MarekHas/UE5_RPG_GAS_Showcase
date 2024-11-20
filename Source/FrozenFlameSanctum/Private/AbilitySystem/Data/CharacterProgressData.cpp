// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/CharacterProgressData.h"

int32 UCharacterProgressData::GetCharacterLevel(int32 InExperiencePoints) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		if (LevelUpData.Num() - 1 <= Level) return Level;
		if (InExperiencePoints >= LevelUpData[Level].ExperiencePointsRequired)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
