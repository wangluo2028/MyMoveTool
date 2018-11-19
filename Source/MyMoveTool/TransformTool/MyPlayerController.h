// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYMOVETOOL_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;
	
protected:
	void OnLeftMouseButtonDown();

	void OnLeftMouseButtonUp();

	void StartDrag();

	void EndDrag();

protected:
	UPROPERTY(BlueprintReadWrite, Category="TransformTool")
	class ATransformTool *TransformTool;
	UPROPERTY(EditAnywhere, Category = "TransformTool")
	TSubclassOf<class ATransformTool> TransformToolClass;
	
	UPROPERTY()
	AActor *SelectedActor;

	bool bLMouseBtnDown;
	bool bCanStartDrag;
	bool bDragging;

	FVector LastLocation;
	FRotator LastRotation;
	FVector LastMouseWorldPos;
	FVector LastMouseWorldDir;
};
