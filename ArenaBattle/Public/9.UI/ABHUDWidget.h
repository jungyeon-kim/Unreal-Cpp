#pragma once

#include "ArenaBattle.h"
#include "Blueprint/UserWidget.h"
#include "ABHUDWidget.generated.h"

UCLASS()
class ARENABATTLE_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	TWeakObjectPtr<class UABCharacterStatComponent> CurrentCharacterStat{};
	TWeakObjectPtr<class AABPlayerState> CurrentPlayerState{};

	UPROPERTY()
	class UProgressBar* HPBar;
	UPROPERTY()
	class UProgressBar* ExpBar;
	UPROPERTY()
	class UTextBlock* PlayerName;
	UPROPERTY()
	class UTextBlock* PlayerLevel;
	UPROPERTY()
	class UTextBlock* CurrentScore;
	UPROPERTY()
	class UTextBlock* HighScore;
protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();
public:
	void BindCharacterStat(class UABCharacterStatComponent* CharacterStat);
	void BindPlayerState(class AABPlayerState* PlayerState);
};
