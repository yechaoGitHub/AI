#include "QAudioPlayer.h"

QAudioPlayer::QAudioPlayer(QObject *parent)
	: QObject(parent)
{
	_player = new QMediaPlayer();
}

QAudioPlayer::~QAudioPlayer()
{
	if (_player) {
		delete _player;
		_player = nullptr;
	}
}

void QAudioPlayer::playUrl(const QString& url)
{
	if (_player->state() == QMediaPlayer::State::PlayingState) {
		_player->stop();
	}

	// ²âÊÔµØÖ·
	//http://music.163.com/song/media/outer/url?id=447925558.mp3
	_player->setMedia(QUrl(url));
	_player->play();
}