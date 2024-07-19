// Copyright Message


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Camera/CameraInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check (Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	EnhancedInputComponent->BindAction(ZoomInAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::ZoomIn);
	EnhancedInputComponent->BindAction(ZoomOutAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::ZoomOut);
	EnhancedInputComponent->BindAction(ResetSpringArmAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::ResetSpringArmLength);

	
}

void AAuraPlayerController::ZoomIn()
{
	AdjustSpringArmLength(-ZoomStep);
}

void AAuraPlayerController::ZoomOut()
{
	AdjustSpringArmLength(ZoomStep);
}

void AAuraPlayerController::ResetSpringArmLength()
{
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn && ControlledPawn->GetClass()->ImplementsInterface(UCameraInterface::StaticClass()))
	{
		if (ICameraInterface* CameraPawn = Cast<ICameraInterface>(ControlledPawn))
		{
			if (CameraPawn)
			{
				CameraPawn->ResetSpringArm();
			}
		}
	}
}

void AAuraPlayerController::AdjustSpringArmLength(float DeltaLength)
{
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn && ControlledPawn->GetClass()->ImplementsInterface(UCameraInterface::StaticClass()))
	{
		if (ICameraInterface* CameraPawn = Cast<ICameraInterface>(ControlledPawn))
		{
			if (CameraPawn)
			{
				// Get current spring arm length and adjust
				float CurrentLength = CameraPawn->GetSpringArmLength();
				float NewLength = FMath::Clamp(CurrentLength + DeltaLength, MinZoomLength, MaxZoomLength);
				CameraPawn->AdjustSpringArmLength(NewLength);
			}
		}
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);

	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	 * Line trace from cursor. There are several scenarios:
	 * A. LastActor is null && ThisActor is null
	 *		- Do nothing.
	 * B. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor
	 * C. LastACtor is valid && ThisActor is null
	 *		- UnHighlight LastActor
	 * D. Both actors are valid, but LastActor != ThisActor
	 *		- UnHighlight LastActor, and Highlight ThisActor
	 * E. Both actors are valid, and are the same actor
	 *		- Do nothing
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActor();
		}
		else
		{
			// Case A - both are null, do nothing
		}
	}
	else //LastActor is valid
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActor();
		}
		else // both actors are valid
		{
			if (LastActor != ThisActor)
			{
				//Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case E - Do nothing
			}
		}
	}
}
