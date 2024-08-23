// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FFS_EnemyCharacter.h"

AFFS_EnemyCharacter::AFFS_EnemyCharacter()
{
	//This settings is needed for marking actor, in player controller is checked if cursor hit actor on visiblity chanel with block 
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AFFS_EnemyCharacter::MarkActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(250);
}

void AFFS_EnemyCharacter::UnmarkActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(0);
}
