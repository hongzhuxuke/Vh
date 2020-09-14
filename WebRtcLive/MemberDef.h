#pragma once
typedef enum VhallInteractionRenderType_e {
    RenderType_Video = 0,//默认 视频，包括摄像头和桌面共享
    RenderType_Wait,//等待
    RenderType_Finished,//结束
    RenderType_MasterExit,//主持人退出
    RenderType_Screen,//桌面共享
    RenderType_Photo,//照片
}VhallInteractionRenderType;

//用户列表tab页
enum eTabMember
{
    eTabMember_onLine = 0,   //
    eTabMember_raiseHands,      //
    eTabMember_prohibitChat,      //
    eTabMember_kickOut,      //
    eTabMember_Count,      //
};

#define UPPERMIC QString::fromLocal8Bit("邀请上麦")
#define DOWNMIC QString::fromLocal8Bit("下麦")
#define UPPERINVITATION QString::fromLocal8Bit("邀请中")
#define UPPERDOWNMICING QString::fromLocal8Bit("下麦中")

#define CANCELPROHIBITCHAT QString::fromLocal8Bit("取消禁言")
#define PROHIBITCHAT QString::fromLocal8Bit("禁言")
#define CANCELKICKOUT QString::fromLocal8Bit("取消踢出")
#define KICKOUT QString::fromLocal8Bit("踢出")
const int PageSpanNum = 5;