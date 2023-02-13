// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Waypoint.generated.h"

UCLASS()
class AI_DEMO_API AWaypoint : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintGetter = GetIsTarget, BlueprintSetter = SetIsTarget)
		bool IsTarget = false;
	UPROPERTY(BlueprintGetter = GetAvailableWaypoints, BlueprintSetter = SetAvailableWaypoints, EditAnywhere)
		TArray<AWaypoint*> AvailableWaypoints;
	
public:	
	// Sets default values for this actor's properties
	AWaypoint();

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
		bool GetIsTarget() const { return IsTarget; }

	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
		void SetIsTarget(const bool is_target) { IsTarget = is_target; }

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
		TArray<AWaypoint*> GetAvailableWaypoints() const { return AvailableWaypoints; }

	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
		void SetAvailableWaypoints(const TArray<AWaypoint*> waypoints) { AvailableWaypoints = waypoints; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
