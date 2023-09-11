// Copyright William Inman,, All Rights Reserved


#include "InventoryComponent.h"

#include "ItemComponent.h"
#include "BitSurvival/Characters/BitCharacter.h"
#include "BitSurvival/DataAssets/DA_Item.h"
#include "BitSurvival/UI/Inventory/W_Inventory.h"
#include "Blueprint/UserWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	static ConstructorHelpers::FClassFinder<UUserWidget> InventoryWidget(TEXT("/Game/UI/Inventory/WBP_Inventory"));
	if (IsValid(InventoryWidget.Class))
		InventoryWidgetPrefab = InventoryWidget.Class;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UInventoryComponent::AddItem(UItemComponent* _Item)
{
	if (GetTotalWeight() + _Item->Data->Encumbrance > MaxWeight)
		return false;

	if (_Item->Data->Encumbrance >= 5.0f)
	{
		CurrentlyHeldItems.Add(_Item, 1);
	}
	if (auto StackNumber = CurrentlyHeldItems.FindRef(_Item)) // REPLACE WITH WORKING FIND FUNCTION
	{
		CurrentlyHeldItems[_Item] = (StackNumber + 1);
	}
	else
	{
		CurrentlyHeldItems.Add(_Item, 1);
	}

	return true;
}

bool UInventoryComponent::CanCarryItem(UItemComponent* _SomeItem)
{
	if (GetTotalWeight() + _SomeItem->Data->Encumbrance > MaxWeight)
		return false;

	return true;
}

int32 UInventoryComponent::GetTotalWeight() const
{
	if (CurrentlyHeldItems.Num() <= 0)
		return 0;
	
	int32 TotalWeight{};
	for(auto Item : CurrentlyHeldItems)
	{
		for(int i = 0; i < Item.Value; i++)
		{
			TotalWeight += Item.Key->Data->Encumbrance;
		}
	}
	return TotalWeight;
}

