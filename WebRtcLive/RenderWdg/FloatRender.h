#ifndef VHDESKTOPVIDEO_H
#define VHDESKTOPVIDEO_H
#include "ui_FloatRender.h"
#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include "../Unility/Unility.h"
namespace Ui {
class VHDesktopVideo;
}

class VhallRenderMaskWdg;

class FloatRender : public CBaseWnd
{
    enum Direction {
        UP, DOWN, LEFT, RIGHT, LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP, NONE
    };

    Q_OBJECT

public:
    explicit FloatRender(  QWidget *parent = 0, int iType = 0);
    ~FloatRender();

    virtual void Notice(const CSTDMapParam &);
    CREATE_WND();

    QPaintEngine *paintEngine() const Q_DECL_OVERRIDE;
	void SetRenderMaskWdg(VhallRenderMaskWdg* obj);

    void HandleSubScribeRender(const CSTDMapParam &);
    void HandleSetUserType(const CSTDMapParam &);
    void HandleProcessFloadRender(const CSTDMapParam &mapParam);
    void HandleBigRender(const CSTDMapParam &mapParam);
    void HandleSmallRender(const CSTDMapParam &mapParam);
    void HandlePublishStream(const CSTDMapParam &mapParam);
    void HideTitle();
	void hideRender();
	HWND GetRenderWndID();
   std::shared_ptr<vhall::VideoRenderReceiveInterface> GetRenderWnd();
	void SetMicState(bool open);

	void SetPenState(bool enable);
	void initTrophyRewardNum(const int & iTrophyNum);
	void SetShowNoMic(bool show/*,  bool bSelf*/);
	void SetShowStuNoMic(bool hasAudio);

	void setFixedSize(const QSize &);
   void SetUserName(const QString &userName);
   void SetBigRendering(const bool& big);
public slots:
	void show();
protected:
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void closeEvent(QCloseEvent*);
    void NoticeShowNickName();
    virtual void keyPressEvent(QKeyEvent *e);

	virtual void showEvent(QShowEvent *event);
   virtual void resizeEvent(QResizeEvent *ev);
private:
    void HandlePlayBigRender();

private slots:
    void slot_close();

signals:
    void sig_move();

private:
    Ui::VHDesktopVideo *ui;
    void judgeRegionSetCursor(const QPoint&);
    const int Padding = 2;
    bool isLeftPressDown;  // 判断左键是否按下
    QPoint dragPosition;   // 窗口移动拖动时需要记住的点 
                           // 窗口大小改变时，记录改变方向
    Direction dir;

    bool mbBigRendering = false;
    bool mbSmallRendering = false;
    bool mbBigRenderInit = false;
    bool mbSmallRenderInit = false;

    QString mUserId;
    QString mStreamId;
    QString mNickName;
	int mTrophyRewardNum;
    int mRoleType;

    QString mSmallUserId;
    QString mSmallStreamId;
    QString mSmallNickName;

    bool bStartRenderBig = false;
;
    bool bStartRenderSmall = false;

    //VhallRenderMaskWdg *mpVhallRenderMaskWdg = NULL;

    bool bFullType = false;
};

#endif // VHDESKTOPVIDEO_H
