// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TransformTool.generated.h"

UCLASS()
class MYMOVETOOL_API ATransformTool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATransformTool();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HighLightTransTool(UStaticMeshComponent *InHitToolComp = nullptr);

	void TransformActor(AActor *SelectedActor, FVector LastLocation, FRotator LastRotation, FVector LastPos, FVector LastDir,
		FVector CurrentPos, FVector CurrentDir);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transform")
	UMaterialInterface *HighLightMat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Transform")
	UStaticMeshComponent *MoveInZ;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transform")
	UStaticMeshComponent *RotationInXY;

	UPROPERTY()
	UStaticMeshComponent *HighLightTool;
	UPROPERTY()
	UMaterialInterface *HighLightToolSrcMat;
};
