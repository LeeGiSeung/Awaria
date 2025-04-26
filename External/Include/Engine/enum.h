#pragma once

enum MOVE_DIR {
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
};

enum DRONE_STATE {
	DRONE_SPAWN,
	DRONE_DEATH,
	DRONE_LIVE
};

enum BOSS_VECTOR_STATE {
	BOSS_VECTOR_START,
	BOSS_VECTOR_SPAWN_SKUL,
	BOSS_VECTOR_SPAWN_BOMB,
	BOSS_VECTOR_SPAWN_RHINO
};

enum BIG_RHINO_STATE {
	BIG_RHINO_SPAWN,
	BIG_RHINO_WALK,
	BIG_RHINO_ATTACK,
	BIG_RHINO_DEATH
};

enum BOMBSKUL_STATE {
	BOMBSKUL_SPAWN,
	BOMBSKUL_WALK,
	BOMBSKUL_ATTACK,
	BOMBSKUL_DEATH
};

enum SKUL_STATE {
	SKUL_SPAWN,
	SKUL_READY,
	SKUL_RUN,
	SKUL_DEATH
};

enum SMALL_RHINO_STATE {
	SMALL_RHINO_SPAWN,
	SMALL_RHINO_WALK,
	SMALL_RHINO_ATTACK,
	SMALL_RHINO_DEATH
};

enum PNG_PLAYER_MOVE {
	RIGHTUP,
	RIGHTDOWN,
	LEFTUP,
	LEFTDOWN
};

enum ZMORA_STATE {
	ZMORA_SPAWN,
	ZMORA_WALK,
	ZMORA_SWING,
	ZMORA_TAKEDOWN
};

enum PART {
	NONE_HAND,
	GEAR,
	COPPER,
	CARBON,
	MACHINED_COPPER,
	MACHINED_CARBON,
	PART_END
};

enum GENERATOR_UI_STATE {
	GENERATOR_UI_QUESTION,
	GENERATOR_UI_SUCCESS,
	GENERATOR_UI_PART,
	GENERATOR_UI_NONE
};

enum GENERATOR_STATE {
	GENERATOR_BREAK_START,
	GENERATOR_BREAK_ING,
	GENERATOR_FIX_START,
	GENERATOR_FIX_ING,
	GENERATOR_STATE_END
};

enum APPARATUS_STATE {
	APPARATUS_END,
	APPARATUS_ING,
	APPARATUS_START,
	APPARATUS_READY,
	APPARATUS_STATE_END

};

enum APPARATUS_TYPE {
	APPARATUS_GEAR,
	APPARATUS_COPPER,
	APPARATUS_CARBON,
	APPARATUS_MACHINED_COPPER,
	APPARATUS_MACHINED_CARBON,
	APPARATUS_TYPE_END
};

enum PLAYER_STATE {
	PLAYER_IDLE,
	PLAYER_MOVE,
	PLAYER_DASH,
	PLAYER_DEATH,
	PLAYER_STATE_END
};

enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL,		// b2

	END,
};


// Rasterizer State
enum class RS_TYPE
{
	CULL_BACK, // cull_ccw, �޸�(�ݽð� ����) �ø�
	CULL_FRONT, // cull_cw
	CULL_NONE,  // �ø����� ����
	WIRE_FRAME, // �ø����� ����, 

	END,
};

// DepthStencilState
enum class DS_TYPE
{
	LESS,				// �������� : �� �۾ƾ� ���, ���� ��� O
	LESS_EQUAL,			// �������� : �� �۾ƾ� ������ ���, ���� ��� O

	GREATER,			// �������� : �� �־�� ���, ���� ��� 0

	NO_WRITE,			// �������� : �� �۾ƾ� ���(LESS), ���� ��� X
	NO_TEST_NO_WIRITE,	// �������� �׻� ����(�˻�X), ���� ��� X

	END,
};

// BlendState
enum class BS_TYPE
{
	DEFAULT,	// Src : 1, Dst : 0
	ALPHABLEND, // Src : A, Dst : (1 - A)
	ALPHABLEND_COVERAGE,
	ONE_ONE,

	END,
};

enum class LEVEL_STATE
{
	PLAY,
	STOP,
	PAUSE,
};

enum COLLISION_LAYER
{
	TILE,
	WALL,
	TRAP,
	GHOST,
	GENERATOR,
	PLAYER,
	END
};
extern const char* COLLISION_LAYER_STR[];
extern const wchar_t* COLLISION_LAYER_WSTR[];

enum COMPONENT_TYPE
{
	TRANSFORM,
	COLLIDER2D,
	COLLIDER3D,
	LIGHT2D,
	LIGHT3D,
	CAMERA,
	BOUNDINGBOX,

	// Rendering Component
	MESHRENDER,			// ���� �⺻���� RenderComponent
	SPRITERENDER,		// Sprite �ϳ� �������ϴ� ������Ʈ
	FLIPBOOKRENDER,		// Flipbook ��� ������Ʈ
	TILERENDER,			// Ÿ�ϸ� ������
	PARTICLERENDER,		// ���� ������
	SKYBOX,				// 3D ���
	DECAL,				// ����
	LANDSCAPE,			// 3D ����

	COMPONENT_END,

	SCRIPT,				// �뺻, ����, ������
};
extern const char* COMPONENT_TYPE_STR[];
extern const wchar_t* COMPONENT_TYPE_WSTR[];

enum ASSET_TYPE
{
	MESH,
	MESHDATA,
	TEXTURE,
	MATERIAL,
	PREFAB,
	SOUND,
	GRAPHIC_SHADER,	// ������
	COMPUTE_SHADER, // ���, GP(General Purpose)
	SPRITE,
	FLIPBOOK,
	ASSET_END,
};
extern const char* ASSET_TYPE_STR[];
extern const wchar_t* ASSET_TYPE_WSTR[];

// ����(Projection) ���
enum PROJ_TYPE
{
	ORTHOGRAPHIC,	// ��������
	PERSPECTIVE,	// ��������
};


// ����
enum class DIR
{
	RIGHT,
	UP,
	FRONT,
	END,
};

enum class SHADER_DOMAIN
{
	DOMAIN_QPAQUE,		// ������
	DOMAIN_MASKED,		// ������ + ���� ����
	DOMAIN_TRANSPARENT, // ������ + ����	
	DOMAIN_POSTPROCESS, // ��ó��

	DOMAIN_DEBUG,		// ����� ������
};

enum SCALAR_PARAM
{
	INT_0, INT_1, INT_2, INT_3,
	FLOAT_0, FLOAT_1, FLOAT_2, FLOAT_3,
	VEC2_0, VEC2_1, VEC2_2, VEC2_3,
	VEC4_0, VEC4_1, VEC4_2, VEC4_3,
	MAT_0, MAT_1, MAT_2, MAT_3,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEX_END,
};


enum class DEBUG_SHAPE
{
	// 2D
	RECT,
	CIRCLE,
	LINE,

	// 3D
	CUBE,
	SPHERE,
};

enum class TASK_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	ADD_CHILD,
	CHANGE_LEVEL_STATE,
	CHANGE_LEVEL,
	DELETE_ASSET,
};

enum class LIGHT_TYPE
{
	DIRECTIONAL,	// ���⼺ ����, ���� (�¾�, ��)
	POINT,			// ������, ����, ����, ȶ��
	SPOT,			// ����Ʈ����Ʈ, ������
};

// ��ƼŬ�� ����� ������ ��ɵ�
enum class PARTICLE_MODULE
{
	SPAWN,
	SPAWN_BURST,
	ADD_VELOCITY,
	SCALE,
	DRAG,
	NOISE_FORCE,
	RENDER,

	END,
};