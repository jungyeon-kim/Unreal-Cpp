/*
	Actor:				level�� ��ġ�� �� �ִ� ������Ʈ
						3D transformation�� ����
						���̽� Ŭ������ AActor
	UPROPERTY():		����� default������ �ʱ�ȭ�ϰ� �����Ϳ� �����Ŵ
						����ڰ� ������ �𸮾� ������Ʈ�� �ڵ����� �������� (garbage collection)
						�𸮾� ������Ʈ�� �ƴ϶��, ���� �������ְų� ����Ʈ�����͸� ����ؾ���
						VisibleAnywhere:	��𼭵� �б��۾� ����
						EditAnywhere:		��𼭵� �б�, �����۾� ����
						Category:			������ �з����� ����� ���� ����
	UCLASS():			�ش� Ŭ������ �𸮾� ������Ʈ���� �ٱ��ʿ� ���
	GENERATED_BODY():	�ش� Ŭ������ �𸮾� ������Ʈ���� ���ʿ� ���
	Ŭ���� ���λ�:		A:	���� Ŭ����
						U:	���Ͱ� �ƴ� Ŭ����
						F:	�𸮾� ������Ʈ�� ���þ��� �Ϲ� Ŭ������ ����ü
						
	generated.h:		�𸮾� ��� ���� ���� �ڵ����� �����Ǵ� �ΰ�����. �� include ���־�� ��
	����_API:			DLL �� Ŭ���� ������ �ܺο� �������� �����ϴ� _declspec(dllexport) Ű���带 ����ϱ����� ���
*/

// Fill out your copyright notice in the Description page of Project Settings.

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
