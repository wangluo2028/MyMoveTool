// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "TransformWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYMOVETOOL_API UTransformWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	
protected:
	UPROPERTY()
	class ATransformTool *TransformTool;

	UPROPERTY()
	APlayerController *PlayerController;
};
