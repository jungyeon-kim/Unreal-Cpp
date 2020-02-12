#pragma once

#include "ArenaBattle.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UABHUDWidget* HUDWidget;
	UPROPERTY()
	class AABPlayerState* ABPlayerState;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UABHUDWidget> HUDWidgetClass;
protected:
	virtual void BeginPlay() override;
public:
	AABPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;

	void AddGameScore() const;

	class UABHUDWidget* GetHUDWidget() const;
};
