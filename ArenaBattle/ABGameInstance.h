#pragma once

#include "ArenaBattle.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "ABGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FABCharacterData : public FTableRowBase
{	// csv������ ù ���� �𸮾� �������� �ڵ����� Ű������ ����Ѵ�. (���� x)
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
public:
	// �� ������ �ʱ�ȭ����Ʈ���� {}�� �ʱ�ȭ�� �ȵɱ�?
	FABCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}
};

UCLASS()
class ARENABATTLE_API UABGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UDataTable* ABCharacterTable;
public:
	UABGameInstance();

	virtual void Init() override;

	FABCharacterData* GetABCharacterData(int32 Level);
};
