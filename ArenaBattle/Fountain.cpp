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

// Fill out your copyright notice in the Description page of Project Settings.

#include "Fountain.h"
#include <iostream>

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

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_WATER(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Meshes/Env/SM_Plains_Fountain_02.SM_Plains_Fountain_02"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		PS_SPLASH(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_Base_01.P_Water_Fountain_Splash_Base_01"));

	if (SM_BODY.Succeeded())
		body->SetStaticMesh(SM_BODY.Object);
	if (SM_WATER.Succeeded())
		water->SetStaticMesh(SM_WATER.Object);
	if (PS_SPLASH.Succeeded())
		splash->SetTemplate(PS_SPLASH.Object);
}

// Called when the game starts or when spawned
void AFountain::BeginPlay()
{
	Super::BeginPlay();

	ABLOG_S(Warning);
	ABLOG(Warning, TEXT("Actor Name : %s, ID : %d, Location X : %.3f"), *GetName(), ID, GetActorLocation().X);
}

// Called every frame
void AFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

