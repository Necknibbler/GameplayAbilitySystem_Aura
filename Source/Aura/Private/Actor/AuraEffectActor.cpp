// Copyright Message


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"



AAuraEffectActor::AAuraEffectActor()
{

	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}




void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect>& GameplayEffectClass, EEffectRemovalPolicy RemovalPolicy)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	for (const auto& InstantEffect : InstantGameplayEffects)
	{
		if (InstantEffect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, InstantEffect.EffectClass, EEffectRemovalPolicy::DoNotRemove);
		}
	}

	for (const auto& DurationEffect : DurationGameplayEffects)
	{
		if (DurationEffect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, DurationEffect.EffectClass, EEffectRemovalPolicy::DoNotRemove);
		}
	}

	for (const auto& InfiniteEffect : InfiniteGameplayEffects)
	{
		if (InfiniteEffect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, InfiniteEffect.EffectClass, InfiniteEffect.RemovalPolicy);
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	for (const auto& InstantEffect : InstantGameplayEffects)
	{
		if (InstantEffect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, InstantEffect.EffectClass, EEffectRemovalPolicy::DoNotRemove);
		}
	}

	for (const auto& DurationEffect : DurationGameplayEffects)
	{
		if (DurationEffect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, DurationEffect.EffectClass, EEffectRemovalPolicy::DoNotRemove);
		}
	}

	for (const auto& InfiniteEffect : InfiniteGameplayEffects)
	{
		if (InfiniteEffect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, InfiniteEffect.EffectClass, InfiniteEffect.RemovalPolicy);
		}
	}

	for (const auto& InfiniteEffect : InfiniteGameplayEffects)
	{
		if (InfiniteEffect.RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
			if (!IsValid(TargetASC)) continue;

			TArray<FActiveGameplayEffectHandle> HandlesToRemove;
			for (auto HandlePair : ActiveEffectHandles)
			{
				if (TargetASC == HandlePair.Value)
				{
					TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
					HandlesToRemove.Add(HandlePair.Key);
				}
			}
			for (auto& Handle : HandlesToRemove)
			{
				ActiveEffectHandles.FindAndRemoveChecked(Handle);
			}
		}
	}
}




