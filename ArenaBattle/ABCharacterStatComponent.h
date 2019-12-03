#pragma once

#include "ArenaBattle.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	struct FABCharacterData* CurrentStatData{};

	UPROPERTY(EditInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	int32 Level;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP;
protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
public:
	FOnHPIsZeroDelegate OnHPIsZero{};
	FOnHPChangedDelegate OnHPChanged{};
public:	
	UABCharacterStatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetAttack() const;
	float GetHPRatio() const;
	void SetNewLevel(int32 NewLevel);
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
};
