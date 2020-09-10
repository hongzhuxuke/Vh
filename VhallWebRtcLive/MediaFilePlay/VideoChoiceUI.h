#ifndef _VIDEOCHOICEUI_H_
#define _VIDEOCHOICEUI_H_
#include <QObject>
#include <QModelIndexList>
#include <QMouseEvent>
#include <QCheckBox>
#include <QTimer>

#include "VHDialog.h"
//#include "vhallmenuwidgetitem.h"
#include "ui_VideoChoiceUI.h"
#include "ui_VideoChoiceItem.h"
#include "../Unility/Unility.h"
#include "CWidget.h"

//class TitleWidget;
class PushButton;
class QListWidget;
class QLabel;
class QComboBox;
class QToolButton;
class QListWidgetItem;
class PushButton;
class NoiseTooltip;
class  QFileDialog;
//class QGraphicsDropShadowEffect;
#define MAXFILECOUNT 20
class VideoChoiceItem;

class VideoChoiceUI : public CBaseWnd {
   Q_OBJECT   

public:
   explicit VideoChoiceUI(QWidget *parent = 0);
   ~VideoChoiceUI();
   virtual void Notice(const CSTDMapParam &mapParam);

   CREATE_WND();

   enum CycleType {
	   CycleTyle_None,
	   CycleTyle_List,
	   CycleTyle_Single,
	   CycleTyle_END
   };
   void StartMeadiePlayTime();

public:
   bool Create();
   void SetTitle(const QString& strTitle);

   void Destroy();
   void Clear();
   QStringList GetPlayList(int &currentIndex);   
   int getCircleMode();   
   void AppendFileList(const QStringList &,int, const bool& bSave = false);
   //void AppendFileList(const std::list<QString>& MediaList, int);
   void SetCircleMode(int mode);
   void CenterWindow(QWidget* parent);

   void FlashTips();
   void FlushItems();
   void FLushFileList(bool accept);

   void SetProgressValue(int nCurValue, int nMaxValue);
   void PlaySelectedFile(bool bCurr = true);
   void UpdatePlayStates(bool bPlay);
   void SetMuteSelf(const bool& bMute);
   void SetMuteStudent(const bool& bMute);
   void CloseWdg();

signals:
   void SigSyncFileList(QStringList,int);
   void sigMuteSelf(bool checked);
   void sig_close();
public slots:
   void PlayListDoubleClicked(const QModelIndex &index);
   void DeleteItem(VideoChoiceItem *);
   void slot_paly(VideoChoiceItem *itemWidget, const int& iState);
   void currentItemChanged(QListWidgetItem*,QListWidgetItem*);
protected:
   virtual void showEvent(QShowEvent *);
   virtual bool eventFilter(QObject *obj, QEvent *ev);
private slots:
	void ChoiceFiles(const QStringList &selected);
	
	//播放进度改变
	void OnProgressChanged(int);
	void OnCyclePlay();
	void on_ChkBoxMuteSelf_toggled(bool checked);
	void on_ChkBoxMuteStudent_toggled(bool checked);
	void on_btnPlay_clicked(bool checked = false);
	void on_btnCircleType_clicked(bool checked = false);
	void on_btnClose_clicked(bool checked = false);
	void slot_FileDialogChoose();
	void on_btnMinimize_clicked(bool checked = false);
private:
	
	void SetPlayState(VideoChoiceItem *itemWidget);
	void StopPlayMediaFile();
	int GetCurVolume();
	bool GetCurFile(QString& selectFile);
	int InitMediaStream(const char *szMediaFile);
	int OpenPlayMediaFile(const char *szMediaFile);
	void DealStopPlay(/*void* apData, DWORD adwLen*/);
	bool IsArriveTheEnd();
	void SwitchToNext();
	int GetCurPlayMode();
	void UpdatePlayTime(QString curT, QString fulT);
	

	QString dulation2Str(long long currDulation, long long maxDulation,
	QString& strcurrDulation, QString& strmaxDulation);
	QString dulation2StrPri(long long dulation);

	void MediaFilePlayFaild();
	int GetPlayFileState(signed long long& n64Pos, signed long long& n64MaxPos);

	bool IsFileInList(const QString &fullFilename);
	bool InsertFileList(const QString &filename, const bool& bSave);

	void initItem();

	Ui::VideoChoiceUI ui;
   //TitleWidget *m_pTitleBar = NULL;

   QLabel *mFileTypeTips = NULL;

   bool m_bPaintItem = true;
   int mLiveType = 0;
   int miCycleType = CycleTyle_List;

   bool m_bIsMute;
   bool m_bPlayCurrentFile = false;
   //循环播放定时器
   QTimer m_qCycleTimer;
   unsigned int m_iVolume;

   VideoChoiceItem* mpCurPlayItem = NULL;
   QFileDialog* mFileDlg = nullptr;

   bool m_bChkBoxMuteSelf = false;
   bool m_bChkBoxMuteStudent = false;
   //QGraphicsDropShadowEffect* mpEffect = nullptr;
   //QGraphicsDropShadowEffect* mpEffectLeft = nullptr;
   bool mbMin = false;

   NoiseTooltip *m_pNoiseTips = nullptr;
   int miItemWidth = 0;
};


class VideoChoiceItem : public CWidget
{
	Q_OBJECT

public:

	explicit VideoChoiceItem(QWidget *parent = NULL);
	~VideoChoiceItem();
	void setFileName(const QString &);
	void setWidth(int);
	QString getFilename();

	void setEndLine(bool);

	void setChecked(bool);
	void FlushLabelColor(bool pressed);

	void SetPlayState(int iState);

	enum ePlayState {
		ePlayState_Unplay, //未播放
		ePlayState_Playing, //播放中
		ePlayState_suspend //暂停
	};
protected:
	//virtual void paintEvent(QPaintEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mousePressEvent(QMouseEvent *);
	virtual void leaveEvent(QEvent *);
public slots:
	
signals:
	void closeClicked(VideoChoiceItem *);
	void sig_paly(VideoChoiceItem*, const int& iState);
private slots:
	void closeBtnClicked();
	void slot_Play();
private:

	Ui::VideoChoiceItem ui;
	QString m_strFilePath;

	bool m_bDrawLine = false;
	bool m_bChecked = false;
	int miPlayState = ePlayState_Unplay;  //播放状态
  
};

#endif
