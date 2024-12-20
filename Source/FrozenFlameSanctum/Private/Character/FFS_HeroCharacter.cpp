// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FFS_HeroCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/Data/CharacterProgressData.h"
#include "Player/FFS_PlayerState.h"
#include "Player/FFS_PlayerController.h"
#include "UI/HUD/FFS_GameHUD.h"

AFFS_HeroCharacter::AFFS_HeroCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AFFS_HeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init for the Server
	InitAbilityActorInfo();
	//Character stats only initialize on server values will be replicated to client
	InitDefaultStats();
	AddCharacterAbilities();
	InitHUDWidget();
}

void AFFS_HeroCharacter::OnRep_PlayerState()
{
	// Init for the Client
	InitAbilityActorInfo();
	InitDefaultStats();
	InitHUDWidget();
}

void AFFS_HeroCharacter::AddExperiencePoints_Implementation(int32 InExperiencePoints)
{
	AFFS_PlayerState* AuraPlayerState = GetPlayerState<AFFS_PlayerState>();
	check(AuraPlayerState);
	
	AuraPlayerState->AddExperiencePoints(InExperiencePoints);
}

void AFFS_HeroCharacter::LevelUp_Implementation()
{
	IPlayerInterface::LevelUp_Implementation();
}

int32 AFFS_HeroCharacter::CheckLevelForGivenExperience_Implementation(int32 InExperiencePoints) const
{
	const AFFS_PlayerState* FFS_PlayerState = GetPlayerState<AFFS_PlayerState>();
	check(FFS_PlayerState);
	return FFS_PlayerState->CharacterProgressData->GetCharacterLevel(InExperiencePoints);
}

int32 AFFS_HeroCharacter::GetSkillPoints_Implementation() const
{
	const AFFS_PlayerState* FFS_PlayerState = GetPlayerState<AFFS_PlayerState>();
	check(FFS_PlayerState);
	return FFS_PlayerState->GetSkillPoints();
}

int32 AFFS_HeroCharacter::GetSpellPoints_Implementation() const
{
	const AFFS_PlayerState* FFS_PlayerState = GetPlayerState<AFFS_PlayerState>();
	check(FFS_PlayerState);
	return FFS_PlayerState->GetSpellPoints();
}

int32 AFFS_HeroCharacter::GetExperiencePoints_Implementation() const
{
	const AFFS_PlayerState* FFS_PlayerState = GetPlayerState<AFFS_PlayerState>();
	check(FFS_PlayerState);
	return FFS_PlayerState->GetExperiencePoints();
}

int32 AFFS_HeroCharacter::GetSkillPointsReceived_Implementation(int32 Level) const
{
	const AFFS_PlayerState* FFS_PlayerState = GetPlayerState<AFFS_PlayerState>();
	check(FFS_PlayerState);
	return FFS_PlayerState->CharacterProgressData->LevelUpData[Level].SkillPointsReceived;
}

int32 AFFS_HeroCharacter::GetSpellPointsReceived_Implementation(int32 Level) const
{
	const AFFS_PlayerState* FFS_PlayerState = GetPlayerState<AFFS_PlayerState>();
	check(FFS_PlayerState);
	return FFS_PlayerState->CharacterProgressData->LevelUpData[Level].SpellPointsReceived;
}

void AFFS_HeroCharacter::AddPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AFFS_PlayerState* FFS_PlayerState = GetPlayerState<AFFS_PlayerState>();
	check(FFS_PlayerState);
	FFS_PlayerState->LevelUp(InPlayerLevel);
}

void AFFS_HeroCharacter::AddSkillPoints_Implementation(int32 InSkillPoints)
{
	AFFS_PlayerState* AuraPlayerState = GetPlayerState<AFFS_PlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddSkillPoints(InSkillPoints);
}

void AFFS_HeroCharacter::AddSpellPoints_Implementation(int32 InSpellPoints)
{
	AFFS_PlayerState* AuraPlayerState = GetPlayerState<AFFS_PlayerState>();
	check(AuraPlayerState);
	
	AuraPlayerState->AddSpellPoints(InSpellPoints);
}

int32 AFFS_HeroCharacter::GetPlayerLevel_Implementation()
{
	const AFFS_PlayerState* FFS_PlayerState = GetPlayerState<AFFS_PlayerState>();
	check(FFS_PlayerState);

	return FFS_PlayerState->GetPlayerLevel();
}

void AFFS_HeroCharacter::InitAbilityActorInfo()
{
	AFFS_PlayerState* FFS_PlayerState = GetPlayerState<AFFS_PlayerState>();
	check(FFS_PlayerState);

	FFS_PlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(FFS_PlayerState, this);
	//Bind to AbilitySystemComponent delegate after AbilityActorInfo has been set 
	Cast<UFFS_AbilitySystemComponent>(FFS_PlayerState->GetAbilitySystemComponent())->BindToAbilitySystemDelegates();
	AbilitySystemComponent = FFS_PlayerState->GetAbilitySystemComponent();
	AttributeSet = FFS_PlayerState->GetAttributeSet();
}

void AFFS_HeroCharacter::InitHUDWidget()
{
	AFFS_PlayerState* FFS_PlayerState = GetPlayerState<AFFS_PlayerState>();
	check(FFS_PlayerState);
	//Init HUD Widget - only for valid PlayerController 
	if (AFFS_PlayerController* FFS_PlayerController = Cast<AFFS_PlayerController>(GetController()))
	{
		if (AFFS_GameHUD* GameHUD = Cast<AFFS_GameHUD>(FFS_PlayerController->GetHUD()))
		{
			GameHUD->InitWidget(FFS_PlayerController, FFS_PlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
