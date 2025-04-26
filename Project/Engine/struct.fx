#ifndef _STRUCT
#define _STRUCT

// Particle
struct tParticle
{
    float4  LocalPos; // ���� ������Ʈ�� ������ ��� ��ǥ
    float4  WorldPos; // ��ƼŬ�� ���� ��ǥ
    float4  WorldInitScale;// ��ƼŬ ���� ũ��
    float4  WorldScale; // ��ƼŬ ���� ũ��
    float4  Color; // ��ƼŬ ����

    float4  Force; // ��ƼŬ�� �־����� �ִ� ���� ����
    float4  Velocity; // ��ƼŬ �ӵ�
    float   Mass; // ��ƼŬ ����
        
    float   NoiseForceAccTime;  // Noise Force �ҿ� �����ϴ� ���� �ð�
    float3  NoiseForceDir;      // ����� Noise Forec �� ����
    
    
    float   Age; // ��ƼŬ ����, Age �� Life �� �����ϸ� ������ ���� ��
    float   Life; // ��ƼŬ �ִ� ����
    float   NormalizedAge; // ��ü ���� ���, ���� Age ����. �ڽ��� Age �� Life ��� ����ȭ �� ��

    int     Active; // ��ƼŬ Ȱ��ȭ ����
    int     padding[3]; // ��ƼŬ ����ü ũ�⸦ 16 ����� ���߱� ���� �е� ����Ʈ
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
    float	SpawnRate;		// �ʴ� ��ƼŬ ������
	float4	StartColor;		// �ʱ� ��ƼŬ ����
    float3  MinScale;       // ���� �� �ּ� ũ��
    float3  MaxScale;       // ���� �� �ִ� ũ��
	float	MinLife;		// ���� �� �ּ� �ֱ�
	float	MaxLife;		// ���� �� �ִ� �ֱ�
	int		SpawnShape;		// 0 : Box, 1 : Sphere
    float3  SpawnShapeScale;
	int		SpaceType;		// 0 : Local, 1 : World
    
    uint    BlockSpawnShape; // 0 : Box,  1: Sphere
    float3  BlockSpawnShapeScale; // SpawnShapeScale.x == Radius
    
    // Spawn Burst
    uint    SpawnBurstCount;        // �ѹ��� �߻���Ű�� Particle ��
    uint    SpawnBurstRepeat;       // Burst Ƚ��
    float   SpawnBurstRepeatTime;   // Brush ���ݽð�
    
    // Add Velocity
    uint    AddVelocityType; // 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed 
    float3  AddVelocityFixedDir;
    float   AddMinSpeed;
    float   AddMaxSpeed;    
            
    // Scale
    float   StartScale;
    float   EndScale;
    
    // Drag Module (���� ���)
    float   DestNormalizedAge;
    float   LimitSpeed;
    
    // Noise Force Module
	float	NoiseForceTerm;		// Noise Force ���� ��
	float	NoiseForceScale;	// Noise Force ũ��
    
    // Render Module
    float4  EndColor;       // ��ƼŬ ���� ����    
    int     FadeOut;        // 0 : Off, 1 : Normalized Age
    float   StartRatio;     // FadeOut ȿ���� ���۵Ǵ� Normalized Age ����
    uint    VelocityAlignment; // �ӵ� ���� 0 : Off, 1 : On
    uint    CrossMesh;      // �������� �޽� �߰� 0 : Off, 1: On
    
    // �߰� ������
    float3  ObjectWorldPos;
    
    int     Module[7];   
    
    int     padding;
};



struct tLight2DInfo
{
    int     Type; // ���� ����
    float3  Color; // ���� ����
    float3  WorldPos; // ���� ��ġ	
    float   Radius; // ���� ����, �ݰ�
    float3  Dir; // ������ ����
    float   Angle; // ���� ����(����Ʈ ����Ʈ)	
};





#endif 