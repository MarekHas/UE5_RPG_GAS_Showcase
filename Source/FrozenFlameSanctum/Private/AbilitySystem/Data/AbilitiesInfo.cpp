// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilitiesInfo.h"

FFFS_AbilityInfo UAbilitiesInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FFFS_AbilityInfo& Info : AbilitiesData)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	return FFFS_AbilityInfo();
}
