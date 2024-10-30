// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/EnemiesData.h"

FEnemyDefaultStats UEnemiesData::GetClassDefaultInfo(EEnemyType EnemyType)
{
	return EnemyInfo.FindChecked(EnemyType);
}
