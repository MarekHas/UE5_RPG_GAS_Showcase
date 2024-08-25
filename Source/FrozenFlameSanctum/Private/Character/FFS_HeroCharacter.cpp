// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FFS_HeroCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/FFS_PlayerState.h"

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
}

void AFFS_HeroCharacter::OnRep_PlayerState()
{
	// Init for the Client
	InitAbilityActorInfo();
}

void AFFS_HeroCharacter::InitAbilityActorInfo()
{
	AFFS_PlayerState* FFS_PlayerState = GetPlayerState<AFFS_PlayerState>();
	check(FFS_PlayerState);

	FFS_PlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(FFS_PlayerState, this);
	AbilitySystemComponent = FFS_PlayerState->GetAbilitySystemComponent();
	AttributeSet = FFS_PlayerState->GetAttributeSet();
}
