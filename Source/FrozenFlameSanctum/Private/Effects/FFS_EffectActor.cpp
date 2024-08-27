// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/FFS_EffectActor.h"

// Sets default values
AFFS_EffectActor::AFFS_EffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFFS_EffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}