#pragma once

class SpinLock
{
public:
	SpinLock() : _count(0)
	{ }
	
	inline void Lock(volatile unsigned long long *pTarget)
	{
		while (1)
		{
			if(InterlockedExchange(pTarget, 1) == enNotUsingResource)
				break;

			++_count;

			if (_count > 4000)
			{
				_count = 0;
				YieldProcessor;
			}
		}
	}
	void UnLock(volatile unsigned long long* pTarget)
	{
		InterlockedExchange(pTarget, enNotUsingResource);
	}
private:
	enum
	{
		enNotUsingResource = 0, enUsingResource
	};

	int _count;
};