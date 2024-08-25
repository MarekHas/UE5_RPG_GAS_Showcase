// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Character/FFS_CharacterBase.h"
#include "Interfaces/MarkableInterface.h"

#include "FFS_EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API AFFS_EnemyCharacter : public AFFS_CharacterBase, public IMarkableInterface
{
	GENERATED_BODY()
	
public:
	AFFS_EnemyCharacter();

	// Begin IMarkableInterface
	void MarkActor() override;
	void UnmarkActor() override;
	// end IMarkableInterface

protected:
	virtual void BeginPlay() override;
private:
	void SetupAbilitySystemComponent();
};
