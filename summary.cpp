//	용어정리
/*
	Actor:					level에 배치할 수 있는 오브젝트
	Component:				액터에 추가할 수 있는 함수성 조각
							독립적으로 존재 불가 (액터에 종속)
	Pawn:					플레이어에게 조종당하는 액터
	Character:
	Skeletal Mesh:			애니메이션 재생을 위해 리깅 데이터를 추가한 메시 (스켈레탈 메시 컴포넌트가 관리)
	Navigation:				목적지를 알려주면 스스로 목적지까지 이동하는 길 찾기 기능

	Rigging:				모델링된 데이터에 뼈를 붙이는 작업
	Axis Mapping:			조이스틱 레버의 신호를 전달 (-1, 0, 1)
	Action Mapping:			조이스틱 버튼의 신호를 전달 (keyup, keydown)
*/


// 기능정리
/*
	CreateDefaultSubobject():	컴포넌트를 생성하는 용도로 new 키워드 대신 사용
								컴포넌트 구별을 위해 매개변수로 문자열을 받음 (Hash값 생성에 사용하며, 중복되면 안됨)
	TEXT():					모든 플랫폼에서 2byte 문자열 체계(유니코드)를 유지시켜주는 매크로
	RootComponent:			컴포넌트 생성시 반드시 해당 액터를 대표할 RootComponent를 지정해 주어야함
	SetupAttachment():		해당 컴포넌트를 매개변수의 자식으로 설정
	SetRelativeLocation():	부모를 기준으로 컴포넌트의 위치를 설정
	FClassFinder:			에디터에서 블루프린트 클래스를 가져올 때 사용
	FObjectFinder:			에디터에서 리소스를 가져올 때 사용
*/


// 기타
/*
	클래스 접두사:			A:	액터 클래스
							U:	액터가 아닌 클래스
							F:	언리얼 오브젝트와 관련없는 클래스나 구조체
	UPROPERTY():			멤버를 default값으로 초기화하고 에디터에 노출시킴
							사용자가 선언한 언리얼 오브젝트를 자동으로 관리해줌 (garbage collection)
							언리얼 오브젝트가 아니라면, 직접 해제해주거나 스마트포인터를 사용해야함
							VisibleAnywhere:	어디서든 읽기작업 가능
							EditAnywhere:		어디서든 읽기, 쓰기작업 가능
							Category:			지정한 분류에서 멤버를 관리 가능
							AllowPrivateAccess:	private멤버가 블루프린트에 노출됨
	UCLASS():				해당 클래스가 언리얼 오브젝트임을 바깥쪽에 명시
	GENERATED_BODY():		해당 클래스가 언리얼 오브젝트임을 안쪽에 명시
	generated.h:			언리얼 헤더 툴에 의해 자동으로 생성되는 부가파일. 꼭 include 해주어야 함
	모듈명_API:				DLL 내 클래스 정보를 외부에 공개할지 결정하는 _declspec(dllexport) 키워드를 사용하기위해 명시
*/