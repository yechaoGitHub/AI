#pragma once
#include <QString>

enum TranslationType { MID, FIN };

enum class FunctionType { Translation, VoiceCompositor, ChatBot };

enum class LanguageType { EN, CHS };

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

struct PhoneRegionInfo
{
    QString name;
    QString dialingCode;
    QString abb;
};