// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FFS_CharacterBase.h"
#include "Interfaces/PlayerInterface.h"
#include "FFS_HeroCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API AFFS_HeroCharacter : public AFFS_CharacterBase, public IPlayerInterface
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

	/** Player Interface */
	virtual void LevelUp_Implementation() override;
	virtual void AddExperiencePoints_Implementation(int32 InExperiencePoints) override;
	virtual int32 CheckLevelForGivenExperience_Implementation(int32 InExperiencePoints) const override;

	virtual int32 GetSkillPoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual int32 GetExperiencePoints_Implementation() const override;
	virtual int32 GetSkillPointsReceived_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReceived_Implementation(int32 Level) const override;
	
	virtual void AddPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddSkillPoints_Implementation(int32 InSkillPoints) override;
	virtual void AddSpellPoints_Implementation(int32 InSpellPoints) override;
	/** end Player Interface */
	
	/** Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	/** end Combat Interface */
	
private:
	//~Begin AFFS_CharacterBase override
	virtual void InitAbilityActorInfo() override;
	//~End AFFS_CharacterBase override
	// 
	//Add HUD Widget for valid player controller
	void InitHUDWidget();
};
