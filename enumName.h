#pragma once
#include <string_view>
#include <array>
/*Function signature example for template<typename T> void function()  --- function<EnumType>()
 * MSC_VER:               "void __cdecl function<enum EnumType>(void)"
 * __clang__ or __GNUC__: "void function() [with T = EnumType]"
 */

 /*Function signature example for template<typename T, T type> void function()  --- function<EnumType, EnumType::Type1>()
 * MSC_VER:               "void __cdecl function<enum EnumType,EnumType::Type2>(void)"
 * __clang__ or __GNUC__: "void function() [with T = EnumType; T type = EnumType::Type2]"
 */

#if defined(_MSC_VER)
    #define _FunctionSig_  __FUNCSIG__
    #define _EnumTypeNameSigPreSize_ 19
    #define _EnumTypeNameSigPostSize_ 7
    #define _EnumValueNameSigPreSize_ 22
    #define _EnumValueNameSigPostSize_ 7
#elif  defined(__clang__) || defined(__GNUC__)
    #define _FunctionSignature_  __PRETTY_FUNCTION__
    #define _EnumTypeNameSigPreSize_ 28
    #define _EnumTypeNameSigPostSize_ 1
    #define _EnumValueNameSigPreSize_ 41
    #define _EnumValueNameSigPostSize_ 1
#else
    static_assert(false, "not support compiling")
#endif

#define _FunctionNameSize_ sizeof(__FUNCTION__)

template<typename T>
constexpr auto enumTypeName()
{
    constexpr size_t start = _EnumTypeNameSigPreSize_ + _FunctionNameSize_ - 1;
    constexpr size_t end = sizeof(_FunctionSig_) - _EnumTypeNameSigPostSize_ - 1;
    return std::string_view(_FunctionSig_ + start, end - start);
}

template<typename T>
constexpr auto enumTypeSize()
{
    constexpr size_t start = _EnumTypeNameSigPreSize_ + _FunctionNameSize_ - 1;
    constexpr size_t end = sizeof(_FunctionSig_) - _EnumTypeNameSigPostSize_ - 1;
    return end - start;
}

template<typename T, T type>
constexpr auto enumValueName()
{
    constexpr size_t start = _EnumValueNameSigPreSize_ + _FunctionNameSize_ + 2 * enumTypeSize<T>() - 1;
    constexpr size_t end = sizeof(_FunctionSig_) - _EnumValueNameSigPostSize_ - 1;
    return std::string_view(_FunctionSig_ + start, end - start);
}

#ifndef MaxIndex
    #define MaxIndex 1024
#endif

#ifndef MinIndex
    #define MinIndex -1024
#endif

static_assert(MaxIndex > MinIndex);
constexpr size_t MaxEnumElementNum = MaxIndex - MinIndex;

template <typename E, std::size_t... I>
constexpr auto enumNames(std::index_sequence<I...>)
{
    return std::array<std::string_view, sizeof...(I)>{enumValueName<E, static_cast<E>(I)>()...};
}

template<typename T>
inline constexpr auto tabels =  enumNames<T>(std::make_index_sequence<MaxEnumElementNum>());

template<typename T>
constexpr auto enumName(T type)
{
    return tabels<T>[static_cast<size_t>(type)];
}
