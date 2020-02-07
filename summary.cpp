//	용어정리
/*
	GameMode:				게임의 규칙을 정하는 클래스
	GameInstance:			게임 앱을 관리하는 클래스
	Actor:					level에 배치할 수 있는 오브젝트
	Component:				액터에 추가할 수 있는 함수성 조각
							독립적으로 존재 불가 (액터에 종속)
	RootComponent:			컴포넌트 생성시 반드시 해당 액터를 대표할 RootComponent를 지정해 주어야함
	Pawn:					플레이어에게 조종당하는 액터
							FloatingPawnMovement 사용
	Character:				폰에서 기능을 더 추가한 액터
							Capsule, Mesh, Movement가 기본적으로 존재 (Get으로 참조가능)
							Jump() 메서드가 기본적으로 존재
							CharacterMovement 사용 (중력이 적용된 다양한 애니메이션, 네트워크 자동 동기화)

	★Delegate:				특정 객체가 수행해야할 로직을 다른 객체가 대신 처리해줄 수 있도록 만드는 개념
							1. A 메서드를 B에 등록	2. B에게 명령	3. B가 명령을 실행	4. 1에서 등록한 함수 호출
							블루프린트와도 연동되는 델리게이트 -> 다이나믹 델리게이트 (UFUNCTION 필요)
							AddDynamic:	언리얼 오브젝트기반 클래스의 메서드를 다이나믹 델리게이트에 등록
							AddUObject:	언리얼 오브젝트기반 클래스의 메서드를 델리게이트에 등록
							AddLambda:	람다 함수를 델리게이트에 등록
	★Reflection:			객체 정보를 런타임에서 실시간으로 조회가 가능하게하는 기능
							에디터 디테일 패널, 가비지콜렉팅, 직렬화, BP & C++ 연동 등에 사용
	★Serialization:			객체를 일련의 문자나 숫자로 변환하는 것
							언리얼 엔진에 객체를 저장 & 로드하기 위함

	Skeletal Mesh:			애니메이션 재생을 위해 리깅 데이터를 추가한 메시 (스켈레탈 메시 컴포넌트가 관리)
	Rigging:				모델링된 데이터에 뼈를 붙이는 작업
	Axis Mapping:			조이스틱 레버의 신호를 전달 (-1, 0, 1)
	Action Mapping:			조이스틱 버튼의 신호를 전달 (keyup, keydown)
	Navigation:				목적지를 알려주면 스스로 목적지까지 이동하는 길 찾기 기능
	AnimInstance:			애님 그래프가 참조할 데이터를 제공
	AnimGraph:				애님 인스턴스의 변수 값에 따라 변화하는 애니메이션을 설계
	Montage:				특정 상황에서 원하는 애니메이션을 발동시키는 기능
	Animation Notify:		특정 타이밍에 애님인스턴스에게 신호를 보내는 기능
	Physics:				물리적인 시뮬레이션을 할 때 사용
	Query:					물체간의 오버랩, 레이캐스트, 스윕등 충돌탐지를 할 때 사용
	Overlap:				두 충돌체가 겹쳤는지 판단한다.
	Trace(Ray Cast):		시작점에서 끝점까지 레이저를 발사해 부딫히는 충돌체가 있는지 판단한다.
	Sweep:					충돌체가 한 프레임에 움직인 경로를 예측해 부딫히는 다른 충돌체가 있는지 판단한다.
							충돌체가 구형태였다면 캡슐이 형성될 것이다.
	Object Channel:			양방향 충돌체크
	Trace Channel:			단방향 충돌체크 (주체는 충돌체크를 하지않음)
	BlackBoard:				인공지능의 판단에 사용하는 데이터 집합
	BehaviorTree:			블랙보드를 기반으로 비헤비어트리를 시각화해 저장
	Composite Node:			Selector:	자손 노드를 왼쪽에서 오른쪽 순서로 실행하며, 
										자손 노드 중 하나가 실행에 성공하면 자손의 실행을 멈춘다
							Sequence:	자손 노드를 왼쪽에서 오른쪽 순서로 실행하며, 
										자손 노드중 하나가 실행에 실패하면 자손의 실행을 멈춘다.
							Simple Parallel:	자손 노드를 순차적이 아닌 한번에 실행
	Service Node:			컴포짓 노드에 부착되는 서브노드 (설정한 주기마다 실행)
	Decorator Node:			블랙보드 값을 기반으로 특정 컴포짓 노드의 실행여부를 결정 (조건절)
	Task Node:				행동. 독립적으로 실행이 불가하며 컴포짓 노드를 거쳐야한다.
*/


// 기능정리
/*
	CreateDefaultSubobject():	컴포넌트를 생성하는 용도로 new 키워드 대신 사용
								컴포넌트 구별을 위해 매개변수로 문자열을 받음 (Hash값 생성에 사용하며, 중복되면 안됨)
	SpawnActor<>():			월드에 액터를 생성한다.
	TEXT():					모든 플랫폼에서 2byte 문자열 체계(유니코드)를 유지시켜주는 매크로
	SetupAttachment():		해당 컴포넌트를 매개변수의 자식으로 설정
	SetRelativeLocation():	부모를 기준으로 컴포넌트의 위치를 설정
	TryGetPawnOwner():		일반적으로 Tick은 입력 -> 로직 -> 애니메이션 순이다.
							로직에서 폰이 제거되었다면 애니메이션에서 폰을 참조할 때 유효하지않다.
							따라서, 폰이 유효한지 검사하는 함수가 이것이다.
	Cast<T>():				dynamic_cast와 유사 
							내부에서 템플릿 인자를 포인터로 받기때문에 인자에 *을 써줄 필요가 없다.
	TakeDamege():			액터에게 데미지를 전달 (세기, 종류, 가해자, 사용 도구)
	SetActorEnableCollision():	액터의 충돌설정을 on / off
	SetCollisionProfileName():	어떤 콜리전 프리셋을 사용할 것인지 지정한다.
	AttachToComponent():	컴포넌트에 부착시킨다.
	StaticClass():			런타임에 이 클래스를 나타내는 UClass 객체를 반환
	SetVisibility():		컴포넌트의 시각적 기능을 세팅한다. (에디터, 게임플레이화면에서 모두 사라진다.)
	SetHiddenInGame():		SetVisibility()와 같은 기능이나 게임플레이화면에서만 사라진다.

	OnMontageEnded:			몽타주 재생이 끝나면 발동하는 델리게이트 (애님인스턴스)
	OnComponentBeginOvelap:	Overlap 이벤트가 발생할 때마다 발동하는 델리게이트 (박스 컴포넌트)
	OnSystemFinished:		이펙트 재생이 종료되면 발동하는 델리게이트 (파티클 컴포넌트)

	FString:				FString 타입의 문자열 값을 가져오려면 앞에 *을 붙여야한다. (특이하지만 적응하자.)
	FClassFinder:			에디터에서 블루프린트 클래스를 가져올 때 사용하는 구조체
	FObjectFinder:			에디터에서 리소스를 가져올 때 사용하는 구조체
	FRotationMatrix:		회전된 좌표계 정보를 저장하는 행렬 클래스
	TWeakObjectPtr:			언리얼 오브젝트용 약포인터
							엔진 별도의 메모리 관리 시스템때문에 언리얼 오브젝트는 TWeakPtr 대신 이를 사용해야함
	TSubclassOf<>:			특정 클래스와 상속받은 클래스들로 선언 목록을 제한해주는 클래스
							UCLASS와 일반 Class는 모든 언리얼 오브젝트에 대한 선언이 보이고,
							일반 Class는 메모리할당 전에는 아무것도 보이지가 않기 때문에 사용
							ex)	TSubclassOf<T>			->	T와 T를 상속받은 클래스만 보임
								UClass* / class T*		->	모든 언리얼 오브젝트가 보임
	InterpTo():				시작부터 끝까지 타겟을 부드럽게 보간해주면서 변형시킴
	MakeFrom*():			*방향으로의 회전값을 리턴
	OnConstruction():		해당액터가 에디터에 배치되거나 스폰 될 때 호출
*/


// 기타
/*
	클래스 접두사:			A:	액터 클래스
							U:	액터가 아닌 클래스
							S:	위젯 클래스
							E:	Enum (열거형)
							F:	언리얼 오브젝트와 관련없는 클래스나 구조체
							b:	bool 변수

	언리얼 오브젝트:			1. CDO(Class Default Object): 객체의 초기 값을 자체적으로 관리
							2. Reflection: 객체 정보를 런타임에서 실시간 조회가 가능
							3. GC(Garbage Collection): 참조되지 않는 객체를 메모리에서 자동 해제 가능
							4. Serialization: 객체와 속성 정보를 통으로 안전하게 보관하고 로딩
							5. Delegate: 함수를 묶어서 효과적으로 관리하고 호출 가능
							6. Replication: 네트워크 상에서 객체간에 동기화 가능
							7. Editor Integration: 언리얼 에디터 인터페이스를 통해 값을 편집 가능

	게임의 시작 과정:			게임 앱 초기화	-	레벨의 액터 초기화	-	플레이어 로그인	-	게임 시작
							UGameInstace::		AActor::PostInitialize	AGameMode::			AGameMode::StartPlay()
							Init()				Componets()				PostLogin()			AActor::BeginPlay()

	UPROPERTY():			멤버변수를 default값으로 초기화하고 에디터에 노출시킴
							언리얼 오브젝트가 아니라면, 직접 해제해주거나 스마트포인터를 사용해야함
							VisibleAnywhere:	어디서든 읽기작업 가능 (디테일 패널)
							EditAnywhere:		어디서든 읽기, 쓰기작업 가능 (디테일 패널)
							BlueprintReadOnly:	블루프린트에서 읽기만 가능 (BP 그래프)
							BlueprintReadWrite:	블루프린트에서 읽기, 쓰기 둘다가능 (BP 그래프)
							*DefaultsOnly:		블루프린트 편집화면에서만 (디테일 패널)
							*InstanceOnly:		에디터 뷰포트에서만 (디테일 패널)
							Category:			지정한 분류에서 멤버를 관리 가능
							AllowPrivateAccess:	private멤버가 블루프린트에 노출됨
							Transient:			직렬화에서 제외 -> 휘발성 멤버변수
	UFUNCTION()				멤버함수를 에디터에 노출시킴
							C++멤버함수를 블루프린트에서 호출, 오버라이드를 가능하게함
	UCLASS():				해당 클래스가 언리얼 오브젝트임을 바깥쪽에 명시
	GENERATED_BODY():		해당 클래스가 언리얼 오브젝트임을 안쪽에 명시
	generated.h:			언리얼 헤더 툴(UHT)에 의해 자동으로 생성되는 부가파일. 꼭 include 해주어야 함
	모듈명_API:				DLL 내 클래스 정보를 외부에 공개할지 결정하는 _declspec(dllexport) 키워드를 사용하기위해 명시

	Tip)			
	displayall PlayerController ControlRotation:		플레이어 컨트롤러의 회전값 확인
	Config -> DefaultEngine.ini에서 트레이스 채널 확인 가능
*/