#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class ARENABATTLE_API AABItemBox : public AActor
{
	GENERATED_BODY()
private:
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlapedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:
	UPROPERTY(VisibleAnywhere, Category = "Box")
	UBoxComponent* Trigger;
	UPROPERTY(VisibleAnywhere, Category = "Box")
	UStaticMeshComponent* Box;
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	UParticleSystemComponent* Effect;
	UPROPERTY(EditInstanceOnly, Category = "Box")
	TSubclassOf<class AABWeapon> WeaponItemClass;	// 무기의 정보를 저장할 변수
public:	
	AABItemBox();

	virtual void Tick(float DeltaTime) override;
};
