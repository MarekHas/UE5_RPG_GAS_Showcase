// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FFS_EffectActor.generated.h"

UCLASS()
class FROZENFLAMESANCTUM_API AFFS_EffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFFS_EffectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
