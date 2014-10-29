#pragma once

#include<string>
#include<cstdio>
#include<ctime>
#ifdef _WIN32
#include <Windows.h>
#endif

namespace spitlog
{
namespace details
{
namespace os
{

inline std::tm localtime(const std::time_t &time_tt)
{

#ifdef _WIN32
    std::tm tm;
    localtime_s(&tm, &time_tt);
#else
    std::tm tm;
    localtime_r(&time_tt, &tm);
#endif
    return tm;
}

inline std::tm localtime()
{
    std::time_t now_t = time(0);
    return localtime(now_t);
}


inline std::tm gmtime(const std::time_t &time_tt)
{

#ifdef _WIN32
    std::tm tm;
    gmtime_s(&tm, &time_tt);
#else
    std::tm tm;
    gmtime_r(&time_tt, &tm);
#endif
    return tm;
}

inline std::tm gmtime()
{
    std::time_t now_t = time(0);
    return gmtime(now_t);
}
inline bool operator==(const std::tm& tm1, const std::tm& tm2)
{
    return (tm1.tm_sec == tm2.tm_sec &&
            tm1.tm_min == tm2.tm_min &&
            tm1.tm_hour == tm2.tm_hour &&
            tm1.tm_mday == tm2.tm_mday &&
            tm1.tm_mon == tm2.tm_mon &&
            tm1.tm_year == tm2.tm_year &&
            tm1.tm_isdst == tm2.tm_isdst);
}

inline bool operator!=(const std::tm& tm1, const std::tm& tm2)
{
    return !(tm1 == tm2);
}

#ifdef _WIN32
inline const char* eol()
{
    return "\r\n";
}
#else
constexpr inline const char* eol()
{
    return "\n";
}
#endif

#ifdef _WIN32
inline unsigned short eol_size()
{
    return 2;
}
#else
constexpr inline unsigned short eol_size()
{
    return 1;
}
#endif

//fopen_s on non windows for writing
inline int fopen_s(FILE** fp, const std::string& filename, const char* mode)
{
#ifdef _WIN32
    return ::fopen_s(fp, filename.c_str(), mode);
#else
    *fp = fopen((filename.c_str()), mode);
    return *fp == nullptr;
#endif


}

//Return utc offset in minutes or -1 on failure
inline int utc_minutes_offset(const std::tm& tm = localtime())
{

#ifdef _WIN32
    DYNAMIC_TIME_ZONE_INFORMATION tzinfo;
    auto rv = GetDynamicTimeZoneInformation(&tzinfo);
    if (!rv)
        return -1;
    return -1 * (tzinfo.Bias + tzinfo.DaylightBias);
#else
    return tm.tm_gmtoff / 60;
#endif
}


} //os
} //details
} //spitlog


