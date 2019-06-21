// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KhopeshAnimInstance.generated.h"

UCLASS()
class KHOPESH_API UKhopeshAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UKhopeshAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void SetFightMode(bool IsFight);
	void PlayMontage(UAnimMontage* Montage);
	bool PlayMontageUnique(UAnimMontage* Montage);
	void PlayMontageEquip(UAnimMontage* EquipMontage);

private:
	UFUNCTION()
	void AnimNotify_Equip();

	UFUNCTION()
	void AnimNotify_Unequip();

	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsFightMode;

	UAnimMontage* EquipAnim;
};
