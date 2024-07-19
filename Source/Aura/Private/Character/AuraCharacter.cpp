// Copyright Message


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


AAuraCharacter::AAuraCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = SpringArmLength;
	CameraBoom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
}

void AAuraCharacter::AdjustSpringArmLength(float NewLength)
{
	if (CameraBoom)
	{
		CameraBoom->TargetArmLength = NewLength;
	}
}

void AAuraCharacter::ResetSpringArm()
{
	if (CameraBoom)
	{
		CameraBoom->TargetArmLength = SpringArmLength;
	}
}

float AAuraCharacter::GetSpringArmLength()
{
	if (CameraBoom)
	{
		return CameraBoom->TargetArmLength;
	}
	return 0.0f;
}
