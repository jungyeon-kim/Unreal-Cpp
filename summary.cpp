//	용어정리
/*
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
							블루프린트와도 연동되는 델리게이트 -> 다이나믹 델리게이트

	Skeletal Mesh:			애니메이션 재생을 위해 리깅 데이터를 추가한 메시 (스켈레탈 메시 컴포넌트가 관리)
	Rigging:				모델링된 데이터에 뼈를 붙이는 작업
	Axis Mapping:			조이스틱 레버의 신호를 전달 (-1, 0, 1)
	Action Mapping:			조이스틱 버튼의 신호를 전달 (keyup, keydown)
	Navigation:				목적지를 알려주면 스스로 목적지까지 이동하는 길 찾기 기능
	AnimInstance:			애님 그래프가 참조할 데이터를 제공
	AnimGraph:				애님 인스턴스의 변수 값에 따라 변화하는 애니메이션을 설계
	Montage:				특정 상황에서 원하는 애니메이션을 발동시키는 기능
	Animation Notify:		특정 타이밍에 애님인스턴스에게 신호를 보내는 기능
	Phisics:				물리적인 시뮬레이션을 할 때 사용
	Query:					물체간의 오버랩, 레이캐스트, 스윕등 충돌탐지를 할 때 사용
*/


// 기능정리
/*
	CreateDefaultSubobject():	컴포넌트를 생성하는 용도로 new 키워드 대신 사용
								컴포넌트 구별을 위해 매개변수로 문자열을 받음 (Hash값 생성에 사용하며, 중복되면 안됨)
	TEXT():					모든 플랫폼에서 2byte 문자열 체계(유니코드)를 유지시켜주는 매크로
	SetupAttachment():		해당 컴포넌트를 매개변수의 자식으로 설정
	SetRelativeLocation():	부모를 기준으로 컴포넌트의 위치를 설정
	TryGetPawnOwner():		일반적으로 Tick은 입력 -> 로직 -> 애니메이션 순이다.
							로직에서 폰이 제거되었다면 애니메이션에서 폰을 참조할 때 유효하지않다.
							따라서, 폰이 유효한지 검사하는 함수가 이것이다.
	Cast<T>():				dynamic_cast와 유사
	OnMontageEnded():		몽타주 재생이 끝나면 발동하는 델리게이트

	FClassFinder:			에디터에서 블루프린트 클래스를 가져올 때 사용하는 구조체
	FObjectFinder:			에디터에서 리소스를 가져올 때 사용하는 구조체
	FRotationMatrix:		회전된 좌표계 정보를 저장하는 행렬 클래스
*/


// 기타
/*
	클래스 접두사:			A:	액터 클래스
							U:	액터가 아닌 클래스
							F:	언리얼 오브젝트와 관련없는 클래스나 구조체
	UPROPERTY():			멤버변수를 default값으로 초기화하고 에디터에 노출시킴
							사용자가 선언한 언리얼 오브젝트를 자동으로 관리해줌 (garbage collection)
							언리얼 오브젝트가 아니라면, 직접 해제해주거나 스마트포인터를 사용해야함
							VisibleAnywhere:	어디서든 읽기작업 가능
							EditAnywhere:		어디서든 읽기, 쓰기작업 가능
							*DefaultsOnly:		블루프린트 편집화면에서만
							*InstanceOnly:		에디터 뷰포트에서만
							Category:			지정한 분류에서 멤버를 관리 가능
							AllowPrivateAccess:	private멤버가 블루프린트에 노출됨
							BlueprintReadOnly:	블루프린트에서 읽기만 가능
							BlueprintReadWrite:	블루프린트에서 읽기, 쓰기 둘다가능
	UFUNCTION()				멤버함수를 에디터에 노출시킴
							C++멤버함수를 블루프린트에서 호출, 오버라이드를 가능하게함
	UCLASS():				해당 클래스가 언리얼 오브젝트임을 바깥쪽에 명시
	GENERATED_BODY():		해당 클래스가 언리얼 오브젝트임을 안쪽에 명시
	generated.h:			언리얼 헤더 툴에 의해 자동으로 생성되는 부가파일. 꼭 include 해주어야 함
	모듈명_API:				DLL 내 클래스 정보를 외부에 공개할지 결정하는 _declspec(dllexport) 키워드를 사용하기위해 명시

	~키 명령어)			
	displayall PlayerController ControlRotation:		플레이어 컨트롤러의 회전값 확인
*/