// Copyright William Inman,, All Rights Reserved

#include "W_Inventory.h"

#include "W_ItemSlot.h"
#include "BitSurvival/Components/InventoryComponent.h"
#include "BitSurvival/Components/ItemComponent.h"
#include "BitSurvival/DataAssets/DA_Item.h"
#include "BitSurvival/Items/Item.h"
#include "Components/Image.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"

void UW_Inventory::NativeConstruct()
{
	Super::NativeConstruct();
}

void UW_Inventory::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UW_Inventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	CenterViewportCursor(GetOwningPlayer());
	
	for(auto SomeSlot : MainGrid->GetAllChildren())
	{
		SlotsArray.Add(Cast<UW_ItemSlot>(SomeSlot));
	}
}

void UW_Inventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	SetFocus();
}

FReply UW_Inventory::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Tab)
		CloseWindow();
	
	return FReply::Handled();
}

void UW_Inventory::PopulateFromInventory(UInventoryComponent* _Inventory)
{
	AttachedInventory = _Inventory;
	
	for(int i = _Inventory->MaxWeight; i > _Inventory->MaxWeight - (_Inventory->GetTotalWeight()  - _Inventory->CurrentlyHeldItems.Num()); i--)
	{
		SlotsArray[i]->Icon->SetOpacity(0.3f);
	}
	for(int i = _Inventory->MaxWeight - (_Inventory->GetTotalWeight() - _Inventory->CurrentlyHeldItems.Num()); i >= 0; i--)
	{
		SlotsArray[i]->Icon->SetOpacity(1.0f);
	}

	int iterator{};
	for (auto Item : _Inventory->CurrentlyHeldItems)
	{
		SlotsArray[iterator]->Icon->SetBrushFromTexture(Item.Key->Data->Icon);
		iterator++;
	}
}

void UW_Inventory::CloseWindow()
{
	GetOwningPlayer()->SetInputMode(FInputModeGameOnly{});
	GetOwningPlayer()->SetShowMouseCursor(false);
	CenterViewportCursor(GetOwningPlayer());
	RemoveFromParent();
	MarkAsGarbage();
}

void UW_Inventory::CenterViewportCursor(const APlayerController* PlayerController)
{
	if( PlayerController )
	{
		const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if( LocalPlayer && LocalPlayer->ViewportClient )
		{
			FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;
			if( Viewport )
			{
				FVector2D ViewportSize;
				LocalPlayer->ViewportClient->GetViewportSize(ViewportSize);
				const int32 X = static_cast<int32>(ViewportSize.X * 0.5f);
				const int32 Y = static_cast<int32>(ViewportSize.Y * 0.5f);

				Viewport->SetMouse(X, Y);
			}
		}
	}
}
