#pragma once
#include <QPushButton>
#include <QTimer>
#include <QMovie>

class GifButton : public QPushButton
{
	Q_OBJECT

public:
	GifButton(QWidget *parent = Q_NULLPTR);
	~GifButton();
	void setText(const QString &text);
	void startMove();
	void setPaused();
	void SetImgPath(const QString &path, const int& iw, const int& ih);
	void setMoveStr(const QString &text);
protected:

private slots:
	void iconChged(int);
private:
	QString mstrImgPath;
	QMovie* mpMovie = nullptr;
	QString mMoveStr;
	int miGifW = -1;
	int miGifH = -1;
};
