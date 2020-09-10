#include "CBaseRender.h"

CBaseRender::CBaseRender()
{
}

CBaseRender::~CBaseRender()
{
}

void CBaseRender::SetUserJoinID(QString id) {
    if (id.isEmpty()) {
        return;
    }
    mUserJoinId = id;
}

QString CBaseRender::GetUserJoinID() {
    return mUserJoinId;
}

void CBaseRender::SetStreamID(QString id) {
    if (id.isEmpty()) {
        return;
    }
    mStreamId = id;
}

QString CBaseRender::GetStreamID() {
    return mStreamId;
}

void CBaseRender::SetStreamHasVideoFormat(bool has) {
    mbHasVideoFormat = has;
}

bool CBaseRender::HasVideoFormat() {
    return mbHasVideoFormat;
}

void CBaseRender::SetStreamHasAudioFormat(bool has) {
    mbHasAudioFormat = has;
}

bool CBaseRender::HasAudioFormat() {
    return mbHasAudioFormat;
}


void CBaseRender::SetSmallWndUserJoinID(QString id) {
    if (id.isEmpty()) {
        return;
    }
    mSmallWndUserJoinId = id;
}

QString CBaseRender::GetSmallWndUserJoinID() {
    return mSmallWndUserJoinId;
}

void CBaseRender::SetSmallWndStreamID(QString id) {
    if (id.isEmpty()) {
        return;
    }
    mSmallWndStreamId = id;
}

QString CBaseRender::GetSmallWndStreamID() {
    return mSmallWndStreamId;
}

void CBaseRender::SetSmallWndStreamHasVideoFormat(bool has) {
    mbSmallWndHasVideoFormat = has;
}

bool CBaseRender::HasSmallWndVideoFormat() {
    return mbSmallWndHasVideoFormat;
}

void CBaseRender::SetSmallWndStreamHasAudioFormat(bool has) {
    mbSmallWndHasAudioFormat = has;
}
bool CBaseRender::HasSmallWndAudioFormat() {
    return mbSmallWndHasAudioFormat;
}