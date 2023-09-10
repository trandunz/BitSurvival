#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BitCharacter.generated.h"

UCLASS()
class BITSURVIVAL_API ABitCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	ABitCharacter();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

public:	


};
