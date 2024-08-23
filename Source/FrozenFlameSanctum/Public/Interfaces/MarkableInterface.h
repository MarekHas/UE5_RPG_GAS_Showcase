// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MarkableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMarkableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FROZENFLAMESANCTUM_API IMarkableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void MarkActor() = 0;
	virtual void UnmarkActor() = 0;
};
