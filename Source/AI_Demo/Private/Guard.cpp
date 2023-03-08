// Fill out your copyright notice in the Description page of Project Settings.

#include "guard.h"
#include <Kismet/KismetMathLibrary.h>
#include <Waypoint.h>

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

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

TArray<AWaypoint*> reconstruct_path(TMap<AWaypoint*, AWaypoint*> cameFrom, AWaypoint* current) {
	TArray<AWaypoint*> total_path = TArray<AWaypoint*>();
	total_path.Add(current);
	TArray<AWaypoint*> keys;
	cameFrom.GenerateKeyArray(keys);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("keys %f"), cameFrom.Num()));
	while(keys.Contains(current)) { // pb avec camefrom
		current = cameFrom[current]; // Find()
		D("While keys");
		total_path.Add(current);
	}
	Algo::Reverse(total_path);
	return total_path;
}

// h is the heuristic function
int AGuard::h(Node n1, Node n2) {
	if (n1.heuristique < n2.heuristique)
		return 1;
	else if (n1.heuristique == n2.heuristique)
		return 0;
	else
		return -1;
}

TArray<AGuard::Node> AGuard::GetAvailableNodes(AWaypoint* wp) {
	TArray<Node> nodes = TArray<Node>();
	for (AWaypoint* v : wp->GetAvailableWaypoints()) {
		Node u;
		u.waypoint = v;
		nodes.Add(u);
	}
	return nodes;
}

// return -1 si false et le coût si true
int Contains(TArray<AGuard::Node> array, AGuard::Node n) {
	for (AGuard::Node v : array) {
		if (v.waypoint == n.waypoint) {
			return v.cost;
		}
	}
	return -1;
}

void AGuard::CalculatePath(AWaypoint* start, AWaypoint* goal) {
	Node startNode = { start, 0, 0 };
	Node goalNode = { goal, 0, 0 };
	TArray<Node> ClosedLists = TArray<Node>(); // heap
	TArray<Node> OpenList = TArray<Node>(); // heap
	TMap<AWaypoint*, AWaypoint*> CameFrom = TMap<AWaypoint*, AWaypoint*>(); // map of waypoint indexes
	OpenList.HeapPush(startNode, NodePredicate());
	while (!OpenList.IsEmpty()) {
		Node u;
		OpenList.HeapPop(u, NodePredicate());
		if (u.waypoint == goal) {
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("camefrom %f"), CameFrom.Num()));
			Path = reconstruct_path(CameFrom, u.waypoint);
			return;
		}
		//pour chaque voisin v de u dans g
		for (Node v : GetAvailableNodes(Cast<AWaypoint>(u.waypoint))) {
			// not (v existe dans closedLists ou v existe dans openList avec un coût inférieur)
			if (!(Contains(ClosedLists, v) != -1 || ((Contains(OpenList, v) != -1) && Contains(OpenList, v) < v.cost))) {
				//D(" not (v existe dans closedLists ou v existe dans openList avec un coût inférieur)");
				CameFrom.Add(v.waypoint, u.waypoint); // not working
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("camefrom %f"), CameFrom.Num()));
				v.cost = u.cost + 1;
				v.heuristique = v.cost + FVector::Dist((v.waypoint)->GetActorLocation(), goal->GetActorLocation());
				OpenList.HeapPush(v, NodePredicate());
			}
			ClosedLists.HeapPush(v, NodePredicate());
		}
	}
	D("erreur calculate");
	// terminer le programme (avec erreur)
}

//int AGuard::MinCost(const TArray<AWaypoint*> waypoints, AWaypoint* waypoint, const TArray<int> visited) {
//	// Initialize min value
//	int min = INT_MAX, min_index = -1;
//	int dist;
//	for (int i = 0; i < waypoints.Num(); i++) {
//		dist = FVector::Dist(waypoints[i]->GetActorLocation(), waypoint->GetActorLocation());
//		if (dist <= min && !visited.Contains(i))
//			min = dist, min_index = i;
//	}
//
//	return min_index;
//}

void AGuard::OnePoint() {
	
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("path size %f"), Path.Num()));*/

	if (!hasPath) {
		target =StartWaypoint->GetActorLocation();
	}

	target.Z = GetActorLocation().Z; //ignore Z axis by setting at the same value as guard

	if (target.Equals(GetActorLocation(), 5.0f)) {
		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Your"));
		if (Path.IsValidIndex(CurrentGraphPoint+1)) {
			D(" Message");
			target = Path[++CurrentGraphPoint]->GetActorLocation();
		}
	}

	if (Path.IsValidIndex(CurrentGraphPoint) && !hasPath)
		StartWaypoint = Path[CurrentGraphPoint];

	if (!hasPath) {
		if (GameInstance->GetOnePoint()) {
			D(" B4 calculate");
			CalculatePath(StartWaypoint, GameInstance->GetOnePoint());
			hasPath = true;
		}
	}
	if (Path.IsValidIndex(CurrentGraphPoint)) {
		if (Path[CurrentGraphPoint] != GameInstance->GetOnePoint())
			Seek();
		else {
			Arrival();
		}
	}
}