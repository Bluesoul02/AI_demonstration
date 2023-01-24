// Fill out your copyright notice in the Description page of Project Settings.

#include "guard.h"
#include <Kismet/KismetMathLibrary.h>
#include <Waypoint.h>

// Sets default values
AGuard::AGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
	bGenerateOverlapEventsDuringLevelStreaming = true;
	GameInstance = Cast<UTP_AIGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

// Called every frame
void AGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
		FString CurrentMode = GameInstance->GetCurrentMode();
		target.Z = GetActorLocation().Z; //ignore Z axis by setting at the same value as guard

		if (CurrentMode == FString("Seek")) {
			target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			CurrentWaypoint = 0;
			twoWays = false;
			Seek();
		}

		else if (CurrentMode == FString("Flee")) {
			target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			CurrentWaypoint = 0;
			twoWays = false;
			Flee();
		}

		else if (CurrentMode == FString("Pursuit")) {
			target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			CurrentWaypoint = 0;
			twoWays = false;
			Pursuit();
		}

		else if (CurrentMode == FString("Evade")) {
			target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			CurrentWaypoint = 0;
			twoWays = false;
			Evade();
		}

		else if (CurrentMode == FString("Arrival")) {
			target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			CurrentWaypoint = 0;
			twoWays = false;
			Arrival();
		}

		else if (CurrentMode == FString("One Way")) {
			CurrentWaypoint = 0;
			twoWays = false;
			OneWay();
		}

		else if (CurrentMode == FString("Two Ways")) {
			CurrentWaypoint = 0;
			TwoWays();
		}

		else if (CurrentMode == FString("Circuit")) {
			twoWays = false;
			Circuit();
		}

		this->SetActorRotation(FRotationMatrix::MakeFromX(Velocity).Rotator());

		//FVector new_forward = Velocity.GetSafeNormal();
		//FVector approximate_up = GetActorRotation().Vector();
		//approximate_up.GetSafeNormal(); // if needed
		//FVector new_side = FVector::CrossProduct(new_forward, approximate_up);
		//FVector new_up = FVector::CrossProduct(new_forward, new_side);
		//
		//this->SetActorRotation(FRotator(new_up.X,new_up.Y, new_up.Z));
	}
}

void AGuard::Seek() {
	FVector desired_velocity = (target - GetActorLocation());
	desired_velocity.GetSafeNormal();
	desired_velocity = desired_velocity * GameInstance->GetMaxSpeed();
	FVector steering = desired_velocity - Velocity;
	SteeringForce = steering * GameInstance->GetMaxForce() / steering.Length();
	Acceleration = SteeringForce / GameInstance->GetMass();
	Velocity = (Velocity + Acceleration).GetClampedToMaxSize(GameInstance->GetMaxSpeed());
	Position = GetActorLocation() + Velocity;
	//this->AddMovementInput(steering, 1.f, false);
	this->SetActorLocation(Position);
}

void AGuard::Flee() {
	FVector desired_velocity = (GetActorLocation() - target);
	desired_velocity.GetSafeNormal();
	desired_velocity = desired_velocity * GameInstance->GetMaxSpeed();
	FVector steering = desired_velocity - Velocity;
	SteeringForce = steering.GetClampedToMaxSize(GameInstance->GetMaxForce());
	Acceleration = SteeringForce / GameInstance->GetMass();
	Velocity = (Velocity + Acceleration).GetClampedToMaxSize(GameInstance->GetMaxSpeed());
	Position = GetActorLocation() + Velocity;
	this->SetActorLocation(Position);
}

void AGuard::Pursuit() {
	FVector TargetVelocity = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetVelocity();
	float dist = FVector::Dist(target, GetActorLocation());
	FVector prediction = target + (TargetVelocity * dist);
	//FVector direction = target - GetActorLocation());
	//direction.Normalize(1.0);
	//if (FVector::DotProduct(this->GetActorForwardVector(), direction) >= 0.5)
	target = prediction;
	Seek();
}

void AGuard::Evade() {
	FVector TargetVelocity = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetVelocity();
	float dist = FVector::Dist(target, GetActorLocation());
	FVector prediction = target + (TargetVelocity * dist);
	target = prediction;
	Flee();
}

void AGuard::Arrival() {		
	FVector target_offset = target - GetActorLocation();
	int distance = target_offset.Size();
	double ramped_speed = GameInstance->GetMaxSpeed() * (distance / GameInstance->GetSlowingDistance());
	double clipped_speed = FMath::Min(ramped_speed, GameInstance->GetMaxSpeed());
	FVector desired_velocity = (clipped_speed / distance) * target_offset;
	desired_velocity.GetSafeNormal();

	FVector steering = desired_velocity - Velocity;
	SteeringForce = steering.GetClampedToMaxSize(GameInstance->GetMaxForce());
	Acceleration = SteeringForce / GameInstance->GetMass();
	Velocity = (Velocity + Acceleration).GetClampedToMaxSize(GameInstance->GetMaxSpeed());

	Position = GetActorLocation() + Velocity;
	this->SetActorLocation(Position);
}

void AGuard::OneWay() {
	TArray<AActor*> ActorsToFind;
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AWaypoint::StaticClass(), FName("OneWay"), ActorsToFind);
	}
	for (AActor* WaypointActor : ActorsToFind)
	{
		AWaypoint* Waypoint = Cast<AWaypoint>(WaypointActor);
		target = Waypoint->GetActorLocation();
	}
	Arrival();
}

void AGuard::TwoWays() {
	if (target.Equals(GetActorLocation(), 1.0f))
		twoWays = !twoWays;

	TArray<AActor*> ActorsToFind;
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AWaypoint::StaticClass(), FName("TwoWays"), ActorsToFind);
	}
	if (!twoWays) {
		Cast<AWaypoint>(ActorsToFind[1])->SetIsTarget(false);
		target = ActorsToFind[0]->GetActorLocation();
		Cast<AWaypoint>(ActorsToFind[0])->SetIsTarget(true);
	}
	else {
		Cast<AWaypoint>(ActorsToFind[0])->SetIsTarget(false);
		target = ActorsToFind[1]->GetActorLocation();
		Cast<AWaypoint>(ActorsToFind[1])->SetIsTarget(true);

	}
	Arrival();
}

void AGuard::Circuit() {

	TArray<AActor*> ActorsToFind;
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AWaypoint::StaticClass(), FName("Circuit"), ActorsToFind);
	}

	if (target.Equals(GetActorLocation(), 1.0f)) {
		Cast<AWaypoint>(ActorsToFind[CurrentWaypoint])->SetIsTarget(false);
		CurrentWaypoint++;
	}

	if (GameInstance->GetWaypoints() < CurrentWaypoint) {
		CurrentWaypoint = 0;
	}

	Cast<AWaypoint>(ActorsToFind[CurrentWaypoint])->SetIsTarget(true);
	target = ActorsToFind[CurrentWaypoint]->GetActorLocation();
	Arrival();
}