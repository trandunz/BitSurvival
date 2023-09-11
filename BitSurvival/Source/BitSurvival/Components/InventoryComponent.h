// Copyright William Inman,, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BITSURVIVAL_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class AHUD_Game;
	friend class ACrate;
	friend class ABitCharacter;
	friend class UW_Inventory;
protected:
	UInventoryComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool AddItem(class UItemComponent* _Item);
	bool CanCarryItem(class UItemComponent* _SomeItem);

public:
	UFUNCTION()
	int32 GetTotalWeight() const;
protected:
	UPROPERTY(EditAnywhere)
	int32 MaxWeight{48};
	
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UUserWidget> InventoryWidgetPrefab;
	UPROPERTY(VisibleAnywhere)
	TMap<class UItemComponent*, int16> CurrentlyHeldItems;
};
