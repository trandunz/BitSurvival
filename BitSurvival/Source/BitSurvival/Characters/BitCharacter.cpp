#include "BitCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "BitSurvival/Components/InventoryComponent.h"
#include "BitSurvival/Components/ItemComponent.h"
#include "BitSurvival/DataAssets/DA_Item.h"
#include "BitSurvival/Interfaces/Interactable.h"
#include "BitSurvival/Items/Item.h"
#include "BitSurvival/UI/HUD_Game.h"
#include "BitSurvival/UI/Inventory/W_Inventory.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ABitCharacter::ABitCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MannyMesh(TEXT("/Game/Mannequins/Meshes/SKM_Manny"));
	if (IsValid(MannyMesh.Object))
		GetMesh()->SetSkeletalMesh(MannyMesh.Object);

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	CameraArm->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("neck_01"));
	CameraArm->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	CameraArm->TargetArmLength = 400.0f;	
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	Camera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("head"));

	CameraArm->bUsePawnControlRotation = true;
	Camera->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));

	GrabInputActions();
}

void ABitCharacter::GrabInputActions()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> FoundLookAction(TEXT("/Game/Input/Binds/IA_Look"));
	if (IsValid(FoundLookAction.Object))
		LookAction = FoundLookAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> FoundMoveAction(TEXT("/Game/Input/Binds/IA_Move"));
	if (IsValid(FoundMoveAction.Object))
		MoveAction = FoundMoveAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> FoundJumpAction(TEXT("/Game/Input/Binds/IA_Jump"));
	if (IsValid(FoundJumpAction.Object))
		JumpAction = FoundJumpAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> FoundInteractAction(TEXT("/Game/Input/Binds/IA_Interact"));
	if (IsValid(FoundInteractAction.Object))
		InteractAction = FoundInteractAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> FoundInventoryAction(TEXT("/Game/Input/Binds/IA_Inventory"));
	if (IsValid(FoundInventoryAction.Object))
		InventoryAction = FoundInventoryAction.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> FoundZoomAction(TEXT("/Game/Input/Binds/IA_Zoom"));
	if (IsValid(FoundZoomAction.Object))
		ZoomAction = FoundZoomAction.Object;
}

void ABitCharacter::BeginPlay()
{
	Super::BeginPlay();

	Camera->SetupAttachment(GetMesh(), FName("head"));

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void ABitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABitCharacter::Look);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABitCharacter::Move);
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ABitCharacter::Interact);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, &ABitCharacter::SpawnInventoryWidget, Inventory);

		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ABitCharacter::Zoom);
	}
}

void ABitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		CheckForInteractables();
	}
}

void ABitCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (IsValid(Controller))
	{
		if (Camera->GetAttachParent() == CameraArm)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
		else
		{
			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), MovementVector.X);
		}
	}
}

void ABitCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (IsValid(Controller))
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABitCharacter::AttachCameraToArm()
{
	Camera->AttachToComponent(CameraArm, FAttachmentTransformRules::SnapToTargetNotIncludingScale, USpringArmComponent::SocketName);
}

void ABitCharacter::Zoom(const FInputActionValue& Value)
{
	const float ZoomAxis = Value.Get<float>();
	if (IsValid(Controller))
	{
		if (ZoomAxis > 0 && CameraArm->TargetArmLength < MaxZoom)
			CameraArm->TargetArmLength += ZoomAxis * 10.0f;
		else if (ZoomAxis < 0 && CameraArm->TargetArmLength >= MinZoom)
			CameraArm->TargetArmLength += ZoomAxis * 10.0f;

		CameraArm->TargetArmLength = FMath::Clamp(CameraArm->TargetArmLength, 0, MaxZoom);
	}

	if (CameraArm->TargetArmLength > MinZoom)
	{
		AttachCameraToArm();
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		CameraArm->bUsePawnControlRotation = true;
		Camera->bUsePawnControlRotation = false;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		Camera->bUsePawnControlRotation = true;
		Camera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("head"));
		Camera->SetRelativeLocation(FVector(5.000000,13.000000,0.000000));
	}
}

void ABitCharacter::CheckForInteractables()
{
	if (IsCarryingHeavyItem())
		return;
	
	TArray<FHitResult> HitResults;
	FVector Start = Camera->GetComponentLocation();
	FVector End = Camera->GetComponentLocation() + (Camera->GetForwardVector() * 200.0f);
	// If third person, cast from neck
	if (Camera->GetAttachParent() == CameraArm)
	{
		Start = GetMesh()->GetSocketLocation(FName("neck_01"));
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(Rotation.Pitch, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		End = Start + (ForwardDirection * 200.0f);
	}
	if (GetWorld()->LineTraceMultiByChannel(HitResults, Start, End,  ECC_InteractionChannel))
	{
		TArray<AActor*> HitInteractableActors;
		for (FHitResult& HitResult : HitResults)
		{
			if (IInteractable* Interactable = Cast<IInteractable>(HitResult.GetActor()))
			{
				if (Interactable->CanInteract(this))
					HitInteractableActors.Add(HitResult.GetActor());
			}
		}

		float ClosestInteractableDistance;
		AActor* NewInteractableActor = UGameplayStatics::FindNearestActor(GetActorLocation(), HitInteractableActors, ClosestInteractableDistance);
		if(NewInteractableActor != ClosestInteractableActor)
		{
			if (ClosestInteractable != nullptr)
				ClosestInteractable->StopInteract(this);
			
			ClosestInteractableActor = NewInteractableActor;
			ClosestInteractable = Cast<IInteractable>(NewInteractableActor);
			ClosestInteractable->StartInteract(this);
		}
	}
	else
	{
		if (ClosestInteractable != nullptr)
			ClosestInteractable->StopInteract(this);

		ClosestInteractableActor = nullptr;
		ClosestInteractable = nullptr;
	}
}

void ABitCharacter::Interact()
{
	if (ClosestInteractable)
		ClosestInteractable->Interact(this);
}

void ABitCharacter::SpawnInventoryWidget(UInventoryComponent* _InventoryToInspect)
{
	if (PlayerHUDRef)
	{
		PlayerHUDRef->SpawnInventoryWidget(_InventoryToInspect);
	}
}

void ABitCharacter::DestroyInventoryWidget()
{
	if (PlayerHUDRef)
	{
		PlayerHUDRef->DestroyInventoryWidget();
	}
}

void ABitCharacter::PickupItem(AItem* _SomeItem)
{
	if (Inventory->AddItem(_SomeItem->ItemComponent))
	{
		_SomeItem->Mesh->SetSimulatePhysics(false);
		_SomeItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r"));
		_SomeItem->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		_SomeItem->Mesh->SetWorldScale3D({0.4375f, 0.4375f, 0.4375f});
		_SomeItem->Mesh->SetRelativeLocation({-15.069238f, 27.035471f, -5.658631f});
		_SomeItem->Mesh->SetRelativeRotation(FRotator::ZeroRotator);
		_SomeItem->Mesh->SetRenderCustomDepth(false);
		//_SomeItem->ToggleGhostMaterial(true);
		if (_SomeItem->ItemComponent->Data->Encumbrance >= 5.0f)
		{
			bCarryingHeavyItem = true;
		}
		else
		{
			bCarryingHeavyItem = false;
		}
	}
}

bool ABitCharacter::CanCarryItem(AItem* _SomeItem)
{
	return Inventory->CanCarryItem(_SomeItem->ItemComponent) && _SomeItem->ItemComponent->Data->Encumbrance < 5.0;
}

bool ABitCharacter::IsCarryingHeavyItem() const
{
	return bCarryingHeavyItem;
}



