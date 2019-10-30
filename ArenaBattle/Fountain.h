/*
	Actor:				level에 배치할 수 있는 오브젝트
						3D transformation을 지원
						베이스 클래스는 AActor
	UPROPERTY():		멤버를 default값으로 초기화하고 에디터에 노출시킴
						사용자가 선언한 언리얼 오브젝트를 자동으로 관리해줌 (garbage collection)
						언리얼 오브젝트가 아니라면, 직접 해제해주거나 스마트포인터를 사용해야함
						VisibleAnywhere:	어디서든 읽기작업 가능
						EditAnywhere:		어디서든 읽기, 쓰기작업 가능
						Category:			지정한 분류에서 멤버를 관리 가능
	UCLASS():			해당 클래스가 언리얼 오브젝트임을 바깥쪽에 명시
	GENERATED_BODY():	해당 클래스가 언리얼 오브젝트임을 안쪽에 명시
	클래스 접두사:		A:	액터 클래스
						U:	액터가 아닌 클래스
						F:	언리얼 오브젝트와 관련없는 일반 클래스나 구조체
	generated.h:		언리얼 헤더 툴에 의해 자동으로 생성되는 부가파일. 꼭 include 해주어야 함
	모듈명_API:			DLL 내 클래스 정보를 외부에 공개할지 결정하는 _declspec(dllexport) 키워드를 사용하기위해 명시
*/

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "Fountain.generated.h"

UCLASS()
class ARENABATTLE_API AFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountain();

protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category=ID)
	int32 ID;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* body;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* water;
	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* light;
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* splash;
};
