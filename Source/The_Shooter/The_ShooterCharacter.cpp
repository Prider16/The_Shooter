// Copyright Epic Games, Inc. All Rights Reserved.

#include "The_ShooterCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "Public/Weapon/WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Public/User_Interface/BaseUserWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AThe_ShooterCharacter

AThe_ShooterCharacter::AThe_ShooterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f; // The camera follows at this distance behind the character	
	CameraBoom->SocketOffset = FVector(0.0f, 80.f, 40.f);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input



void AThe_ShooterCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AThe_ShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AThe_ShooterCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AThe_ShooterCharacter::StopJump);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThe_ShooterCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThe_ShooterCharacter::Look);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AThe_ShooterCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AThe_ShooterCharacter::StopSprint);

		//Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AThe_ShooterCharacter::Crouch);

		// Pistol Equip
		EnhancedInputComponent->BindAction(PistolAction, ETriggerEvent::Started, this, &AThe_ShooterCharacter::PistolEquip);

		// Rifle Equip
		EnhancedInputComponent->BindAction(RifleAction, ETriggerEvent::Started, this, &AThe_ShooterCharacter::RifleEquip);

		// Aiming
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AThe_ShooterCharacter::AimStart);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AThe_ShooterCharacter::AimStop);

		// Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AThe_ShooterCharacter::Firing);

		// Reloading
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AThe_ShooterCharacter::Reload);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AThe_ShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Adding and creating Character HUD
	if (CharacterHUDClass)
	{
		// if yes then Create a Widget
		CharacterHUD = CreateWidget<UBaseUserWidget>(GetWorld(), CharacterHUDClass);
		if (CharacterHUD)
		{
			// if created Successfully then Add to viewport
			CharacterHUD->AddToViewport();
		}
	}

	// Setting Up Crouch TimeLine
	if (CrouchCurve)
	{
		FOnTimelineFloat TimelineCallBack;
		TimelineCallBack.BindUFunction(this, FName("UpdateCameraBoom"));
		CrouchTimeline.AddInterpFloat(CrouchCurve, TimelineCallBack);
		CrouchTimeline.SetLooping(false);
		CrouchTimeline.SetPlayRate(2.0f);
	}

	// Setting up Aim TimeLine
	// Normal Aim
	if (AimingCurveNormal)
	{
		FOnTimelineFloat TimelineCallBack;
		TimelineCallBack.BindUFunction(this, FName("UpdateCameraBoom"));
		AimNormalTimeline.AddInterpFloat(AimingCurveNormal, TimelineCallBack);
		AimNormalTimeline.SetLooping(false);
		AimNormalTimeline.SetPlayRate(2.0f);
	}
	if (AimingCurveCrouched)
	{
		FOnTimelineFloat TimelineCallBack;
		TimelineCallBack.BindUFunction(this, FName("UpdateCameraBoom"));
		AimCrouchTimeline.AddInterpFloat(AimingCurveCrouched, TimelineCallBack);
		AimCrouchTimeline.SetLooping(false);
		AimCrouchTimeline.SetPlayRate(2.0f);
	}
	if (AimingViewCurve)
	{
		FOnTimelineFloat TimelineCallBack;
		TimelineCallBack.BindUFunction(this, FName("UpdateCameraView"));
		AimViewTimeline.AddInterpFloat(AimingViewCurve, TimelineCallBack);
		AimViewTimeline.SetLooping(false);
		AimViewTimeline.SetPlayRate(2.0f);
	}

}

void AThe_ShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tick Crouch Timeline
	CrouchTimeline.TickTimeline(DeltaTime);
	
	// Tick Aim Timeline
	AimNormalTimeline.TickTimeline(DeltaTime);
	AimCrouchTimeline.TickTimeline(DeltaTime);
	AimViewTimeline.TickTimeline(DeltaTime);

	// Tick Check for Foot Position
	CheckFootstep(LeftFootBone, bLeftFootOnGround);
	CheckFootstep(RightFootBone, bRightFootOnGround);
}

void AThe_ShooterCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AThe_ShooterCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AThe_ShooterCharacter::Jump(const FInputActionValue& Value)
{
	if (!bisCrouch)
	{
		bisJump = true;
		ACharacter::Jump();
	}
}

void AThe_ShooterCharacter::StopJump(const FInputActionValue& Value)
{
	bisJump = false;
	ACharacter::StopJumping();
}

void AThe_ShooterCharacter::StartSprint(const FInputActionValue& Value)
{
	if (bisCrouch || bisAim)
	{
		bisSprint = false;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
	else
	{
		bisSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

void AThe_ShooterCharacter::StopSprint(const FInputActionValue& Value)
{
	bisSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AThe_ShooterCharacter::UpdateCameraBoom(float Length)
{
	CameraBoom->TargetArmLength = Length;
}

void AThe_ShooterCharacter::UpdateCameraView(float View)
{
	FollowCamera->FieldOfView = View;
}

void AThe_ShooterCharacter::Crouch(const FInputActionValue& Value)
{
	if (!GetCharacterMovement()->IsFalling())
	{

		bisCrouch = !bisCrouch;

		if (!bisCrouch)
		{
			CrouchTimeline.PlayFromStart();
		}
		else
		{
			CrouchTimeline.ReverseFromEnd();
		}
	}
}

void AThe_ShooterCharacter::PistolEquip(const FInputActionValue& Value)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (bWeaponEquip)
	{
		if (bPistolEquip)
		{
			if (!AnimInstance || !PistolUnEquipMontage) return;

			bWeaponEquip = false;
			bPistolEquip = false;

			AnimInstance->Montage_Play(PistolUnEquipMontage);

			AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AThe_ShooterCharacter::OnPistolUnEquipNotifyBeginReceived);
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AThe_ShooterCharacter::OnPistolUnEquipNotifyBeginReceived);
		}
		else
		{
			if (!AnimInstance || !RifleUnEquipMontage || !PistolEquipMontage) return;

			bRifleEquip = false;

			float Duration = AnimInstance->Montage_Play(RifleUnEquipMontage);
			AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AThe_ShooterCharacter::OnRiflelUnEquipNotifyBeginReceived);
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AThe_ShooterCharacter::OnRiflelUnEquipNotifyBeginReceived);
			if (Duration > 0.0f)
			{
				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &AThe_ShooterCharacter::OnRifleMontageEnded);
				AnimInstance->Montage_SetEndDelegate(EndDelegate, RifleUnEquipMontage);
			}
		}
	}
	else
	{
		if (!AnimInstance || !PistolEquipMontage) return;

		bWeaponEquip = true;
		bPistolEquip = true;

		AnimInstance->Montage_Play(PistolEquipMontage);

		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AThe_ShooterCharacter::OnPistolEquipNotifyBeginReceived);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AThe_ShooterCharacter::OnPistolEquipNotifyBeginReceived);
	}
}

void AThe_ShooterCharacter::OnPistolEquipNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload)
{
	if (NotifyName == "Spawn Pistol")
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning pistol!"));
		AThe_ShooterCharacter::SpawnPistol();
	}
}

void AThe_ShooterCharacter::OnPistolUnEquipNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload)
{
	if (NotifyName == "Destroy Pistol")
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroying pistol!"));
		PistolRefrence->Destroy();
	}
}

void AThe_ShooterCharacter::OnPistolMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !RifleEquipMontage) return;

	bRifleEquip = true;

	AnimInstance->Montage_Play(RifleEquipMontage);

	AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AThe_ShooterCharacter::OnRiflelEquipNotifyBeginReceived);
	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AThe_ShooterCharacter::OnRiflelEquipNotifyBeginReceived);

}

void AThe_ShooterCharacter::SpawnPistol()
{
	if (!PistolBlueprint) return;

	UWorld* World = GetWorld();
	if (!World) return;

	// Setting up the transform
	FVector SpawnLocation(0.f, 0.f, 0.f);
	FRotator SpawnRotation(0.f, 0.f, 0.f);
	FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.f, 1.f, 1.f));

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawning the Pistol in the scene
	AWeaponBase* SpawnedPistol = World->SpawnActor<AWeaponBase>(PistolBlueprint, SpawnTransform, SpawnParams);

	PistolRefrence = SpawnedPistol;

	CharacterHUD->SetPistolRefrence(SpawnedPistol);

	// if the pistol is spawned...
	if (SpawnedPistol)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

		//... Attaching it to the component
		SpawnedPistol->AttachToComponent(GetMesh(), AttachmentRules, TEXT("Pistol_Socket"));
	}
}

void AThe_ShooterCharacter::RifleEquip(const FInputActionValue& Value)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (bWeaponEquip)
	{
		if (bRifleEquip)
		{
			if (!AnimInstance || !RifleUnEquipMontage) return;

			bWeaponEquip = false;
			bRifleEquip = false;

			AnimInstance->Montage_Play(RifleUnEquipMontage);

			AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AThe_ShooterCharacter::OnRiflelUnEquipNotifyBeginReceived);
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AThe_ShooterCharacter::OnRiflelUnEquipNotifyBeginReceived);
		}
		else
		{
			if (!AnimInstance || !PistolUnEquipMontage || !RifleEquipMontage) return;

			bPistolEquip = false;

			float Duration = AnimInstance->Montage_Play(PistolUnEquipMontage);
			AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AThe_ShooterCharacter::OnPistolUnEquipNotifyBeginReceived);
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AThe_ShooterCharacter::OnPistolUnEquipNotifyBeginReceived);
			if (Duration > 0.0f)
			{
				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &AThe_ShooterCharacter::OnPistolMontageEnded);
				AnimInstance->Montage_SetEndDelegate(EndDelegate, PistolUnEquipMontage);
			}
		}
	}
	else
	{
		if (!AnimInstance || !RifleEquipMontage) return;
		
		bWeaponEquip = true;
		bRifleEquip = true;

		AnimInstance->Montage_Play(RifleEquipMontage);

		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AThe_ShooterCharacter::OnRiflelEquipNotifyBeginReceived);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AThe_ShooterCharacter::OnRiflelEquipNotifyBeginReceived);
	}
}

void AThe_ShooterCharacter::OnRiflelEquipNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload)
{
	if (NotifyName == "Spawn Rifle")
	{
		AThe_ShooterCharacter::SpawnRifle();
	}
}

void AThe_ShooterCharacter::OnRiflelUnEquipNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload)
{
	if (NotifyName == "Destroy Rifle")
	{
		RifleRefrence->Destroy();
	}
}

void AThe_ShooterCharacter::OnRifleMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !PistolEquipMontage) return;

	bPistolEquip = true;

	AnimInstance->Montage_Play(PistolEquipMontage);

	AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AThe_ShooterCharacter::OnPistolEquipNotifyBeginReceived);
	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AThe_ShooterCharacter::OnPistolEquipNotifyBeginReceived);
}

void AThe_ShooterCharacter::SpawnRifle()
{
	if (!RifleBlueprint) return;

	UWorld* World = GetWorld();
	if (!World) return;

	// Setting up the transform
	FVector SpawnLocation(0.f,0.f,0.f);
	FRotator SpawnRotation(0.f, 0.f, 0.f);
	FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.f, 1.f, 1.f));

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawning the Rifle in the scene
	AWeaponBase* SpawnedRifle = World->SpawnActor<AWeaponBase>(RifleBlueprint, SpawnTransform, SpawnParams);

	RifleRefrence = SpawnedRifle;

	CharacterHUD->SetRifleRefrence(SpawnedRifle);

	// if the Rifle is spawned...
	if (SpawnedRifle)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

		//... Attaching it to the component
		SpawnedRifle->AttachToComponent(GetMesh(), AttachmentRules, TEXT("Rifle_Socket"));
	}
}

void AThe_ShooterCharacter::AimStart(const FInputActionValue& Value)
{
	bisAim = true;

	if (bisCrouch)
	{
		AimCrouchTimeline.PlayFromStart();
		AimViewTimeline.PlayFromStart();
	}
	else
	{
		AimNormalTimeline.PlayFromStart();
		AimViewTimeline.PlayFromStart();
	}
}

void AThe_ShooterCharacter::AimStop(const FInputActionValue& Value)
{
	bisAim = false;

	if (bisCrouch)
	{
		AimCrouchTimeline.ReverseFromEnd();
		AimViewTimeline.ReverseFromEnd();
	}
	else
	{
		AimNormalTimeline.ReverseFromEnd();
		AimViewTimeline.ReverseFromEnd();
	}
	
}

void AThe_ShooterCharacter::Firing(const FInputActionValue& Value)
{
	if (bWeaponEquip)
	{
		if (!bisSprint)
		{
			if (bPistolEquip)
			{
				if (PistolRefrence->Pistol_currentammo > 0.0f)
				{
					PlayAnimMontage(PistolCharacterShootMontage);
					PistolRefrence->GetWeaponMesh()->PlayAnimation(PistolGunShootMontage, false);
					PistolRefrence->Pistol_Fire();
				}
				else
				{
					PlayAnimMontage(PistolCharacterShootMontage);
					PistolRefrence->GetWeaponMesh()->PlayAnimation(PistolGunNoAmmoMontage, false);
					PistolRefrence->Pistol_Fire();
				}
			}
			if (bRifleEquip)
			{
				if (RifleRefrence->Rifle_currentammo > 0.0f)
				{
					PlayAnimMontage(RifleCharacterShootMontage);
					RifleRefrence->GetWeaponMesh()->PlayAnimation(RifleGunShootMontage, false);
					RifleRefrence->Rifle_Fire();
				}
				else
				{
					PlayAnimMontage(RifleCharacterShootMontage);
					RifleRefrence->GetWeaponMesh()->PlayAnimation(RifleGunNoAmmoMontage, false);
					RifleRefrence->Rifle_Fire();
				}
			}
		}
		if (bisSprint && bisAim)
		{
			if (bPistolEquip)
			{
				if (PistolRefrence->Pistol_currentammo > 0.0f)
				{
					PlayAnimMontage(PistolCharacterShootMontage);
					PistolRefrence->GetWeaponMesh()->PlayAnimation(PistolGunShootMontage, false);
					PistolRefrence->Pistol_Fire();
				}
				else
				{
					PlayAnimMontage(PistolCharacterShootMontage);
					PistolRefrence->GetWeaponMesh()->PlayAnimation(PistolGunNoAmmoMontage, false);
					PistolRefrence->Pistol_Fire();
				}
			}
			if (bRifleEquip)
			{
				if (RifleRefrence->Rifle_currentammo > 0.0f)
				{
					PlayAnimMontage(RifleCharacterShootMontage);
					RifleRefrence->GetWeaponMesh()->PlayAnimation(RifleGunShootMontage, false);
					RifleRefrence->Rifle_Fire();
				}
				else
				{
					PlayAnimMontage(RifleCharacterShootMontage);
					RifleRefrence->GetWeaponMesh()->PlayAnimation(RifleGunNoAmmoMontage, false);
					RifleRefrence->Rifle_Fire();
				}
			}
		}
	}
}

void AThe_ShooterCharacter::Reload(const FInputActionValue& Value)
{
	if (bWeaponEquip)
	{
		if (bPistolEquip)
		{
			if (PistolRefrence->Pistol_currentammo != PistolRefrence->Pistol_totalammo)
			{
				PlayAnimMontage(PistolCharacterReloadMontage);
				PistolRefrence->GetWeaponMesh()->PlayAnimation(PistolGunReloadMontage, false);
				PistolRefrence->Pistol_Reload();
			}
		}
		if (bRifleEquip)
		{
			if (RifleRefrence->Rifle_currentammo != RifleRefrence->Rifle_totalammo)
			{
				PlayAnimMontage(RifleCharacterReloadMontage);
				RifleRefrence->GetWeaponMesh()->PlayAnimation(RifleGunReloadMontage, false);
				RifleRefrence->Rifle_Reload();
			}
		}
	}
}

void AThe_ShooterCharacter::CheckFootstep(FName BoneName, bool& bWasOnGround)
{
	if (!GetMesh()) return;

	FVector BoneLocation = GetMesh()->GetBoneLocation(BoneName);
	FVector EndLocation = BoneLocation - FVector(0, 0, TraceDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, BoneLocation, EndLocation, ECC_Visibility, Params);

	if (bHit && !bWasOnGround && !bisCrouch)
	{
		bWasOnGround = true;

		if (LeftFootstepSound && BoneName == LeftFootBone)
		{
			UGameplayStatics::PlaySoundAtLocation(this, LeftFootstepSound, BoneLocation);
		}
		if (RightFootstepSound && BoneName == RightFootBone)
		{
			UGameplayStatics::PlaySoundAtLocation(this, RightFootstepSound, BoneLocation);
		}
	}
	else if (!bHit)
	{
		bWasOnGround = false;
	}

	// Draw debug trace
	//DrawDebugLine(GetWorld(), BoneLocation, EndLocation, bHit ? FColor::Green : FColor::Red, false, 0.1f, 0, 1.0f);

}