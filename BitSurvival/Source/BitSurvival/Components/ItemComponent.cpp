// Copyright William Inman,, All Rights Reserved

#include "ItemComponent.h"

UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UItemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

