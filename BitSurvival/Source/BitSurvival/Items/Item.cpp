// Copyright William Inman,, All Rights Reserved

#include "Item.h"

#include "BitSurvival/Characters/BitCharacter.h"
#include "BitSurvival/Components/ItemComponent.h"
#include "BitSurvival/DataAssets/DA_Item.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCustomDepthStencilValue(1);

	ItemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("Item Component"));

	static ConstructorHelpers::FObjectFinder<UMaterial> GhostMaterial(TEXT("/Game/Materials/M_Ghost"));
	if (IsValid(GhostMaterial.Object))
		GhostMat = GhostMaterial.Object;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	OldMaterials = Mesh->GetMaterials();

	if (ItemComponent->bIsPlaced)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetSimulatePhysics(true);
	}
	Mesh->SetCollisionResponseToChannel(ECC_InteractionChannel, ECR_Block);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AItem::CanInteract(ABitCharacter* _Instigator)
{
	if (_Instigator->CanCarryItem(this))
		return true;
	
	return false;
}

void AItem::StartInteract(ABitCharacter* _Instigator)
{
	Mesh->SetRenderCustomDepth(true);
}

void AItem::Interact(ABitCharacter* _Instigator)
{
	_Instigator->PickupItem(this);
}

void AItem::StopInteract(ABitCharacter* _Instigator)
{
	Mesh->SetRenderCustomDepth(false);
}

void AItem::ToggleGhostMaterial(bool _GhostMaterial)
{
	if (_GhostMaterial)
	{
		for(int i = 0 ; i < Mesh->GetNumMaterials(); i++)
			Mesh->SetMaterial(i, GhostMat);
	}
	else
	{
		for(int i = 0 ; i < Mesh->GetNumMaterials(); i++)
			Mesh->SetMaterial(i, OldMaterials[i]);
	}
}

