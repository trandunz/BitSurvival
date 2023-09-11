// Copyright William Inman,, All Rights Reserved

#include "HUD_Game.h"

#include "CanvasItem.h"
#include "BitSurvival/Characters/BitCharacter.h"
#include "BitSurvival/Components/InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "Inventory/W_Inventory.h"


AHUD_Game::AHUD_Game()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/Textures/Crosshair"));
	if (IsValid(CrosshairTexObj.Object))
		CrosshairTexture = CrosshairTexObj.Object;


}

void AHUD_Game::BeginPlay()
{
	Super::BeginPlay();
	Cast<ABitCharacter>(GetOwningPawn())->PlayerHUDRef = this;
}

void AHUD_Game::DrawHUD()
{
	Super::DrawHUD();

	CanvasCenter = FVector2D(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	
	const FVector2D CrosshairDrawPosition( CanvasCenter.X - 25,
										   CanvasCenter.Y - 25);
	
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTexture->GetResource(), FLinearColor::White);
	TileItem.Size = {50.0f, 50.0f};
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}

void AHUD_Game::SpawnInventoryWidget(class UInventoryComponent* _InventoryToInspect, FVector2D _Offset)
{
	if (_InventoryToInspect && _InventoryToInspect->InventoryWidgetPrefab)
	{
		CurrentInventoryWidget = CreateWidget<UW_Inventory>(GetOwningPlayerController(), _InventoryToInspect->InventoryWidgetPrefab, FName("Inventory Widget"));
		CurrentInventoryWidget->AddToViewport();
		CurrentInventoryWidget->SetPositionInViewport(CanvasCenter + (FVector2D{_Offset.X, -_Offset.Y} + FVector2D(-125, -175)));
		GetOwningPlayerController()->SetInputMode(FInputModeUIOnly{});
		GetOwningPlayerController()->SetShowMouseCursor(true);
		CurrentInventoryWidget->SetFocus();
		CurrentInventoryWidget->PopulateFromInventory(_InventoryToInspect);
	}
}

void AHUD_Game::DestroyInventoryWidget()
{
	if (CurrentInventoryWidget)
	{
		CurrentInventoryWidget->CloseWindow();
	}
		
}

