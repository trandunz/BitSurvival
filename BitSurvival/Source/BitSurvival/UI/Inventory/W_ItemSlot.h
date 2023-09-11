// Copyright William Inman,, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_ItemSlot.generated.h"

UCLASS()
class BITSURVIVAL_API UW_ItemSlot : public UUserWidget
{
	friend class UW_Inventory;
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class ABitCharacter* OwningCharacter;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UImage* Icon;
};
