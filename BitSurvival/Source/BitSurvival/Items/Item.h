// Copyright William Inman,, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BitSurvival/Interfaces/Interactable.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class BITSURVIVAL_API AItem : public AActor, public IInteractable
{
	friend class ABitCharacter;
	friend class UW_Inventory;
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
public:
	virtual bool CanInteract(ABitCharacter* _Instigator) override;
	virtual void StartInteract(ABitCharacter* _Instigator) override;
	virtual void Interact(ABitCharacter* _Instigator) override;
	virtual void StopInteract(ABitCharacter* _Instigator) override;
protected:
	UFUNCTION()
	void ToggleGhostMaterial(bool _GhostMaterial);
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere)
	class UItemComponent* ItemComponent;

	UPROPERTY(VisibleAnywhere)
	class UMaterial* GhostMat;

	UPROPERTY(VisibleAnywhere)
	TArray<UMaterialInterface*> OldMaterials;
};
