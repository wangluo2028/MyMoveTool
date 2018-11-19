// Fill out your copyright notice in the Description page of Project Settings.

#include "MyMoveTool.h"
#include "TransformTool.h"


// Sets default values
ATransformTool::ATransformTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MoveInZ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MoveInZComponent"));
	MoveInZ->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	RotationInXY = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotationInXY"));
	RotationInXY->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	HighLightTool = nullptr;
	HighLightToolSrcMat = nullptr;
}

// Called when the game starts or when spawned
void ATransformTool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATransformTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATransformTool::HighLightTransTool(UStaticMeshComponent *InHitToolComp)
{
	if (HighLightTool)
	{
		HighLightTool->SetMaterial(0, HighLightToolSrcMat);
	}

	HighLightTool = InHitToolComp;
	if (HighLightTool)
	{
		HighLightToolSrcMat = HighLightTool->GetMaterial(0);
		HighLightTool->SetMaterial(0, HighLightMat);
	}
}

#include "Kismet/KismetMathLibrary.h"

float Ue4Radian2Normal(float InUe4Radian)
{
	if (InUe4Radian < 0)
	{
		InUe4Radian += UKismetMathLibrary::GetTAU();
	}

	return InUe4Radian;
}

float GetRotationOfVector2D(const FVector2D& InVector)
{
	float HeadingAngle = FMath::Atan2(InVector.Y, InVector.X);
	HeadingAngle = Ue4Radian2Normal(HeadingAngle);

	return HeadingAngle;
}


void ATransformTool::TransformActor(AActor *SelectedActor, FVector LastLocation, FRotator LastRotation, FVector LastPos, FVector LastDir,
		FVector CurrentPos, FVector CurrentDir)
{
	if (!SelectedActor)
	{
		return;
	}

	if (HighLightTool)
	{
		if (HighLightTool == MoveInZ)
		{
			FVector OldLocation = LastLocation;
			FVector HDir = FVector::CrossProduct(FVector::UpVector, LastDir);
			FVector NormalOfPalne = FVector::CrossProduct(HDir, FVector::UpVector);
			FPlane ActorMovePlane(OldLocation, NormalOfPalne);
			FVector LastPosInPlane = FMath::LinePlaneIntersection(LastPos, LastPos + LastDir, ActorMovePlane);
			FVector CurrentPosInPlane = FMath::LinePlaneIntersection(CurrentPos, CurrentPos + CurrentDir, ActorMovePlane);

			FVector DeltaPos = CurrentPosInPlane - LastPosInPlane;
			FVector NewLocation = OldLocation + FVector(0, 0, DeltaPos.Z);
			SelectedActor->SetActorLocation(NewLocation);
		}
		else if (HighLightTool == RotationInXY)
		{
			FVector OldLocation = LastLocation;
			FPlane ActorMovePlane(OldLocation, FVector::UpVector);
			FVector LastPosInPlane = FMath::LinePlaneIntersection(LastPos, LastPos + LastDir, ActorMovePlane);
			FVector CurrentPosInPlane = FMath::LinePlaneIntersection(CurrentPos, CurrentPos + CurrentDir, ActorMovePlane);

			FVector2D LastDirection(LastPosInPlane - OldLocation);
			FVector2D CurrentDirection(CurrentPosInPlane - OldLocation);
			float DeltaAngle = GetRotationOfVector2D(CurrentDirection) - GetRotationOfVector2D(LastDirection);
			SelectedActor->SetActorRotation(LastRotation + FRotator(0, UKismetMathLibrary::RadiansToDegrees(DeltaAngle), 0));
		}
	}
	else
	{
		FVector OldLocation = LastLocation;
		FPlane ActorMovePlane(OldLocation, FVector::UpVector);
		FVector LastPosInPlane = FMath::LinePlaneIntersection(LastPos, LastPos + LastDir, ActorMovePlane);
		FVector CurrentPosInPlane = FMath::LinePlaneIntersection(CurrentPos, CurrentPos + CurrentDir, ActorMovePlane);

		FVector DeltaPos = CurrentPosInPlane - LastPosInPlane;
		
		FVector NewLocation = OldLocation + FVector(DeltaPos.X, DeltaPos.Y, 0);
		SelectedActor->SetActorLocation(NewLocation);
	}

	SetActorLocation(SelectedActor->GetActorLocation());
}

