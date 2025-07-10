// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/WeaponBase.h"
#include "BaseUserWidget.generated.h"

class AWeaponBase;

UCLASS()
class THE_SHOOTER_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Called every frame
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void SetRifleRefrence(AWeaponBase* RifleRef);

	UFUNCTION()
	void SetPistolRefrence(AWeaponBase* PistolRef);

	UFUNCTION()
	void SetHealth(float Value);

	UFUNCTION(BlueprintPure, Category = "UI")
	FText GetPistolCurrentAmmo() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	FText GetPistolTotalAmmo() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	FText GetRifleCurrentAmmo() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	FText GetRifleTotalAmmo() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	float GetPlayerHealth() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	AWeaponBase* RifleRefrence = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	AWeaponBase* PistolRefrence = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float Health = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "variables")
	float Pistol_CurrentAmmo = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "variables")
	float Pistol_TotalAmmo = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "variables")
	float Rifle_CurrentAmmo = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "variables")
	float Rifle_TotalAmmo = 0.0f;

	int32 Bandages;

	UFUNCTION()
	void SetBandageCount(int32 Value);

	UFUNCTION(BlueprintPure, Category = "UI")
	FText GetBandageCount() const;

};
