// Copyright Message

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CameraInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCameraInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICameraInterface
{
	GENERATED_BODY()

	
public:
	virtual void AdjustSpringArmLength(float NewLength) = 0;
	virtual void ResetSpringArm() = 0;
	virtual float GetSpringArmLength() = 0;
};
