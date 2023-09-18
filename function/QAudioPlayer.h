#pragma once

#include <QObject>
#include <qmediaplayer.h>


class QAudioPlayer  : public QObject
{
	Q_OBJECT
signals:
	void	sig_playStop();
public:
	QAudioPlayer(QObject *parent);
	~QAudioPlayer();

	static QAudioPlayer* GetInstance();

	void playUrl(const QString& url);


private:
	QMediaPlayer* _player = nullptr;
	static QAudioPlayer* _instance;
	bool	_user_stop = false;
};
