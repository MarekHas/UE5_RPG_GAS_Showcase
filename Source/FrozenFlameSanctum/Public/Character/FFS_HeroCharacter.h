// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FFS_CharacterBase.h"
#include "FFS_HeroCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API AFFS_HeroCharacter : public AFFS_CharacterBase
{
	GENERATED_BODY()

public:
	AFFS_HeroCharacter();

	/**APawn
	 * Called when this Pawn is possessed. Only called on the server (or in standalone).
	 * @param NewController The controller possessing this pawn
	 */
	virtual void PossessedBy(AController* NewController) override;
	/** APawn 
	* PlayerState Replication Notification Callback*/
	virtual void OnRep_PlayerState() override;
private:
	void InitAbilityActorInfo();
};
