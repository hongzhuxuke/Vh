#include "GifButton.h"
#include <QTextDocument>
#include <QPainter> 

GifButton::GifButton(QWidget *parent /*= Q_NULLPTR*/)
	:QPushButton(parent)
{

}

GifButton::~GifButton()
{

}

void GifButton::SetImgPath(const QString &path, const int& iw, const int& ih)
{
	mstrImgPath = path;
	miGifW = iw;
	miGifH = ih;
}

void GifButton::setText(const QString &text)
{
	if(nullptr!= mpMovie && QMovie::Running == mpMovie->state())
		mpMovie->stop();

	setIcon(QIcon(""));
	setIconSize(QSize(0, 0));
	update();
	QPushButton::setText(text);
	
}

void GifButton::setMoveStr(const QString &text)
{
	mMoveStr = text;
}

void GifButton::startMove()
{
	setText("");
	//this->setFixedSize(36, 20);
	if (nullptr == mpMovie && !mstrImgPath.isEmpty())
	{
		mpMovie = new QMovie(mstrImgPath, QByteArray(), this);
		connect(mpMovie, SIGNAL(frameChanged(int)), this, SLOT(iconChged(int)));
	}

	if (nullptr != mpMovie && mpMovie->isValid())
	{
		if (QMovie::NotRunning == mpMovie->state())
		{
			mpMovie->start();
		}
		else if(QMovie::Paused == mpMovie->state())
		{
			mpMovie->setPaused(false);
		}
	}
	else
		setText(mMoveStr);
}

void GifButton::setPaused()
{
	if (nullptr != mpMovie /*&& mpMovie->isValid()*/)
		mpMovie->setPaused(true);
}

void GifButton::iconChged(int)
{
	QPixmap currFrame = mpMovie->currentPixmap();

	int iCurW = width();
	int iCurH = height();

	QPixmap pixmap(iCurW, iCurH);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	
	if (miGifW == -1 && miGifH == -1)
	{
		miGifW = iCurW;
		miGifH = iCurH;
	}

	painter.drawPixmap(0.5*(iCurW-miGifW), 0.5*(iCurH- miGifH), miGifW, miGifH, currFrame);
	setIcon(QIcon(pixmap));
	setIconSize(pixmap.rect().size());
}