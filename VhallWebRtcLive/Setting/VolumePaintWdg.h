#pragma once

#include <QWidget>
#include "ui_VolumePaintWdg.h"
#include "../Unility/Unility.h"
#include <QTimer>
class QColor;

enum VolumeType {
    VolumeType_PreviewMic = 0,
    VolumeType_Play_File = 1,
};

class VolumePaintWdg : public QWidget
{
    Q_OBJECT

public:
    VolumePaintWdg(QWidget *parent = Q_NULLPTR);
    ~VolumePaintWdg();

    void SetVolumeType(VolumeType type);
	void SetColor(QColor color);
protected:
    virtual void paintEvent(QPaintEvent*);
signals:
	void sig_micHasAudio();
private slots:
    void slot_PaintVolume();

private:
    Ui::VolumePaintWdg ui;
    int mVolumeType = 0;
    QTimer mPaintTimer;
	QColor mcolor;
};
