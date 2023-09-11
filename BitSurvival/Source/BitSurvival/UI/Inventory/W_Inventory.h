// Copyright William Inman,, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/Reply.h"
#include "W_Inventory.generated.h"

UCLASS()
class BITSURVIVAL_API UW_Inventory : public UUserWidget
{
	friend class AHUD_Game;
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
protected:
	void PopulateFromInventory(class UInventoryComponent* _Inventory);
	
	UFUNCTION(BlueprintCallable)
	void CloseWindow();

	void CenterViewportCursor(const APlayerController* PlayerController);

	UPROPERTY(VisibleAnywhere)
	class UInventoryComponent* AttachedInventory;
	
	UPROPERTY(VisibleAnywhere)
	TArray<class UW_ItemSlot*>  SlotsArray;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UGridPanel* MainGrid;
};
