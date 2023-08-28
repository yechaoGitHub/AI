#pragma once

#include <QObject>
#include <qmediaplayer.h>


class QAudioPlayer  : public QObject
{
	Q_OBJECT

public:
	QAudioPlayer(QObject *parent);
	~QAudioPlayer();

	void playUrl(const QString& url);

private:
	QMediaPlayer* _player = nullptr;
};
