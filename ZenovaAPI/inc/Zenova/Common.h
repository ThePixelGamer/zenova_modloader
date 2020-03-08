#include <cstdint>
#include <stdexcept>

#ifdef ZENOVA_EXPORT
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport) 
#endif

using exception = std::runtime_error;

//unsigned int types
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

//signed int types
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;