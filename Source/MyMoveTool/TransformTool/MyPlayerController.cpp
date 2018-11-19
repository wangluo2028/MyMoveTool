// Fill out your copyright notice in the Description page of Project Settings.

#include "MyMoveTool.h"
#include "MyPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TransformTool.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawmParams;
	SpawmParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	TransformTool = GetWorld()->SpawnActor<ATransformTool>(TransformToolClass.Get(), SpawmParams);
	TransformTool->SetActorHiddenInGame(true);

	bLMouseBtnDown = false;
	SelectedActor = nullptr;
	bCanStartDrag = false;
	bDragging = false;
}

void AMyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TransformTool->Destroy();

	Super::EndPlay(EndPlayReason);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("OnLeftMouseButton"), IE_Pressed, this, &AMyPlayerController::OnLeftMouseButtonDown);
	InputComponent->BindAction(TEXT("OnLeftMouseButton"), IE_Released, this, &AMyPlayerController::OnLeftMouseButtonUp);
}

void AMyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bLMouseBtnDown)
	{
		bCanStartDrag = false;
		if (SelectedActor)
		{
			FHitResult MouseHitResult;
			this->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, MouseHitResult);
			AActor *MouseHitActor = MouseHitResult.GetActor();
			UStaticMeshComponent *HitToolStaticMeshComp = nullptr;
			if (TransformTool == MouseHitActor)
			{
				UPrimitiveComponent *MouseHitComponent = MouseHitResult.GetComponent();
				if (MouseHitComponent->IsA(UStaticMeshComponent::StaticClass()))
				{
					HitToolStaticMeshComp = Cast<UStaticMeshComponent>(MouseHitComponent);
				}
			}
			TransformTool->HighLightTransTool(HitToolStaticMeshComp);

			if (HitToolStaticMeshComp || (SelectedActor == MouseHitActor))
			{
				bCanStartDrag = true;
			}
		}

		if (bCanStartDrag)
		{
			CurrentMouseCursor = EMouseCursor::CardinalCross;
		}
		else
		{
			CurrentMouseCursor = EMouseCursor::Default;
		}
	}
	else
	{
		if (bDragging)
		{
			FVector MouseWorld, MouseDir;
			this->DeprojectMousePositionToWorld(MouseWorld, MouseDir);

			TransformTool->TransformActor(SelectedActor, LastLocation, LastRotation, LastMouseWorldPos, LastMouseWorldDir, MouseWorld, MouseDir);

			//LastMouseWorldPos = MouseWorld;
			//LastMouseWorldDir = MouseDir;
		}
	}
}

void AMyPlayerController::OnLeftMouseButtonDown()
{
	bCanStartDrag = false;
	if (SelectedActor)
	{
		FHitResult MouseHitResult;
		this->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, MouseHitResult);
		AActor *MouseHitActor = MouseHitResult.GetActor();
		UStaticMeshComponent *HitToolStaticMeshComp = nullptr;
		if (TransformTool == MouseHitActor)
		{
			UPrimitiveComponent *MouseHitComponent = MouseHitResult.GetComponent();
			if (MouseHitComponent->IsA(UStaticMeshComponent::StaticClass()))
			{
				HitToolStaticMeshComp = Cast<UStaticMeshComponent>(MouseHitComponent);
			}
		}
		TransformTool->HighLightTransTool(HitToolStaticMeshComp);

		if (HitToolStaticMeshComp || (SelectedActor == MouseHitActor))
		{
			bCanStartDrag = true;
		}
	}

	if (bCanStartDrag)
	{
		bDragging = true;
		bCanStartDrag = false;
		this->DeprojectMousePositionToWorld(LastMouseWorldPos, LastMouseWorldDir);
		this->GetPawn()->DisableInput(this);
		LastLocation = SelectedActor->GetActorLocation();
		LastRotation = SelectedActor->GetActorRotation();
	}

	bLMouseBtnDown = true;
}

void AMyPlayerController::OnLeftMouseButtonUp()
{
	if (bDragging)
	{
		bDragging = false;
		this->GetPawn()->EnableInput(this);
	}
	else
	{
		FVector MouseWorld, MouseDir;
		this->DeprojectMousePositionToWorld(MouseWorld, MouseDir);
		FHitResult MouseHitResult;
		this->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, MouseHitResult);
		SelectedActor = MouseHitResult.GetActor();
		if (SelectedActor)
		{
			TransformTool->SetActorHiddenInGame(false);
			TransformTool->SetActorLocationAndRotation(SelectedActor->GetActorLocation(),
				FRotator(0, 0, 0));
		}
		else
		{
			TransformTool->SetActorHiddenInGame(true);
		}
	}

	bLMouseBtnDown = false;
}

void AMyPlayerController::StartDrag()
{

}

void AMyPlayerController::EndDrag()
{

}
