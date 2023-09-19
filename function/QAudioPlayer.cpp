#include "QAudioPlayer.h"


QAudioPlayer* QAudioPlayer::_instance = nullptr;
QAudioPlayer::QAudioPlayer(QObject *parent)
	: QObject(parent)
{
	_player = new QMediaPlayer();
	connect(_player, &QMediaPlayer::stateChanged, this, [=](QMediaPlayer::State state) {
		if (state == QMediaPlayer::StoppedState && !_user_stop) {
			emit sig_playStop();
		}
		if (_user_stop) {
			_user_stop = false;
		}
		});
}

QAudioPlayer::~QAudioPlayer()
{
	if (_player) {
		delete _player;
		_player = nullptr;
	}
}

QAudioPlayer* QAudioPlayer::GetInstance()
{
	if (!_instance) {
		_instance = new QAudioPlayer(nullptr);
	}
	return _instance;
}

void QAudioPlayer::playUrl(const QString& url)
{
	if (_player->state() == QMediaPlayer::State::PlayingState) {
		_user_stop = true;
		_player->stop();
	}
	else {
		_user_stop = false;
	}

	// ²âÊÔµØÖ·
	//http://music.163.com/song/media/outer/url?id=447925558.mp3
	_player->setMedia(QUrl(url));
	_player->play();
}