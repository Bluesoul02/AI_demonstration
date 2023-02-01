// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TP_AIGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AI_DEMO_API UTP_AIGameInstance : public UGameInstance
{
	GENERATED_BODY()

	private:
		UPROPERTY(BlueprintGetter = GetCurrentMode, BlueprintSetter = SetCurrentMode)
			FString CurrentMode;

		UPROPERTY(BlueprintGetter = GetMaxSpeed, BlueprintSetter = SetMaxSpeed)
			double MaxSpeed = 6;

		UPROPERTY(BlueprintGetter = GetMaxForce, BlueprintSetter = SetMaxForce)
			double MaxForce = 3;

		UPROPERTY(BlueprintGetter = GetSlowingDistance, BlueprintSetter = SetSlowingDistance)
			double SlowingDistance = 350;

		UPROPERTY(BlueprintGetter = GetMass, BlueprintSetter = SetMass)
			double Mass = 9;

		UPROPERTY(BlueprintGetter = GetWaypoints, BlueprintSetter = SetWaypoints)
			int Waypoints = 0;

		UPROPERTY(BlueprintGetter = GetWaypointsOneWay, BlueprintSetter = SetWaypointsOneWay)
			int WaypointsOneWay = 0;

		UPROPERTY(BlueprintGetter = GetWaypointsTwoWays, BlueprintSetter = SetWaypointsTwoWays)
			int WaypointsTwoWays = 1;
	public:
		UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
			FString GetCurrentMode() const { return CurrentMode; }

		UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
			void SetCurrentMode(const FString mode) { CurrentMode = mode; }

		UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
			int GetWaypoints() const { return Waypoints; }

		UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
			void SetWaypoints(const int waypoints_count) { Waypoints = waypoints_count; }

		UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
			double GetMaxSpeed() const { return MaxSpeed; }

		UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
			void SetMaxSpeed(const double speed) { MaxSpeed = speed; }

		UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
			double GetMaxForce() const { return MaxForce; }

		UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
			void SetMaxForce(const double force) { MaxForce = force; }

		UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
			double GetSlowingDistance() const { return SlowingDistance; }

		UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
			void SetSlowingDistance(const double dist) { SlowingDistance = dist; }

		UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
			double GetMass() const { return Mass; }

		UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
			void SetMass(const double new_mass) { Mass = new_mass; }

		UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
			int GetWaypointsOneWay() const { return WaypointsOneWay; }

		UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
			void SetWaypointsOneWay(const int waypoints_count) { WaypointsOneWay = waypoints_count; }

		UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
			int GetWaypointsTwoWays() const { return WaypointsTwoWays; }

		UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
			void SetWaypointsTwoWays(const int waypoints_count) { WaypointsTwoWays = waypoints_count; }
};
