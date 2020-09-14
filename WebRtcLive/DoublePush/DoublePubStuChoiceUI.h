#ifndef _DOUBLEPUBSTUCHOICEUI_H_
#define _DOUBLEPUBSTUCHOICEUI_H_
#include <QObject>
#include <QModelIndexList>
#include <QMouseEvent>
#include "../Unility/Unility.h"

#include "ui_DoublePubStuChoiceUI.h"
#include "ui_DoublePubStuChoiceItem.h"

#include "CWidget.h"


class DoublePubStuChoiceItem;
class LeftToolBarWdg;
class DoublePubStuChoiceUI : public CBaseWnd {
   Q_OBJECT   

public:
   explicit DoublePubStuChoiceUI(QWidget *parent = 0);
   ~DoublePubStuChoiceUI();
   virtual void Notice(const CSTDMapParam &mapParam);

   CREATE_WND();
   enum eChoiceType
   {
      eChoiceType_Deskop = 0,      //双向桌面共享
      eChoiceType_Camera,           //双推
      eChoiceType_Invalid
   };

   void initItem(const eChoiceType& type);
   int iRowCount();
   
public:
   bool Create();
   void SetTitle(const QString& strTitle);

   void Destroy();
   void Clear();
   void AddUser(const CDoubleVrtcUsers& vrtcUser);

   void CloseWdg();

signals:
   void sig_close();
   void sig_inviteDoubleVrtc(QString strId, const eChoiceType& eType);
public slots:
   void slot_ItemDoubleClicked(const QModelIndex &index);
   void DeleteItem(DoublePubStuChoiceItem *);
   void slot_inviteDoubleVrtc(QString strId);
   void on_btnClose_clicked(bool checked = false);
public Q_SLOTS:
   virtual int exec();
private:

	//bool InsertFileList(const QString &filename);
	
	Ui::DoublePubStuChoiceUI ui;
   eChoiceType mType;
   LeftToolBarWdg* mpLeftToolBarWdg = nullptr;
};


class DoublePubStuChoiceItem : public CWidget
{
	Q_OBJECT

public:

	explicit DoublePubStuChoiceItem(QWidget *parent = NULL);
	~DoublePubStuChoiceItem();
	void setName(const QString &);
   void setId(const QString &);
   void setNoDoubleVrtc(const int& iVrtc);
	void setWidth(int);

   enum eEnableVrtc
   {
      //不支持双路连麦 0:可以双推 1:不可以
      eEnableVrtc_enable,
      eEnableVrtc_DisEnable
   };

	void setChecked(bool);
	void FlushLabelColor(bool pressed);
protected:
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mousePressEvent(QMouseEvent *);
	virtual void leaveEvent(QEvent *);

signals:
	void closeClicked(DoublePubStuChoiceItem *);
   void sig_inviteDoubleVrtc(QString strId);
private slots:

	void slot_Choice();
private:

	Ui::DoublePubStuChoiceItem ui;
	QString m_strFilePath;
   bool m_bChecked = false;
   QString  mStrId;
	QString  mNickName;
};

#endif
