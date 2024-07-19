// Copyright Message

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraInterface.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class IEnemyInterface;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void ZoomIn();
	void ZoomOut();
	void ResetSpringArmLength();
private:

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ZoomInAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ZoomOutAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ResetSpringArmAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	float ZoomStep = 10.f;

	UPROPERTY(EditAnywhere, Category = "Zoom")
	float MinZoomLength = 300.0f;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float MaxZoomLength = 1500.0f;
	
	void AdjustSpringArmLength(float DeltaLength);

	void Move(const struct FInputActionValue& InputActionValue);
	void CursorTrace();

	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;
	
};
