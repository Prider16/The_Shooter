// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "The_ShooterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
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

public:
	AThe_ShooterCharacter();
	

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

	/** Called for Crouch input */
	void Crouch(const FInputActionValue& Value);
	
protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

