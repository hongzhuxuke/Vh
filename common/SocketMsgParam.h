#pragma once
#include <string>

class Event;
class SocketMsgParam
{
public:
    SocketMsgParam();
    ~SocketMsgParam();
    void SocketIOMsgParamToEvent(const std::string& msg, Event* memberInfo);
};

