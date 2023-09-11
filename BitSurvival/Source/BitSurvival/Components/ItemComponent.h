// Copyright William Inman,, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BITSURVIVAL_API UItemComponent : public UActorComponent
{
	friend class UInventoryComponent;
	friend class AItem;
	friend class ACrate;
	friend class ABitCharacter;
	friend class UW_Inventory;
	GENERATED_BODY()
protected:
	UItemComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
protected:
	UPROPERTY(EditAnywhere)
	class UDA_Item* Data;

	UPROPERTY(EditAnywhere)
	bool bIsPlaced{true};
};
