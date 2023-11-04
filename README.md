AiSound项目模块简介

soundAl.nsi ：nsis打包脚本

功能模块：
function:
    QHook:全局快捷键
    QAudioPlayer：在线MP3播放

    bussiness:
        接口业务处理类，及一些代理model

base:
    GlobalSetting:本地化记录类
    HttpClicen:http请求封装类

UI模块：
    1.悬浮窗
        WRobotNavigation
        WRobotSettingMenu
    2.聊天机器人：chat
        WRobotChat.h
        WChatItem.h
    3.翻译模块：WTranslationMain
        WTranslationMain.h
        WTranslationTimer.h
    4.语音合成模块：Speech
        WSpeechGenerationUi.h
        WSpeechTimer.h
    5.设置界面：Setting
        1.主界面:WSettingMainUi
        2.chatbot模块：ChatBot
            WChatBotMainUI
            WHistoryPage
        3.SoundBot模块：SoundBot
            WSoundBotMainUi
