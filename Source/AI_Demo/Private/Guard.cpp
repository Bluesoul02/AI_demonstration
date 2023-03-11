// Fill out your copyright notice in the Description page of Project Settings.

#include "guard.h"
#include <Kismet/KismetMathLibrary.h>
#include <Waypoint.h>

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Yellow, TEXT(x));}

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
		//Velocity.Z = target.Z;
		target.Z = GetActorLocation().Z; //ignore Z axis by setting at the same value as guard

		if (CurrentMode == FString("Seek")) {
			CurrentWaypointTwoWays = 0;
			CurrentWaypointOneWay = 0;
			target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			CurrentWaypoint = 0;
			twoWays = false;
			Seek();
		}

		else if (CurrentMode == FString("Flee")) {
			CurrentWaypointTwoWays = 0;
			CurrentWaypointOneWay = 0;
			target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			CurrentWaypoint = 0;
			twoWays = false;
			Flee();
		}

		else if (CurrentMode == FString("Pursuit")) {
			CurrentWaypointTwoWays = 0;
			CurrentWaypointOneWay = 0;
			target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			CurrentWaypoint = 0;
			twoWays = false;
			Pursuit();
		}

		else if (CurrentMode == FString("Evade")) {
			CurrentWaypointTwoWays = 0;
			CurrentWaypointOneWay = 0;
			target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			CurrentWaypoint = 0;
			twoWays = false;
			Evade();
		}

		else if (CurrentMode == FString("Arrival")) {
			CurrentWaypointTwoWays = 0;
			CurrentWaypointOneWay = 0;
			target = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			CurrentWaypoint = 0;
			twoWays = false;
			Arrival();
		}

		else if (CurrentMode == FString("OneWay")) {
			CurrentWaypointTwoWays = 0;
			CurrentWaypoint = 0;
			twoWays = false;
			OneWay();
		}

		else if (CurrentMode == FString("TwoWays")) {
			CurrentWaypointOneWay = 0;
			CurrentWaypoint = 0;
			TwoWays();
		}

		else if (CurrentMode == FString("Circuit")) {
			CurrentWaypointTwoWays = 0;
			CurrentWaypointOneWay = 0;
			twoWays = false;
			Circuit();
		}

		else if (CurrentMode == FString("One Point")) {
			OnePoint();
		}

		else {
			CurrentWaypointTwoWays = 0;
			CurrentWaypointOneWay = 0;
			CurrentWaypoint = 0;
			twoWays = false;

			hasPath = false;
			CurrentGraphPoint = 0;
			Path.Empty();
		}

		this->SetActorRotation(Velocity.Rotation());

		//FVector new_forward = Velocity.GetSafeNormal();
		//FVector approximate_up = GetActorRotation().Vector();
		//approximate_up.GetSafeNormal(); // if needed
		//FVector new_side = FVector::CrossProduct(new_forward, approximate_up);
		//FVector new_up = FVector::CrossProduct(new_forward, new_side);
		//
		//this->SetActorRotation(FRotator(new_up.X,new_up.Y, new_up.Z)); // not working
	}
}

void AGuard::Seek() {
	FVector desired_velocity = (target - GetActorLocation());
	desired_velocity = desired_velocity.GetSafeNormal() * GameInstance->GetMaxSpeed();
	FVector steering = desired_velocity - Velocity;
	SteeringForce = steering.GetClampedToMaxSize(GameInstance->GetMaxForce());
	Acceleration = SteeringForce / GameInstance->GetMass();
	Velocity = (Velocity + Acceleration).GetClampedToMaxSize(GameInstance->GetMaxSpeed());
	Position = GetActorLocation() + Velocity;
	//this->AddMovementInput(steering, 1.f, false);
	this->SetActorLocation(Position);
}

void AGuard::Flee() {
	FVector desired_velocity = (GetActorLocation() - target);
	desired_velocity = desired_velocity.GetSafeNormal() * GameInstance->GetMaxSpeed();
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
	FVector prediction = target + (TargetVelocity.GetSafeNormal() * dist);
	//FVector direction = target - GetActorLocation());
	//direction.Normalize(1.0);
	//if (FVector::DotProduct(this->GetActorForwardVector(), direction) >= 0.5)
	target = prediction;
	Seek();
}

void AGuard::Evade() {
	FVector TargetVelocity = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetVelocity();
	float dist = FVector::Dist(target, GetActorLocation());
	FVector prediction = target + (TargetVelocity.GetSafeNormal() * dist);
	target = prediction;
	Flee();
}

void AGuard::Arrival() {		
	FVector target_offset = target - GetActorLocation();
	int distance = target_offset.Size();
	double ramped_speed = GameInstance->GetMaxSpeed() * (distance / GameInstance->GetSlowingDistance());
	double clipped_speed = FMath::Min(ramped_speed, GameInstance->GetMaxSpeed());
	FVector desired_velocity = (clipped_speed / distance) * target_offset;

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
	if (CurrentWaypointOneWay == GameInstance->GetWaypointsOneWay()) {
		// do nothing
	}
	else if (target.Equals(GetActorLocation(), 5.0f)) {
		Cast<AWaypoint>(ActorsToFind[CurrentWaypointOneWay])->SetIsTarget(false);
		CurrentWaypointOneWay++;
	}

	if (GameInstance->GetWaypointsOneWay() < CurrentWaypointOneWay) {
		CurrentWaypointOneWay = 0;
	}

	Cast<AWaypoint>(ActorsToFind[CurrentWaypointOneWay])->SetIsTarget(true);
	target = ActorsToFind[CurrentWaypointOneWay]->GetActorLocation();
	if (GameInstance->GetWaypointsOneWay() == CurrentWaypointOneWay)
		Arrival();
	else
		Seek();
}

void AGuard::TwoWays() {

	TArray<AActor*> ActorsToFind;
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AWaypoint::StaticClass(), FName("TwoWays"), ActorsToFind);
	}

	if (target.Equals(GetActorLocation(), 5.0f)) {
		Cast<AWaypoint>(ActorsToFind[CurrentWaypointTwoWays])->SetIsTarget(false);
		if (!twoWays)
			CurrentWaypointTwoWays++;
		else
			CurrentWaypointTwoWays--;
	}
	if (CurrentWaypointTwoWays < 0) {
		twoWays = !twoWays;
		CurrentWaypointTwoWays++;
	}
	else if (GameInstance->GetWaypointsTwoWays() < CurrentWaypointTwoWays){
		twoWays = !twoWays;
		CurrentWaypointTwoWays--;
	}
	else {
		Cast<AWaypoint>(ActorsToFind[CurrentWaypointTwoWays])->SetIsTarget(false);
	}

	Cast<AWaypoint>(ActorsToFind[CurrentWaypointTwoWays])->SetIsTarget(true);
	target = ActorsToFind[CurrentWaypointTwoWays]->GetActorLocation();

	if (CurrentWaypointTwoWays == 0 || GameInstance->GetWaypointsTwoWays() == CurrentWaypointTwoWays)
		Arrival();
	else
		Seek();
}

void AGuard::Circuit() {

	TArray<AActor*> ActorsToFind;
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AWaypoint::StaticClass(), FName("Circuit"), ActorsToFind);
	}

	if (target.Equals(GetActorLocation(), 5.0f)) {
		Cast<AWaypoint>(ActorsToFind[CurrentWaypoint])->SetIsTarget(false);
		CurrentWaypoint++;
	}

	if (GameInstance->GetWaypoints() < CurrentWaypoint) {
		CurrentWaypoint = 0;
	}

	Cast<AWaypoint>(ActorsToFind[CurrentWaypoint])->SetIsTarget(true);
	target = ActorsToFind[CurrentWaypoint]->GetActorLocation();
	Seek();
}

TArray<AGuard::FNode*> AGuard::GetAvailableNodes(AWaypoint* wp) {
	TArray<FNode*> nodes = TArray<FNode*>();
	for (AWaypoint* v : wp->GetAvailableWaypoints()) {
		FNode* u = new FNode;
		u->waypoint = v;
		nodes.Add(u);
	}
	return nodes;
}

// Helper function to calculate the Manhattan distance between two waypoints
int32 ManhattanDistance(const FVector& A, const FVector& B) {
	return FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y) + FMath::Abs(A.Z - B.Z);
}

void AGuard::CalculatePath(AWaypoint* Start, AWaypoint* Goal) {
	FNode* StartNode = new FNode;
	StartNode->waypoint = Start;
	StartNode->G = 0;
	StartNode->H = ManhattanDistance(Start->GetActorLocation(), Goal->GetActorLocation());
	//StartNode->F = StartNode->G + StartNode->H;
	TArray<FNode*> ClosedLists = TArray<FNode*>();
	TArray<FNode*> OpenList = TArray<FNode*>();
	OpenList.Add(StartNode);
	while (!OpenList.IsEmpty()) {

		// Find the FNode with the lowest F cost
		FNode* u = OpenList[0];
		float LowestFCost = ManhattanDistance(u->waypoint->GetActorLocation(), Goal->GetActorLocation());
		for (int32 i = 1; i < OpenList.Num(); i++) {
			float FCost = ManhattanDistance(OpenList[i]->waypoint->GetActorLocation(), Goal->GetActorLocation());
			if (FCost < LowestFCost) {
				u = OpenList[i];
				LowestFCost = FCost;
			}
		}

		OpenList.Remove(u);
		ClosedLists.Add(u);


		// If the current FNode is the end FNode, return the path
		if (u->waypoint == Goal) {
			Path.Add(Goal);
			FNode* ParentNode = u->Parent;
			while (ParentNode && ParentNode->waypoint != Start) { // crash ?
				Path.Insert(ParentNode->waypoint, 0);
				u = ParentNode;
				ParentNode = ParentNode->Parent;
			}
			Path.Insert(Start, 0);
			return;
		}

		//pour chaque voisin v de u
		for (FNode* v : GetAvailableNodes(Cast<AWaypoint>(u->waypoint))) {

			if (ClosedLists.Contains(v)) {
				continue;
			}

			float GCost = ManhattanDistance(v->waypoint->GetActorLocation(), Start->GetActorLocation());
			float HCost = ManhattanDistance(v->waypoint->GetActorLocation(), Goal->GetActorLocation());

			float FCost = GCost + HCost;

			if (!OpenList.Contains(v)) {
				v->G = GCost;
				v->H = HCost;
				v->Parent = u;
				OpenList.Add(v);
			}

			else {
				int32 Index = OpenList.Find(v);
				if (FCost < OpenList[Index]->G + OpenList[Index]->H) {
					OpenList[Index]->G = GCost;
					OpenList[Index]->H = HCost;
					OpenList[Index]->Parent = u;
				}
			}
		}
	}
	D("erreur calculate");
	// terminer le programme (avec erreur)
}

void AGuard::OnePoint() {

	if (!hasPath) {
		target = StartWaypoint->GetActorLocation(); // crash after empty
	}

	target.Z = GetActorLocation().Z; //ignore Z axis by setting at the same value as guard

	if (target.Equals(GetActorLocation(), 5.0f)) {
		if (Path.IsValidIndex(CurrentGraphPoint+1)) {
			target = Path[++CurrentGraphPoint]->GetActorLocation();
		}
	}

	if (!hasPath) {
		if (GameInstance->GetOnePoint()) {
			CalculatePath(StartWaypoint, GameInstance->GetOnePoint());
			hasPath = true;
		}
	}
	if (Path.IsValidIndex(CurrentGraphPoint)) { // crash?
		if (Path[CurrentGraphPoint] != GameInstance->GetOnePoint())
			Seek();
		else {
			Arrival();
			if (target.Equals(GetActorLocation(), 5.0f)) {
				hasPath = false;
				CurrentGraphPoint = 0;
				Path.Empty();
				StartWaypoint = GameInstance->GetOnePoint();
				GameInstance->SetOnePoint(nullptr);
			}
		}
	}
}