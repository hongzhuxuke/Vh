#ifndef H_PRICONST_H
#define H_PRICONST_H


//typedef enum VhallInteractionRenderType_e {
//	RenderType_Video = 0,//Ĭ�� ��Ƶ����������ͷ�����湲��
//	RenderType_Wait,//�ȴ�
//	RenderType_Finished,//����
//	RenderType_MasterExit,//�������˳�
//	RenderType_Screen,//���湲��
//	RenderType_Photo,//��Ƭ
//}VhallInteractionRenderType;
#define MEMBERLISTCOUNT 25 //��ҳ�����û��б�ÿҳ�û�����

//�û��б�tabҳ
enum eTabMember
{
	eTabMember_onLine = 0,   //
	eTabMember_raiseHands,      //
	eTabMember_prohibitChat,      //
	eTabMember_Count,      //
};
//
//enum eJOIN_ROLE {
//	//�λ��ɫ  
//	eJOIN_ROLE_Teacher = 1,// 1 : ��ʦ
//	eJOIN_ROLE_Student,     // 2 ; ѧԱ
//	eJOIN_ROLE_Assistant,   // 3 : ����
//	eJOIN_ROLE_Guest,        // 4 : �α�
//	eJOIN_ROLE_Supervise,    //5 : ���
//	eJOIN_ROLE_Admin, // 6 : ����Ա
//};

enum eClassType
{
	eClassType_Classing = 1, //�Ͽ���
	eClassType_Notice,			//Ԥ��
	eClassType_playback,//�ط�
	eClassType_Relay,//ת��
	eClassType_Over,//�¿�
};

/*���ִ��ڱ�ʶ*/
enum eHandsUpState
{
	eHandsUpState_No = 0,
	eHandsUpState_existence
};

#define UPPERWHEAT QString::fromLocal8Bit("����")
#define INVITATIONTOBARLEY QString::fromLocal8Bit("����")
#define DOWNMIC QString::fromLocal8Bit("����")
#define AGREESPEAK QString::fromLocal8Bit("ͬ��")

#define UPPERINVITATION QString::fromLocal8Bit("������")
#define UPPERDOWNMICING QString::fromLocal8Bit("������")

#define CANCELPROHIBITCHAT QString::fromLocal8Bit("ȡ������")
#define PROHIBITCHAT QString::fromLocal8Bit("����")
#define CANCELKICKOUT QString::fromLocal8Bit("ȡ���߳�")
#define KICKOUT QString::fromLocal8Bit("�߳�����")

enum eDestopType
{
	eDestopType_Desktop = 3, //������Ļ�б�
	eDestopType_Windows = 5,	//	��ȡ��������б�
   eDestopType_StuDesktop
};

enum WndFloatType {
   WndFloatType_Normal = 0,
   WndFloatType_Float = 1, //��������
   WndFloatType_Full = 2,  //ȫ��
};
//const int PageSpanNum = 5;
#endif//H_PRICONST_H