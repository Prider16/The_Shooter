// Fill out your copyright notice in the Description page of Project Settings.


#include "User_Interface/BaseUserWidget.h"

void UBaseUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (PistolRefrence)
	{
		GetPistolCurrentAmmo();
		GetPistolTotalAmmo();
	}
	if (RifleRefrence)
	{
		GetRifleCurrentAmmo();
		GetRifleTotalAmmo();
	}
}

void UBaseUserWidget::SetRifleRefrence(AWeaponBase* RifleRef)
{
	RifleRefrence = RifleRef;
	if (RifleRefrence)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rifle Not Null!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Rifle is Null!"));
	}
	//Rifle_CurrentAmmo = RifleRefrence->Rifle_currentammo;
	//Rifle_TotalAmmo = RifleRefrence->Rifle_totalammo;
}

void UBaseUserWidget::SetPistolRefrence(AWeaponBase* PistolRef)
{
	PistolRefrence = PistolRef;
	if (PistolRefrence)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pistol Not Null!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pistol is Null!"));
	}
	//Pistol_CurrentAmmo = PistolRefrence->Pistol_currentammo;
	//Pistol_TotalAmmo = PistolRefrence->Pistol_totalammo;
}

FText UBaseUserWidget::GetPistolCurrentAmmo() const
{
	return FText::FromString(FString::Printf(TEXT("%f"), PistolRefrence->Pistol_currentammo));
}

FText UBaseUserWidget::GetPistolTotalAmmo() const
{
	return FText::FromString(FString::Printf(TEXT("%f"), PistolRefrence->Pistol_totalammo));
}

FText UBaseUserWidget::GetRifleCurrentAmmo() const
{
	return FText::FromString(FString::Printf(TEXT("%f"), RifleRefrence->Rifle_currentammo));
}

FText UBaseUserWidget::GetRifleTotalAmmo() const
{
	return FText::FromString(FString::Printf(TEXT("%f"), RifleRefrence->Rifle_totalammo));
}


