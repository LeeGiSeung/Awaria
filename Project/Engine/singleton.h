#pragma once

template<typename T>
class singleton
{
private:
	static T*	g_This;

public:
	static T* GetInst()
	{
		if (nullptr == g_This)
		{
			g_This = new T;
		}

		return g_This;
	}

	static void Destroy()
	{
		if (nullptr != g_This)
		{
			delete g_This;
			g_This = nullptr;
		}
	}

protected:
	singleton()
	{
		typedef void(*FUNC)(void);
		atexit((FUNC)&singleton::Destroy);
	}

	~singleton()
	{}
};

// ���� �ɹ� �ʱ�ȭ �� ����
template<typename T>
T* singleton<T>::g_This = nullptr;