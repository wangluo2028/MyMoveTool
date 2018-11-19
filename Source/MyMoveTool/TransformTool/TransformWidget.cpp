// Fill out your copyright notice in the Description page of Project Settings.

#include "MyMoveTool.h"
#include "TransformWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TransformTool.h"

void UTransformWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FActorSpawnParameters SpawmParams;
	SpawmParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	TransformTool = GetWorld()->SpawnActor<ATransformTool>(SpawmParams);

	PlayerController = GetWorld()->GetFirstPlayerController();
}

FReply UTransformWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UTransformWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FVector MouseWorld, MouseDir;
	PlayerController->DeprojectMousePositionToWorld(MouseWorld, MouseDir);
	FHitResult MouseHitResult;
	PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, MouseHitResult);
	AActor *HitActor = MouseHitResult.GetActor();
	if (HitActor)
	{
		TransformTool->SetActorHiddenInGame(false);
		TransformTool->SetActorLocationAndRotation(HitActor->GetActorLocation(), 
			FRotator(0, 0, 0));
	}
	else
	{
		TransformTool->SetActorHiddenInGame(true);
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}
