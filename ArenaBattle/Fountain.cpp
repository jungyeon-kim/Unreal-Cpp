/*
	Component:				액터에 추가할 수 있는 함수성 조각
							독립적으로 존재 불가 (액터에 종속)
	CreateDefaultSubobject:	컴포넌트를 생성하는 용도로 new 키워드 대신 사용
							컴포넌트 구별을 위해 매개변수로 문자열을 받음 (Hash값 생성에 사용하며, 중복되면 안됨)
	TEXT():					모든 플랫폼에서 2byte 문자열 체계(유니코드)를 유지시켜주는 매크로
	RootComponent:			컴포넌트 생성시 반드시 해당 액터를 대표할 RootComponent를 지정해 주어야함
	SetupAttachment():		해당 컴포넌트를 매개변수의 자식으로 설정
	SetRelativeLocation():	부모를 기준으로 컴포넌트의 위치를 설정
*/

// Fill out your copyright notice in the Description page of Project Settings.

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

void AFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ABLOG_S(Warning);
}

// Called when the game starts or when spawned
void AFountain::BeginPlay()
{
	Super::BeginPlay();

	ABLOG_S(Warning);
	ABLOG(Warning, TEXT("Actor Name : %s, ID : %d, Location X : %.3f"), *GetName(), ID, GetActorLocation().X);
}

void AFountain::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ABLOG_S(Warning);
}

// Called every frame
void AFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

