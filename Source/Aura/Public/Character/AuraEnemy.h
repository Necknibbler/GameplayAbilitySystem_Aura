// Copyright Message

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	/** Enemy Interface Begin */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** Enemy Interface End */
	
	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

protected:
	virtual void BeginPlay() override;
	
};
