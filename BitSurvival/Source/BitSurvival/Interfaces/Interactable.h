// Copyright William Inman,, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

#define ECC_InteractionChannel ECC_GameTraceChannel1

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
	// This class does not need to be modified.
};

class BITSURVIVAL_API IInteractable
{
	GENERATED_BODY()
public:
	virtual bool CanInteract(class ABitCharacter* _Instigator) { return true; }
	virtual void StartInteract(class ABitCharacter* _Instigator){}
	virtual void Interact(class ABitCharacter* _Instigator) = 0;
	virtual void StopInteract(class ABitCharacter* _Instigator){}
	
};
