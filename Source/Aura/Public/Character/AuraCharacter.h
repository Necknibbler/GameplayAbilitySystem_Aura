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

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void AdjustSpringArmLength(float const NewLength) override;
	virtual void ResetSpringArm() override;
	virtual float GetSpringArmLength() override;

	/** Combat Interface **/
	virtual int32 GetPlayerLevel() override;
	/** end Combat Interface **/
private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ViewCamera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float SpringArmLength = 750.f;

	virtual void InitAbilityActorInfo() override;
};
