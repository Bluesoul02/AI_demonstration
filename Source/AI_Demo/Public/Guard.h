// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h" 
#include <TP_AIGameInstance.h>
#include <Kismet/GameplayStatics.h>
#include "Guard.generated.h"

UCLASS()
class AI_DEMO_API AGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGuard();

protected:
	UTP_AIGameInstance* GameInstance;
	FVector SteeringForce;
	FVector Acceleration;
	FVector Velocity = this->GetVelocity();
	FVector Position = this->GetActorLocation();
	FVector target;
	bool twoWays = false;
	int CurrentWaypoint = 0;
	int CurrentWaypointOneWay = 0;
	int CurrentWaypointTwoWays = 0;
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Seek();

	virtual void Flee();

	virtual void Pursuit();

	virtual void Evade();

	virtual void Arrival();

	virtual void OneWay();

	virtual void TwoWays();

	virtual void Circuit();
};
