// Fill out your copyright notice in the Description page of Project Settings.


#include "OwnPhysicalComponent.h"
#include "WeaponBounceCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Json.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UOwnPhysicalComponent::UOwnPhysicalComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	BoneName = "None";
	HitReactionTimeRemaining = 0;
	DeltaTimeSpeed = 1;
	FInterpSpeed = 0.5f;
	WeaponBounce = 160.f;

	JsonFileParser();
}


// Called when the game starts
void UOwnPhysicalComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UOwnPhysicalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HitReactionTimeRemaining <= 0)
	{
		HitReactionTimeRemaining = 0;
		if (CharacterSkeletalMesh)
		{
			CharacterSkeletalMesh->SetAllBodiesBelowSimulatePhysics(BoneName, false, true);
		}

		this->SetComponentTickEnabled(false);
		Character->GetMovementComponent()->Activate();
		Character->bIsHitted = false;

	}
	else
	{
		FMath::FInterpTo(HitReactionTimeRemaining, 0.f, 0.f, FInterpSpeed);
		HitReactionTimeRemaining -= DeltaTime*DeltaTimeSpeed;
		if (CharacterSkeletalMesh)
		{
			CharacterSkeletalMesh->SetAllBodiesBelowPhysicsBlendWeight(BoneName, FMath::Min<float>(HitReactionTimeRemaining, 1.f), false, true);
		}
	}
}

void UOwnPhysicalComponent::JsonFileParser()
{

	FString FilePath = FPaths::ProjectConfigDir().Append("MannequinParams.msg");
    FString FileData = "";

    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("DID NOT FIND FILE"));
		return;
    }

	if (!FFileHelper::LoadFileToString(FileData,*FilePath))
	{

        UE_LOG(LogTemp, Warning, TEXT("FILE IS NOT LOADED"));
		return;
	}

	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(FileData);

    if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
    {
		FString ResultJson = JsonParsed->GetStringField("WeaponBounceStr");
		WeaponBounce = FCString::Atof(*ResultJson);
        UE_LOG(LogTemp, Warning, TEXT("WeaponBounce: %f"), WeaponBounce);
    }
	else
	{
    	UE_LOG(LogTemp, Warning, TEXT("FILE DESERIALIZE FILED"));
	}
}

void UOwnPhysicalComponent::OnHitReaction_Implementation(FVector HitDir, FVector SurfaceNormal, FVector HitLocation, FName IncBoneName, float IncHitReactionTimeRemaining)
{
	if(!CharacterSkeletalMesh) return;

	Character->bIsHitted = true;

	if(ImpactVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, HitLocation);
	}

	if(ImpactSound)
	{

		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, HitLocation);

	}

	this->SetComponentTickEnabled(true);

	HitReactionTimeRemaining = IncHitReactionTimeRemaining;
	BoneName = IncBoneName;

	FVector ReflectImpulse = HitDir - 2 * FVector::DotProduct(HitDir, SurfaceNormal) * SurfaceNormal;

	float Dot = FVector::DotProduct(SurfaceNormal, ReflectImpulse);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Dot: %f"), Dot));
	
	ReflectImpulse = ReflectImpulse * WeaponBounce * 20;
	if (abs(Dot) >= 0.25)
	{
		Character->bIsAttack = false;		
	}

	CharacterSkeletalMesh->SetAllBodiesBelowSimulatePhysics(BoneName, true, true);
	CharacterSkeletalMesh->AddImpulse(ReflectImpulse,"hand_r", false);
}
