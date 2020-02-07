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
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UABHUDWidget> HUDWidgetClass;
protected:
	virtual void BeginPlay() override;
public:
	AABPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;

	class UABHUDWidget* GetHUDWidget() const;
};
