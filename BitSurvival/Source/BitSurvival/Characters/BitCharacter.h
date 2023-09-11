#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BitCharacter.generated.h"

UCLASS()
class BITSURVIVAL_API ABitCharacter : public ACharacter
{
	GENERATED_BODY()
	friend class AHUD_Game;
protected:
	ABitCharacter();
	void GrabInputActions();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void AttachCameraToArm();
	void Zoom(const struct FInputActionValue& Value);

	void CheckForInteractables();
	void Interact();
public:
	void SpawnInventoryWidget(class UInventoryComponent* _InventoryToInspect = nullptr);
	void DestroyInventoryWidget();

	void PickupItem(class AItem* _SomeItem);

	bool CanCarryItem(class AItem* _SomeItem);

	UFUNCTION(BlueprintCallable)
	bool IsCarryingHeavyItem() const;
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* InventoryAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ZoomAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* MappingContext;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class AHUD_Game* PlayerHUDRef;
	
	UPROPERTY(VisibleAnywhere, Category= Interaction)
	class AActor* ClosestInteractableActor;
	class IInteractable* ClosestInteractable;

	UPROPERTY(VisibleAnywhere)
	class UInventoryComponent* Inventory;

	UPROPERTY(VisibleAnywhere)
	bool bCarryingHeavyItem{};

	UPROPERTY(EditAnywhere)
	float MaxZoom{600.0f};
	UPROPERTY(EditAnywhere)
	float MinZoom{100.0f};
};
