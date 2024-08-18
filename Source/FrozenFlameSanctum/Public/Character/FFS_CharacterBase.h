// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FFS_CharacterBase.generated.h"

UCLASS(Abstract)
class FROZENFLAMESANCTUM_API AFFS_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFFS_CharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category ="Props")
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
