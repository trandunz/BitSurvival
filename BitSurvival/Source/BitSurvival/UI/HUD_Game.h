// Copyright William Inman,, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_Game.generated.h"

UCLASS()
class BITSURVIVAL_API AHUD_Game : public AHUD
{
	friend class ABitCharacter;
	GENERATED_BODY()

protected:
	AHUD_Game();
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;
	void SpawnInventoryWidget(class UInventoryComponent* _InventoryToInspect, FVector2D _Offset = FVector2D::Zero());
	void DestroyInventoryWidget();
protected:
	UPROPERTY(VisibleAnywhere)
	class UTexture2D* CrosshairTexture;

	UPROPERTY(VisibleAnywhere, Category = "Main Widget")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Main Widget")
	class UUserWidget* CurrentWidget;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	class UW_Inventory* CurrentInventoryWidget;

	UPROPERTY(VisibleAnywhere)
	FVector2D CanvasCenter;
};
