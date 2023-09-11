// Copyright William Inman,, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BitSurvival/Interfaces/Interactable.h"
#include "BitSurvival/Items/Item.h"
#include "GameFramework/Actor.h"
#include "Crate.generated.h"

UCLASS()
class BITSURVIVAL_API ACrate : public AItem
{
	GENERATED_BODY()
	
protected:	
	ACrate();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual bool CanInteract(ABitCharacter* _Instigator) override;
	virtual void Interact(class ABitCharacter* _Instigator) override;
	virtual void StopInteract(ABitCharacter* _Instigator) override;
protected:	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;
};
