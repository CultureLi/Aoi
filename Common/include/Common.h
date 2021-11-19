#pragma once
#ifndef C_COMMON
#define C_COMMON


#include <string>
#include <vector>
#include <map>

#define GENUUID64(high,low) ((uint64_t)(high) << 32) | ((uint64_t)(low))
#define LOWUUID64(v) ((uint32_t)(v))
#define HIGHUUID64(v) ((uint32_t)((uint64_t)(v) >> 32) & 0xFFFFFFFF)


#define SAFE_DELETE(p) {if(p) {delete (p);(p)=nullptr;}}


template<typename TK, typename TV>
std::vector<TK> extract_keys(std::map<TK, TV> const& input_map)
{
	std::vector<TK> retval;
	for (auto const& element : input_map) {
		retval.push_back(element.first);
	}
	return retval;
}




template<typename ... Args>
static std::string str_format(const std::string& format, Args ... args)
{
	auto size_buf = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	std::unique_ptr<char[]> buf(new(std::nothrow) char[size_buf]);

	if (!buf)
		return std::string("");

	std::snprintf(buf.get(), size_buf, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size_buf - 1);
}


template<typename ... Args>
static std::wstring wstr_format(const std::wstring& format, Args ... args)
{
	auto size_buf = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	std::unique_ptr<char[]> buf(new(std::nothrow) char[size_buf]);

	if (!buf)
		return std::wstring("");

	std::snprintf(buf.get(), size_buf, format.c_str(), args ...);
	return std::wstring(buf.get(), buf.get() + size_buf - 1);
}


#endif // C_COMMON