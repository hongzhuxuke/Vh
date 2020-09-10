#include "stdafx.h"
#include "Msg_CommonToolKit.h"


static bool common_have_clockfreq = false;
static LARGE_INTEGER common_clock_freq;

static inline uint64_t get_clockfreq(void)
{
    if (!common_have_clockfreq)
        QueryPerformanceFrequency(&common_clock_freq);
    return common_clock_freq.QuadPart;
}

uint64_t common_os_gettime_ns(void)
{
    LARGE_INTEGER current_time;
    double time_val;
    QueryPerformanceCounter(&current_time);
    time_val = (double)current_time.QuadPart;
    time_val *= 1000000000.0;
    time_val /= (double)get_clockfreq();
    return (uint64_t)time_val;
}

STRU_HTTPCENTER_HTTP_RQ::STRU_HTTPCENTER_HTTP_RQ() {
   memset(this, 0, sizeof(STRU_HTTPCENTER_HTTP_RQ));
}

STRU_HTTPCENTER_HTTP_RQ& STRU_HTTPCENTER_HTTP_RQ::operator=(const STRU_HTTPCENTER_HTTP_RQ& aoSrc) {
   m_i64UserData = aoSrc.m_i64UserData;
   m_dwPluginId = aoSrc.m_dwPluginId;
   wcsncpy(m_wzRequestUrl, aoSrc.m_wzRequestUrl, DEF_MAX_HTTP_URL_LEN);
   return*this;
}

STRU_HTTPCENTER_HTTP_RS::STRU_HTTPCENTER_HTTP_RS() {
   memset(this, 0, sizeof(STRU_HTTPCENTER_HTTP_RS));
   m_uuid = common_os_gettime_ns(); 
}

STRU_HTTPCENTER_HTTP_RS& STRU_HTTPCENTER_HTTP_RS::operator=(const STRU_HTTPCENTER_HTTP_RS& aoSrc) {
   memset(this, 0, sizeof(STRU_HTTPCENTER_HTTP_RS));
   m_i64UserData = aoSrc.m_i64UserData;
   m_dwPluginId = aoSrc.m_dwPluginId;
   wcsncpy(m_wzRequestUrl, aoSrc.m_wzRequestUrl, DEF_MAX_HTTP_URL_LEN);
   wcsncpy(m_wzUrlData, aoSrc.m_wzUrlData, DEF_MAX_HTTP_URL_LEN);
   m_bIsSuc = aoSrc.m_bIsSuc;
   memcpy(m_uid, aoSrc.m_uid, DEF_MAX_HTTP_URL_LEN);
   bHasVideo = aoSrc.bHasVideo;
   m_uuid = aoSrc.m_uuid;
   bActiveApi = aoSrc.bActiveApi;
   return *this;
}