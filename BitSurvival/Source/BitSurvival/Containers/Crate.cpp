// Copyright William Inman,, All Rights Reserved


#include "Crate.h"

#include "BitSurvival/Components/InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "BitSurvival/Characters/BitCharacter.h"
#include "BitSurvival/Components/ItemComponent.h"

ACrate::ACrate()
{
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
}

void ACrate::BeginPlay()
{
	Super::BeginPlay();
}

void ACrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ACrate::CanInteract(ABitCharacter* _Instigator)
{
	return true;
}

void ACrate::Interact(ABitCharacter* _Instigator)
{
	if(ItemComponent->bIsPlaced)
	{
		_Instigator->SpawnInventoryWidget(InventoryComponent);
		_Instigator->SpawnInventoryWidget();
	}
	else
	{
		Super::Interact(_Instigator);
	}
}

void ACrate::StopInteract(ABitCharacter* _Instigator)
{
	Super::StopInteract(_Instigator);
	_Instigator->DestroyInventoryWidget();
}

