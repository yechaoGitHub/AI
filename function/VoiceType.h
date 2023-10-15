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
    int     voiceCode;
    QString name;
    int     language;
    QString languageName;
    int     gender;
    QString genderName;
};

struct PhoneRegionInfo
{
    QString name;
    QString dialingCode;
    QString abb;
};