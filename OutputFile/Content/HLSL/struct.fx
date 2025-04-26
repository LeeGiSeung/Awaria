#ifndef _STRUCT
#define _STRUCT

// Particle
struct tParticle
{
    float4  LocalPos; // 소유 오브젝트로 부터의 상대 좌표
    float4  WorldPos; // 파티클의 월드 좌표
    float4  WorldInitScale;// 파티클 스폰 크기
    float4  WorldScale; // 파티클 월드 크기
    float4  Color; // 파티클 색상

    float4  Force; // 파티클에 주어지고 있는 힘의 총합
    float4  Velocity; // 파티클 속도
    float   Mass; // 파티클 질량
        
    float   NoiseForceAccTime;  // Noise Force 텀에 도달하는 누적 시간
    float3  NoiseForceDir;      // 적용된 Noise Forec 의 방향
    
    
    float   Age; // 파티클 나이, Age 가 Life 에 도달하면 수명이 다한 것
    float   Life; // 파티클 최대 수명
    float   NormalizedAge; // 전체 수명 대비, 현재 Age 비율. 자신의 Age 를 Life 대비 정규화 한 값

    int     Active; // 파티클 활성화 여부
    int     padding[3]; // 파티클 구조체 크기를 16 배수로 맞추기 위한 패딩 바이트
};

// SpawnCount
struct tSpawnCount
{
    int     SpawnCount;
    float3  vPadding;
};

// Particle Module
struct tParticleModule
{
    // Spawn Module
    float	SpawnRate;		// 초당 파티클 생성량
	float4	StartColor;		// 초기 파티클 색상
    float3  MinScale;       // 생성 시 최소 크기
    float3  MaxScale;       // 생성 시 최대 크기
	float	MinLife;		// 생성 시 최소 주기
	float	MaxLife;		// 생성 시 최대 주기
	int		SpawnShape;		// 0 : Box, 1 : Sphere
    float3  SpawnShapeScale;
	int		SpaceType;		// 0 : Local, 1 : World
    
    uint    BlockSpawnShape; // 0 : Box,  1: Sphere
    float3  BlockSpawnShapeScale; // SpawnShapeScale.x == Radius
    
    // Spawn Burst
    uint    SpawnBurstCount;        // 한번에 발생시키는 Particle 수
    uint    SpawnBurstRepeat;       // Burst 횟수
    float   SpawnBurstRepeatTime;   // Brush 간격시간
    
    // Add Velocity
    uint    AddVelocityType; // 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed 
    float3  AddVelocityFixedDir;
    float   AddMinSpeed;
    float   AddMaxSpeed;    
            
    // Scale
    float   StartScale;
    float   EndScale;
    
    // Drag Module (감속 모듈)
    float   DestNormalizedAge;
    float   LimitSpeed;
    
    // Noise Force Module
	float	NoiseForceTerm;		// Noise Force 변경 텀
	float	NoiseForceScale;	// Noise Force 크기
    
    // Render Module
    float4  EndColor;       // 파티클 최종 색상    
    int     FadeOut;        // 0 : Off, 1 : Normalized Age
    float   StartRatio;     // FadeOut 효과가 시작되는 Normalized Age 지점
    uint    VelocityAlignment; // 속도 정렬 0 : Off, 1 : On
    uint    CrossMesh;      // 십자형태 메쉬 추가 0 : Off, 1: On
    
    // 추가 데이터
    float3  ObjectWorldPos;
    
    int     Module[7];   
    
    int     padding;
};



struct tLight2DInfo
{
    int     Type; // 광원 종류
    float3  Color; // 광원 색상
    float3  WorldPos; // 광원 위치	
    float   Radius; // 광원 범위, 반경
    float3  Dir; // 광원의 방향
    float   Angle; // 광원 각도(스포트 라이트)	
};





#endif 