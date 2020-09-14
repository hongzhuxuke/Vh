#include "VolumePaintWdg.h"
#include <QPainter>
extern ToolManager *globalToolManager;
VolumePaintWdg::VolumePaintWdg(QWidget *parent)
    : QWidget(parent),
	mcolor(QColor(255, 192, 89))
{
    ui.setupUi(this);
    connect(&mPaintTimer, SIGNAL(timeout()), this, SLOT(slot_PaintVolume()));
}

VolumePaintWdg::~VolumePaintWdg()
{
    mPaintTimer.stop();
}

void VolumePaintWdg::SetVolumeType(VolumeType type) {
    mVolumeType = type;
    mPaintTimer.start(100);
}

void VolumePaintWdg::SetColor(QColor color) {
	mcolor = color;
}

void VolumePaintWdg::paintEvent(QPaintEvent*) {
    QPainter paint;
    paint.begin(this);
    paint.setPen(QPen(mcolor, 1, Qt::SolidLine));//设置画笔形式 
    paint.setBrush(QBrush(mcolor, Qt::SolidPattern));//设置画刷形式 
    int value = 0;
    if (mVolumeType == VolumeType_PreviewMic) {
        value = globalToolManager->GetClassSDK()->GetMicVolumValue();
    }
    else if (mVolumeType == VolumeType_Play_File) {
        value = globalToolManager->GetClassSDK()->GetPlayAudioFileVolum();
    }
	if (value > 0) {
		emit sig_micHasAudio();
	}

    int paintWidth = (float)value / 100.0 * this->width();
    int i = 0;
    for (; i < paintWidth / 10; i++) {
        if (i % 2 == 0) {
            paint.drawRect(QRect(i * 10, 0, 10, this->height()));
        }
    }

    QColor rightColor(237, 237, 237);
    paint.setPen(QPen(rightColor, 1, Qt::SolidLine));//设置画笔形式 
    paint.setBrush(QBrush(rightColor, Qt::SolidPattern));//设置画刷形式 
    for (; i <= this->width() / 10; i++) {
        if (i % 2 == 0) {
            paint.drawRect(QRect(i * 10, 0, 10, this->height()));
        }
    }
    paint.end();
}

void VolumePaintWdg::slot_PaintVolume() {
    repaint();
}
