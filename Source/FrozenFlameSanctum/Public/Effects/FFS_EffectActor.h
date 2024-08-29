// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"

#include "FFS_EffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationType : uint8
{
	OnOverlap		UMETA(DisplayName = "OnOverlap apply effect"),
	OnEndOverlap	UMETA(DisplayName = "OnEndOverlap apply effect"),
	DoNotApply		UMETA(DisplayName = "Do not apply effect")
};

UENUM(BlueprintType)
enum class EEffectRemovalType : uint8
{
	OnEndOverlap	UMETA(DisplayName = "OnEndOverlap remove effect"),
	DoNotRemove		UMETA(DisplayName = "Do not remove effect")
};

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

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToActor(TSubclassOf<UGameplayEffect> InGameplayEffectClass, AActor* AffectedActor);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* OverlapActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* OverlapActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	EEffectApplicationType ApplicationType = EEffectApplicationType::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	EEffectRemovalType RemovalType = EEffectRemovalType::OnEndOverlap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	float EffectLevel = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	bool bDestroyAfterEffectApplied = false;

private:
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
};
