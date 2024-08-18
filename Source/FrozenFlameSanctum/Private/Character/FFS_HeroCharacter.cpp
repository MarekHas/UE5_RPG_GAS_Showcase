// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FFS_HeroCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

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
