// Copyright William Inman,, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_Item.generated.h"

UCLASS()
class BITSURVIVAL_API UDA_Item : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Encumbrance{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* Icon;
};

