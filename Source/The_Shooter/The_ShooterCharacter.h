// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Animation/AnimMontage.h"
#include "Public/Weapon/WeaponBase.h"
#include "Components/TimelineComponent.h"
#include "The_ShooterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;
class UAnimationAsset;
class AActor;
class UCurveFloat;
class UTimelineComponent;
struct FInputActionValue;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AThe_ShooterCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Pistol Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PistolAction;

	/** Rifle Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RifleAction;

	/** Aim Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	/*Valiables*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables, meta = (AllowPrivateAccess = "true"))
	bool bisCrouch;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables, meta = (AllowPrivateAccess = "true"))
	bool bisSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables, meta = (AllowPrivateAccess = "true"))
	bool bisAim;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables, meta = (AllowPrivateAccess = "true"))
	bool bisJump;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables, meta = (AllowPrivateAccess = "true"))
	bool bWeaponEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables, meta = (AllowPrivateAccess = "true"))
	bool bPistolEquip;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables, meta = (AllowPrivateAccess = "true"))
	bool bRifleEquip;

	/* Pistol Montage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* PistolEquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* PistolUnEquipMontage;

	/* Rifle Montage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RifleEquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RifleUnEquipMontage;

	/* Weapon Blueprints */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeaponBase> PistolBlueprint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	AWeaponBase* PistolRefrence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeaponBase> RifleBlueprint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	AWeaponBase* RifleRefrence;

	// Crouch TimeLine Curve
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CrouchCurve;

	// Aim TimeLine Curve
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* AimingCurveNormal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* AimingCurveCrouched;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* AimingViewCurve;

	/* Pistol Shoot Montage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* PistolCharacterShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* PistolGunShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* PistolGunNoAmmoMontage;

	/* Rifle Shoot Montage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RifleCharacterShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* RifleGunShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* RifleGunNoAmmoMontage;

	/* Pistol Reload Montage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* PistolCharacterReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* PistolGunReloadMontage;

	/* Rifle Reload Montage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RifleCharacterReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimationAsset* RifleGunReloadMontage;

public:
	AThe_ShooterCharacter();

	// Crouch TimeLine
	FTimeline CrouchTimeline;

	// Aim TimeLine
	FTimeline AimNormalTimeline;
	FTimeline AimCrouchTimeline;
	FTimeline AimViewTimeline;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Jump input */
	void Jump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);

	/** Called for Sprint input */
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);

	// CameraBoom Update Function
	UFUNCTION()
	void UpdateCameraBoom(float Length);

	UFUNCTION()
	void UpdateCameraView(float View);

	/** Called for Crouch input */
	void Crouch(const FInputActionValue& Value);

	/** Called for Pistol Equip input */
	void PistolEquip(const FInputActionValue& Value);

	/** Called for Rifle Equip input */
	void RifleEquip(const FInputActionValue& Value);

	/* Called for Aim input */
	void AimStart(const FInputActionValue& Value);
	void AimStop(const FInputActionValue& Value);
	
	/** Called for Fire input */
	void Firing(const FInputActionValue& Value);

	/** Called for Reload input */
	void Reload(const FInputActionValue& Value);

protected:

	//Spawn Weapon Function
	void SpawnPistol();
	void SpawnRifle();

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	// FootSteps Variables
	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	USoundBase* LeftFootstepSound;

	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	USoundBase* RightFootstepSound;

	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	float TraceDistance = 15.f;

	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	FName LeftFootBone = "foot_l";

	UPROPERTY(EditDefaultsOnly, Category = "Footsteps")
	FName RightFootBone = "foot_r";

private:

	// Variable To Check FootSteps
	bool bLeftFootOnGround = false;
	bool bRightFootOnGround = false;

	// Function To check FootSteps
	void CheckFootstep(FName BoneName, bool& bWasOnGround);

	// Notify for pistol Equip
	UFUNCTION()
	void OnPistolEquipNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload);

	// Notify for pistol UnEquip
	UFUNCTION()
	void OnPistolUnEquipNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload);

	// Notify for pistol UnEquip
	UFUNCTION()
	void OnPistolMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// Notify for Rifle Equip
	UFUNCTION()
	void OnRiflelEquipNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload);

	// Notify for Rifle UnEquip
	UFUNCTION()
	void OnRiflelUnEquipNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload);

	// Notify for pistol UnEquip
	UFUNCTION()
	void OnRifleMontageEnded(UAnimMontage* Montage, bool bInterrupted);


public:

	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

