#pragma once

template <typename T>
class Ptr
{
private:
	T*	m_Asset;

public:
	T* Get() const { return m_Asset; }
	T** GetAdressOf() const { return &m_Asset; }

	bool operator == (T* _Asset) const
	{
		if (m_Asset == _Asset)
			return true;
		return false;
	}

	bool operator == (const  Ptr<T>& _Other) const
	{
		if (m_Asset == _Other.m_Asset)
			return true;
		return false;
	}

	bool operator != (T* _Asset) const
	{
		if (m_Asset == _Asset)
			return false;
		return true;
	}

	bool operator != (const  Ptr<T>& _Other) const
	{
		if (m_Asset == _Other.m_Asset)
			return false;
		return true;
	}

	Ptr& operator = (T* _Asset)
	{
		if (nullptr != m_Asset)
			m_Asset->Release();

		m_Asset = _Asset;

		if(nullptr != m_Asset)
			m_Asset->AddRef();

		return *this;
	}

	Ptr& operator = (const Ptr<T>& _Other)
	{
		if (nullptr != m_Asset)
			m_Asset->Release();

		m_Asset = _Other.m_Asset;

		if (nullptr != m_Asset)
			m_Asset->AddRef();

		return *this;
	}

	T* operator ->() const
	{
		return m_Asset;
	}


public:
	Ptr()
		: m_Asset(nullptr)
	{}

	Ptr(T* _Asset)
		: m_Asset(_Asset)
	{
		if (nullptr != m_Asset)
			m_Asset->AddRef();
	}

	Ptr(const Ptr<T>& _Other)
		: m_Asset(_Other.m_Asset)
	{
		if (nullptr != m_Asset)
			m_Asset->AddRef();
	}

	~Ptr()
	{
		if (nullptr != m_Asset)
			m_Asset->Release();
	}
};


// 전역 operator
template<typename T>
bool operator ==(void* _Left, Ptr<T> _Right)
{
	return _Left == _Right.Get();
}

// 전역 operator
template<typename T>
bool operator !=(void* _Left, Ptr<T> _Right)
{
	return _Left != _Right.Get();
}