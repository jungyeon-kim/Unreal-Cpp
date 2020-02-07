#pragma once

#include "ArenaBattle.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterWidget.generated.h"

UCLASS()
class ARENABATTLE_API UABCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UProgressBar* HPProgressBar;
	TWeakObjectPtr<class UABCharacterStatComponent> CurrentCharacterStat;
protected:
	virtual void NativeConstruct() override;	// 초기화 함수
	void UpdateHPWidget();
public:
	void BindCharacterStat(class UABCharacterStatComponent* NewCharacterStat);
};
