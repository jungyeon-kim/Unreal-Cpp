// Fill out your copyright notice in the Description page of Project Settings.

/*
	Component:				���Ϳ� �߰��� �� �ִ� �Լ��� ����
							���������� ���� �Ұ� (���Ϳ� ����)
	CreateDefaultSubobject:	������Ʈ�� �����ϴ� �뵵�� new Ű���� ��� ���
							������Ʈ ������ ���� �Ű������� ���ڿ��� ���� (Hash�� ������ ����ϸ�, �ߺ��Ǹ� �ȵ�)
	TEXT():					��� �÷������� 2byte ���ڿ� ü��(�����ڵ�)�� ���������ִ� ��ũ��
	RootComponent:			������Ʈ ������ �ݵ�� �ش� ���͸� ��ǥ�� RootComponent�� ������ �־����
	SetupAttachment():		�ش� ������Ʈ�� �Ű������� �ڽ����� ����
	SetRelativeLocation():	�θ� �������� ������Ʈ�� ��ġ�� ����
*/

#include "Fountain.h"

// Sets default values
AFountain::AFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"));
	light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
	splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"));

	RootComponent = body;

	water->SetupAttachment(body);
	light->SetupAttachment(body);
	splash->SetupAttachment(body);

	water->SetRelativeLocation({ 0, 0, 135 });
	light->SetRelativeLocation({ 0, 0, 195 });
	splash->SetRelativeLocation({ 0, 0, 195 });
}

// Called when the game starts or when spawned
void AFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

