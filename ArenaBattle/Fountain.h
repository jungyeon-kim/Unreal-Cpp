// Fill out your copyright notice in the Description page of Project Settings.

/*
	Actor:				level에 배치할 수 있는 오브젝트
						3D transformation을 지원
						베이스 클래스는 AActor
	UPROPERTY():		멤버를 에디터에 노출시킴
						사용자가 선언한 언리얼 오브젝트를 자동으로 관리해줌 (garbage collection)
						언리얼 오브젝트가 아니라면, 직접 해제해주거나 스마트포인터를 사용해야함
						VisibleAnywhere:	어디서든 읽기작업 가능
						EditAnywhere:		어디서든 읽기, 쓰기작업 가능
	UCLASS():			해당 클래스가 언리얼 오브젝트임을 바깥쪽에 명시
	GENERATED_BODY():	해당 클래스가 언리얼 오브젝트임을 안쪽에 명시
	클래스 접두사:		액터 클래스는 A, 액터가 아닌 클래스는 U를 앞에붙임
	generated.h:		언리얼 헤더 툴에 의해 자동으로 생성되는 부가파일. 꼭 include 해주어야 함
	모듈명_API:			DLL 내 클래스 정보를 외부에 공개할지 결정하는 _declspec(dllexport) 키워드를 사용하기위해 명시
*/

#pragma once

#include "EngineMinimal.h"
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
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* body{};
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* water{};
	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* light{};
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* splash{};
};
