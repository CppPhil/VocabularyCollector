#pragma once
#include <QtWidgets/QMainWindow>
#include <QString>
#include <QClipBoard>
#include <QNetWorkReply>
#include <QNetworkAccessManager>
#include "ui_vocabularycollector.h"

#ifdef TRANSLATE_TEXT_DUMMY
    static_assert("TRANSLATE_TEXT_DUMMY was already defined in vocabularycollector.h");
#endif // TRANSLATE_TEXT_DUMMY

#ifdef KEY_DUMMY
    static_assert("KEY_DUMMY was already defined in vocabularycollector.h");
#endif // KEY_DUMMY

#define TRANSLATE_TEXT_DUMMY    "{PUTTEXTHERE}"
#define KEY_DUMMY   "{PUTKEYHERE}"

class VocabularyCollector final : public QMainWindow {
    Q_OBJECT

public:

    enum class Language {
        English = 0,
        German = 1,
    };

    void setTableItem(int row, Language lang, QString const &text) const;
    explicit VocabularyCollector(QWidget *parent = nullptr);
    void insertVocab(QString englishText, QString germanText = QString{ });
    
public slots:
    void insertVocabViaButton();
    void insertVocabFromClipboard();
    void netWorkManagerGotData(QNetworkReply *data);
    void createSaveFileDialog() const;
    void createPrintDialog() const noexcept;
    void createDeleteEntryDialog();

signals:
    void signalDeleteRow(int);

private:
    void exportToFile(QString const &fileName) const;

    void printTableTo(QTextStream &fstream) const;

    static QString const notARealText;

    Ui::VocabularyCollectorClass ui;
    QClipboard *clipboard_;
    QNetworkAccessManager netWorkAccessManager_;
    std::string baseUrl_ = 
        "https://translate.yandex.net/api/v1.5/tr/translate?"
        "key="
        KEY_DUMMY
        "&text="
        TRANSLATE_TEXT_DUMMY
        "&lang=de";
    QString currentTranslation_ = notARealText;
    QString currentEnglishText_ = notARealText;
};
