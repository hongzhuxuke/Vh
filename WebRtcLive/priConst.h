#ifndef H_PRICONST_H
#define H_PRICONST_H


//typedef enum VhallInteractionRenderType_e {
//	RenderType_Video = 0,//默认 视频，包括摄像头和桌面共享
//	RenderType_Wait,//等待
//	RenderType_Finished,//结束
//	RenderType_MasterExit,//主持人退出
//	RenderType_Screen,//桌面共享
//	RenderType_Photo,//照片
//}VhallInteractionRenderType;
#define MEMBERLISTCOUNT 25 //按页请求用户列表，每页用户数量

//用户列表tab页
enum eTabMember
{
	eTabMember_onLine = 0,   //
	eTabMember_raiseHands,      //
	eTabMember_prohibitChat,      //
	eTabMember_Count,      //
};
//
//enum eJOIN_ROLE {
//	//参会角色  
//	eJOIN_ROLE_Teacher = 1,// 1 : 老师
//	eJOIN_ROLE_Student,     // 2 ; 学员
//	eJOIN_ROLE_Assistant,   // 3 : 助教
//	eJOIN_ROLE_Guest,        // 4 : 嘉宾
//	eJOIN_ROLE_Supervise,    //5 : 监课
//	eJOIN_ROLE_Admin, // 6 : 管理员
//};

enum eClassType
{
	eClassType_Classing = 1, //上课中
	eClassType_Notice,			//预告
	eClassType_playback,//回放
	eClassType_Relay,//转播
	eClassType_Over,//下课
};

/*举手存在标识*/
enum eHandsUpState
{
	eHandsUpState_No = 0,
	eHandsUpState_existence
};

#define UPPERWHEAT QString::fromLocal8Bit("上麦")
#define INVITATIONTOBARLEY QString::fromLocal8Bit("上麦")
#define DOWNMIC QString::fromLocal8Bit("下麦")
#define AGREESPEAK QString::fromLocal8Bit("同意")

#define UPPERINVITATION QString::fromLocal8Bit("邀请中")
#define UPPERDOWNMICING QString::fromLocal8Bit("下麦中")

#define CANCELPROHIBITCHAT QString::fromLocal8Bit("取消禁言")
#define PROHIBITCHAT QString::fromLocal8Bit("禁言")
#define CANCELKICKOUT QString::fromLocal8Bit("取消踢出")
#define KICKOUT QString::fromLocal8Bit("踢出课堂")

enum eDestopType
{
	eDestopType_Desktop = 3, //电脑屏幕列表
	eDestopType_Windows = 5,	//	获取所有软件列表
   eDestopType_StuDesktop
};

enum WndFloatType {
   WndFloatType_Normal = 0,
   WndFloatType_Float = 1, //浮动窗口
   WndFloatType_Full = 2,  //全屏
};
//const int PageSpanNum = 5;
#endif//H_PRICONST_H