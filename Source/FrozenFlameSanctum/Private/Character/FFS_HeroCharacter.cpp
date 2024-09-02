// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FFS_HeroCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystem/FFS_AbilitySystemComponent.h"
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
	//Bind to AbilitySystemComponent delegate after AbilityActorInfo has been set 
	Cast<UFFS_AbilitySystemComponent>(FFS_PlayerState->GetAbilitySystemComponent())->BindToAbilitySystemDelegates();
	AbilitySystemComponent = FFS_PlayerState->GetAbilitySystemComponent();
	AttributeSet = FFS_PlayerState->GetAttributeSet();
	
	//Init HUD Widget - only for valid PlayerController 
	if (AFFS_PlayerController* FFS_PlayerController = Cast<AFFS_PlayerController>(GetController()))
	{
		if (AFFS_GameHUD* GameHUD = Cast<AFFS_GameHUD>(FFS_PlayerController->GetHUD()))
		{
			GameHUD->InitWidget(FFS_PlayerController, FFS_PlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
