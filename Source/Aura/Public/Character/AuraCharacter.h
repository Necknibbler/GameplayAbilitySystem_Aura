// Copyright Message

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraInterface.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public ICameraInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual void AdjustSpringArmLength(float NewLength) override;
	virtual void ResetSpringArm() override;
	virtual float GetSpringArmLength() override;
private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ViewCamera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float SpringArmLength = 750.f;

};
