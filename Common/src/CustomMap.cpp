// #include "CustomMap.h"
// #include <cassert>
// 
// template<typename TK, typename TV, uint32_t uidIndexBits, uint32_t flagBits>
// CustomMap<TK,TV, uidIndexBits, flagBits>::CustomMap()
// {
// 	int a = sizeof(TK);
// 	assert(uidIndexBits + flagBits <= 8 * sizeof(TK));
// 	flag = 0;
// 	uidIndex = 0;
// }
// 
// template<typename TK, typename TV, uint32_t uidIndexBits, uint32_t flagBits>
// void CustomMap<TK, TV, uidIndexBits, flagBits>::SetFlag(uint32_t flag2)
// {
// 	assert(flag2 < (1 << flagBits));
// 	this->flag = flag2;
// }
// 
// template<typename TK, typename TV, uint32_t uidIndexBits, uint32_t flagBits>
// TK CustomMap<TK, TV, uidIndexBits, flagBits>::AddData(TV value)
// {
// 	int keyBits = 8 * sizeof(TK);
// 	uidIndex++;
// 	TK key = flag << (keyBits - flagBits) | uidIndex;
// 
// 	objMap[key] = value;
// 	return key;
// }
// 
// template<typename TK, typename TV, uint32_t uidIndexBits, uint32_t flagBits>
// TV CustomMap<TK, TV, uidIndexBits, flagBits>::GetData(TK key)
// {
// 	std::map<TK, TV>::iterator iter = objMap.find(key);
// 	if (iter != objMap.end())
// 		return iter->second;
// }
// 
// template<typename TK, typename TV, uint32_t uidIndexBits, uint32_t flagBits>
// void CustomMap<TK, TV, uidIndexBits, flagBits>::DelData(TK key)
// {
// 	objMap.erase(key);
// }
