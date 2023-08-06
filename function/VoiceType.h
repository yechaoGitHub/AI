#pragma once
#include <QString>

enum TranslationType { MID, FIN };

enum class FunctionType { Translation, VoiceCompositor, ChatBot };

struct TranslationLanguage
{
    QString name;
    QString nameEn;
    QString language;
};

struct VoiceData
{
    int     id;
    QString voiceCode;
    QString name;
    int     language;
    int     gender;
};
