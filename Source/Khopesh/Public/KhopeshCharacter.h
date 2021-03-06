// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KhopeshCharacter.generated.h"

enum class EMontage : uint8;

UCLASS(config=Game)
class AKhopeshCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* LeftWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* RightWeapon;

public:
	// Constructor
	AKhopeshCharacter();

private:
	// Virtual Function
	virtual void BeginPlay() override;
	virtual void Tick(float DelatSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Binding Function (Excluding dodge bind function because it include RPC)
	void MoveForward(float Value);
	void MoveRight(float Value);

	void Attack();
	void Defense();

	void OnPressDodge();
	void OnReleaseDodge();

	void OnAttack();
	void SetCombat(bool IsEquip);

	// RPC Function Declaration
	UFUNCTION(Server, Reliable, WithValidation)
	void Attack_Request(FRotator NewRotation);

	UFUNCTION(NetMulticast, Reliable)
	void Attack_Response(EMontage Montage, FName Section, FRotator NewRotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void Defense_Request(FRotator NewRotation);

	UFUNCTION(NetMulticast, Reliable)
	void Defense_Response(FRotator NewRotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void Dodge_Request(FRotator NewRotation, bool IsLongDodge);

	UFUNCTION(NetMulticast, Reliable)
	void Dodge_Response(FRotator NewRotation, bool IsLongDodge);

	UFUNCTION(Client, Reliable)
	void ShowCombatEffect();

	UFUNCTION(Client, Reliable)
	void ShowHitEffect();

	UFUNCTION(Client, Reliable)
	void ShowParryingEffect();

	UFUNCTION(Client, Reliable)
	void ApplyEnemyHP(float HP);

	UFUNCTION(NetMulticast, Reliable)
	void PlayHitSound();

	UFUNCTION(NetMulticast, Reliable)
	void PlayHitMontage(EMontage Montage);

	UFUNCTION(NetMulticast, Reliable)
	void EndDefenseMontage(bool IsSuccess, FRotator Rotator = FRotator());

	UFUNCTION(NetMulticast, Reliable)
	void PlayBroken();

	UFUNCTION(NetMulticast, Reliable)
	void PlayEquip(bool IsEquip);

	UFUNCTION(NetMulticast, Reliable)
	void SetWeapon(bool IsEquip);

	UFUNCTION(NetMulticast, Reliable)
	void PlayDie();

private:
	// RPC Function Implementation
	void Attack_Request_Implementation(FRotator NewRotation);
	bool Attack_Request_Validate(FRotator NewRotation);
	void Attack_Response_Implementation(EMontage Montage, FName Section, FRotator NewRotation);

	void Defense_Request_Implementation(FRotator NewRotation);
	bool Defense_Request_Validate(FRotator NewRotation);
	void Defense_Response_Implementation(FRotator NewRotation);

	void Dodge_Request_Implementation(FRotator NewRotation, bool IsLongDodge);
	bool Dodge_Request_Validate(FRotator NewRotation, bool IsLongDodge);
	void Dodge_Response_Implementation(FRotator NewRotation, bool IsLongDodge);

	void ShowCombatEffect_Implementation();
	void ShowHitEffect_Implementation();
	void ShowParryingEffect_Implementation();

	void PlayHitSound_Implementation();
	void ApplyEnemyHP_Implementation(float HP);

	void PlayHitMontage_Implementation(EMontage Montage);
	void EndDefenseMontage_Implementation(bool IsSuccess, FRotator Rotator);
	void PlayBroken_Implementation();
	void PlayEquip_Implementation(bool IsEquip);
	void SetWeapon_Implementation(bool IsEquip);

	void PlayDie_Implementation();

protected:
	// Blueprint Function
	UFUNCTION(BlueprintImplementableEvent)
	void OnShowCombatEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void OnShowHitEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void OnShowParryingEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void OnApplyEnemyHP(float HP);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayHitSound();

private:
	// Other Function
	void Move(EAxis::Type Axis, float Value);
	void Break(AKhopeshCharacter* Target);
	void Die();

	bool CanDodge() const;
	bool IsEnemyNear() const;
	FRotator GetRotationByAim() const;
	FRotator GetRotationByInputKey() const;
	EMontage GetHitMontageByDir(float Dir) const;

private:
	// Animation Instance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = true))
	class UKhopeshAnimInstance* Anim;

	// Blueprint Property
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Replicated, Meta = (AllowPrivateAccess = true))
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CombatSwapRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float WeakAttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float StrongAttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	uint8 MaxCombo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float ComboDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float DefenseDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float BrokenDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float DodgeDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed, Meta = (AllowPrivateAccess = true))
	float ReadySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed, Meta = (AllowPrivateAccess = true))
	float FightSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed, Meta = (AllowPrivateAccess = true))
	float SpeedRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodge, Meta = (AllowPrivateAccess = true))
	float DodgeReinforceDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KnockBack, Meta = (AllowPrivateAccess = true))
	float HitKnockBackImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HitNum, Meta = (AllowPrivateAccess = true))
	TArray<uint8> WeakAttackHitNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HitNum, Meta = (AllowPrivateAccess = true))
	TArray<uint8> StrongAttackHitNum;

	// Replicated Property (HP exclude here. Because it include Blueprint Property.)
	UPROPERTY(Replicated)
	float Speed;

	UPROPERTY(Replicated)
	bool IsCombatMode;

	// Other Variable
	FTimerHandle ComboTimer, DefenseTimer, BrokenTimer, DodgeTimer;
	uint8 CurrentCombo;
	float BrokenPlayRate;
	float NextDodgeTime;

	// Flag Variable
		// Server
	bool IsStrongMode;
	bool IsStartCombat;
	bool IsDefensing;

		// Owner
	bool IsReadyDodge;
};