#pragma once


#define SINGLE(type)	friend class singleton<type>;\
						type& operator = (const type& _Other) = delete;\
						type();\
						type(const type& _Origin) = delete;\
						~type();

#define DEVICE	CDevice::GetInst()->GetDevice().Get()
#define CONTEXT CDevice::GetInst()->GetContext().Get()

#define KEY_CHECK(key)		CKeyMgr::GetInst()->GetKeyState(key)
#define KEY_TAP(key)		(CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::TAP)
#define KEY_PRESSED(key)	(CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::PRESSED)
#define KEY_RELEASED(key)	(CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::RELEASED)
#define KEY_NONE(key)		(CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::NONE)

#define MAX_LAYER 32

#define CLONE(Type) virtual Type* Clone() override { return new Type(*this); }
#define CLONE_DISABLE(Type) virtual Type* Clone() override {return nullptr;}\
							private: Type(const Type& _Other) = delete;

#define DT			CTimeMgr::GetInst()->GetDeltaTime()
#define TIME		CTimeMgr::GetInst()->GetTime()
#define EngineDT	CTimeMgr::GetInst()->GetEngineDeltaTime()
#define EngineTIME	CTimeMgr::GetInst()->GetEngineTime()

#define LEVEL_LOBBY L"Lobby_Level"
#define LEVEL_0 L"0_Level"
#define LEVEL_BOSS L"Boss_Level"


typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;