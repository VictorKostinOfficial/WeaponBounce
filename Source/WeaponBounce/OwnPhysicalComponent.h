// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OwnPhysicalComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEAPONBOUNCE_API UOwnPhysicalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOwnPhysicalComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DeltaTimeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FInterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WeaponBounce;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class USoundCue* ImpactSound;

	class USkeletalMeshComponent* CharacterSkeletalMesh;

	class AWeaponBounceCharacter* Character;

	// class USkeletalMeshComponent* CharacterMesh;
protected:

	FName BoneName;
	float HitReactionTimeRemaining;
	// Called when the game starts
	virtual void BeginPlay() override;

	void JsonParser();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	class UPhysicalAnimationComponent* PhysicalAnimationComp;

	void JsonFileParser();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnHitReaction(FVector HitDir, FVector SurfaceNormal, FVector HitLocation, FName IncBoneName, float IncHitReactionTimeRemaining);
};
