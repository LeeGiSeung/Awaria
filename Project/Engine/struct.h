#pragma once



// Vertex
// ��ü�� ���¸� �̷�� ���� �⺻ ����
struct Vtx
{	
	Vec3	vPos;		// ������ ��ġ
	Vec4	vColor;		// ������ ����	
	Vec2	vUV;		// ������ ������ �ؽ����� ��ġ(��ǥ)
};

// DebugShapeInfo
struct tDebugShapeInfo
{
	DEBUG_SHAPE Shape;
	Vec3	    WorldPos;
	Vec3		WorldScale;
	Vec3		WorldRotation;
	Matrix		MatWorld;
	Vec4		Color;
	float		CurTime;
	float		Duration;
	bool		DepthTest;
};

struct tTask
{
	TASK_TYPE Type;
	DWORD_PTR Param0;
	DWORD_PTR Param1;
};

struct tLight2DInfo
{
	LIGHT_TYPE	Type;		// ���� ����
	Vec3		Color;		// ���� ����
	Vec3		WorldPos;	// ���� ��ġ	
	float		Radius;		// ���� ����, �ݰ�
	Vec3		Dir;		// ������ ����
	float		Angle;		// ���� ����(����Ʈ ����Ʈ)	
};


// =============
// Particle ����
// =============
struct tParticle
{
	Vec4	LocalPos;		// ���� ������Ʈ�� ������ ��� ��ǥ
	Vec4	WorldPos;		// ��ƼŬ�� ���� ��ǥ
	Vec4	WorldInitScale; // ��ƼŬ ���� �� �ʱ� ũ��
	Vec4	WorldScale;		// ��ƼŬ ���� ũ��
	Vec4	Color;			// ��ƼŬ ����

	Vec4	Force;			// ��ƼŬ�� �־����� �ִ� ���� ����
	Vec4	Velocity;		// ��ƼŬ �ӵ�
	float	Mass;			// ��ƼŬ ����

	float   NoiseForceAccTime;  // Noise Force �ҿ� �����ϴ� ���� �ð�
	Vec3	NoiseForceDir;      // ����� Noise Forec �� ����

	float	Age;			// ��ƼŬ ����, Age �� Life �� �����ϸ� ������ ���� ��
	float	Life;			// ��ƼŬ �ִ� ����
	float	NormalizedAge;  // ��ü ���� ���, ���� Age ����. �ڽ��� Age �� Life ��� ����ȭ �� ��

	int		Active;			// ��ƼŬ Ȱ��ȭ ����
	int		padding[3];		// ��ƼŬ ����ü ũ�⸦ 16 ����� ���߱� ���� �е� ����Ʈ
};

struct tSpawnCount
{
	int		SpawnCount;
	Vec3	vPadding;
};

// Particle Module
struct tParticleModule
{
	// Spawn Modlue
	float	SpawnRate;			// �ʴ� ��ƼŬ ������
	Vec4	StartColor;			// �ʱ� ��ƼŬ ����

	Vec3	MinScale;			// ���� �� �ּ� ũ��
	Vec3	MaxScale;			// ���� �� �ִ� ũ��
	float	MinLife;			// ���� �� �ּ� �ֱ�
	float	MaxLife;			// ���� �� �ִ� �ֱ�
	int		SpawnShape;			// 0 : Box, 1 : Sphere
	Vec3	SpawnShapeScale;
	int		SpaceType;			// 0 : Local, 1 : World

	UINT	BlockSpawnShape;		// 0 : Box,  1: Sphere
	Vec3	BlockSpawnShapeScale;	// SpawnShapeScale.x == Radius

	// Spawn Burst
	UINT	SpawnBurstCount;		// �ѹ��� �߻���Ű�� Particle ��
	UINT	SpawnBurstRepeat;
	float	SpawnBurstRepeatTime;


	// Add Velocity
	UINT	AddVelocityType;		// 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed 
	Vec3	AddVelocityFixedDir;
	float	AddMinSpeed;
	float	AddMaxSpeed;
	 
	// Scale Module
	float	StartScale;
	float	EndScale;

	// Drag Module (���� ���)
	float	DestNormalizedAge;
	float	LimitSpeed;

	// Noise Force Module
	float	NoiseForceTerm;		// Noise Force ���� ��
	float	NoiseForceScale;	// Noise Force ũ��

	// Render Module
	Vec4	EndColor;			// ��ƼŬ ���� ����
	int		FadeOut;			// 0 : Off, 1 : Normalized Age
	float   StartRatio;			// FadeOut ȿ���� ���۵Ǵ� Normalized Age ����
	UINT	VelocityAlignment;  // �ӵ� ���� 0 : Off, 1 : On
	UINT	CrossMesh;			// �������� �޽� ��� 0 : Off, 1 : ON

	// �߰� ������
	Vec3	ObjectWorldPos;

	// Module On / Off
	int		Module[(UINT)PARTICLE_MODULE::END];

	int		padding[1];
};


// ===================
// ������� ���� ����ü
// ===================
struct tTransform
{
	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;

	Matrix	matWV;
	Matrix	matWVP;
};
extern tTransform g_Trans;

// ������ ���ؼ� ���޵Ǵ� ���
struct tMtrlConst
{
	int		iArr[4];
	float	fArr[4];
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix	matArr[4];

	//�ؽ��� ���ε� ����
	int		bTex[TEX_END];
	int		padding[2];
};

// Global Data
struct tGlobal
{
	Vec2	g_Resolution;

	float   g_DT;
	float   g_EngineDT;

	float   g_Time;
	float   g_EngineTime;

	int     g_Light2DCount;
	int     g_Light3DCount;
};
extern tGlobal g_global;