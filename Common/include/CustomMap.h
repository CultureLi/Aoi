#pragma once
#ifndef CUSTOM_MAP
#define CUSTOM_MAP
#include <map>
#include <cassert>

namespace Custom_Develop
{

	template<typename TK, typename TV, uint32_t uidIndexBits, uint32_t flagBits>
	class CustomMap
	{
		// public:
		// 	CustomMap();
		// 	void SetFlag(uint32_t flag2);
		// 	TK AddData(TV value);
		// 	TV GetData(TK key);
		// 	void DelData(TK key);
		// 
	private:
		uint32_t flag; //用于生成的uid高32为
		uint32_t uidIndex;//自增，uid的低32位
	public:
		std::map<TK, TV> objMap;
	public:
		CustomMap()
		{
			int a = sizeof(TK);
			assert(uidIndexBits + flagBits <= 8 * sizeof(TK));
			flag = 0;
			uidIndex = 0;
		}


		void SetFlag(uint32_t flag2)
		{
			assert(flag2 < (1 << flagBits));
			this->flag = flag2;
		}

		TK AddData(TV value)
		{
			int keyBits = 8 * sizeof(TK);
			uidIndex++;
			TK key = flag << (keyBits - flagBits) | uidIndex;

			objMap[key] = value;
			return key;
		}

		//template<typename TK, typename TV, uint32_t uidIndexBits, uint32_t flagBits>
		TV GetData(TK key)
		{
			auto iter = objMap.find(key);
			if (iter != objMap.end())
				return iter->second;
			return nullptr;
		}




		void DelData(TK key)
		{
			objMap.erase(key);
		}


	};
}

#endif // !CUSTOM_MAP



