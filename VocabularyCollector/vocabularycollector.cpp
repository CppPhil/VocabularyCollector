#include "stdafx.h"
#include "vocabularycollector.h"
#include <QTableWidgetItem>
#include <QNetworkRequest>
#include <QApplication>
#include "utils.h"
#include <fstream>
#include <QFileDialog>
#include <memory>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>

QString const VocabularyCollector::notARealText = "este no es un texto real.";

void VocabularyCollector::setTableItem(int row, Language lang, QString const &text) const {
    ui.tableWidget->setItem(row, 
                            static_cast<int>(lang), // column
                            new QTableWidgetItem{ text }); // item (the table will take ownership of it)
}

VocabularyCollector::VocabularyCollector(QWidget *parent)
    : QMainWindow{ parent }, 
      clipboard_{ nullptr },
      netWorkAccessManager_{ this } {
   
    ui.setupUi(this);

    std::ifstream ifstream{ "Key.txt" };
    std::string key{ };
    std::getline(ifstream, key);
    utils::replaceSubstring(baseUrl_, std::string{ KEY_DUMMY }, key);

    clipboard_ = QApplication::clipboard();

    connect(ui.pushButton, SIGNAL(clicked(bool)),
            this, SLOT(insertVocabViaButton()));

    connect(clipboard_, SIGNAL(dataChanged()),
            this, SLOT(insertVocabFromClipboard()));

    connect(&netWorkAccessManager_, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(netWorkManagerGotData(QNetworkReply *)));

    connect(ui.actionSave_As, SIGNAL(triggered(bool)),
            this, SLOT(createSaveFileDialog()));

    connect(ui.actionPrint_Now, SIGNAL(triggered(bool)),
            this, SLOT(createPrintDialog()));
}

void VocabularyCollector::insertVocab(QString englishText, QString germanText) {
    QByteArray dummy{ };
    auto url = baseUrl_;
    auto stdStrEnglTxt = englishText.toStdString();
    utils::replaceSubstring(url, std::string{ TRANSLATE_TEXT_DUMMY }, stdStrEnglTxt);
    QString qUrlString{ url.data() };
    QUrl qUrl{ qUrlString };
    QNetworkRequest networkRequest{ qUrl };
    networkRequest.setRawHeader(QByteArray{ "content-type" },
                                QByteArray{ "application/x-www-form-urlencoded" });

    auto cpy = englishText;
    cpy.remove(QRegExp{ "\\s+$" });

    if (cpy.isEmpty()) {
        return;
    }

    if (germanText.isEmpty()) {
        // look up translation
        currentEnglishText_ = englishText;
        netWorkAccessManager_.post(networkRequest, dummy);
        
        return; // abort; callback will recall
    }

    auto const table = ui.tableWidget;

    // create new row
    auto const newRowToAdd = table->rowCount();
    table->insertRow(newRowToAdd);

    auto addItem = [&row = newRowToAdd, this] (Language lang, QString const &txt) {
        setTableItem(row, lang, txt);
    };

    // add the english text
    addItem(Language::English, englishText);

    // add the german text
    addItem(Language::German, germanText);
}

void VocabularyCollector::insertVocabViaButton() {
    insertVocab(ui.lineEditEnglish->text(), ui.lineEditGerman->text());
}

void VocabularyCollector::insertVocabFromClipboard() {
    insertVocab(clipboard_->text());
}

/* format:
<?xml version="1.0" encoding="utf-8"?>
<Translation code="200" lang="en-de"><text>Katze</text></Translation>
*/

void VocabularyCollector::netWorkManagerGotData(QNetworkReply *data) {
    QString str{ data->readAll() };
    QMessageBox msgBox{ };

    auto stdString = str.toStdString();
    auto matches = utils::getRegexMatchesInString(stdString,
                                                  R"~(<Translation code="(.*?)"|<text>(.*?)</text>)~");
    try {
        auto tranlationCode = std::stoi(matches.at(1).at(0));

        utils::showMsgBox(static_cast<utils::HttpErrorCode>(tranlationCode));

        auto translation = matches.at(2).at(1);

        currentTranslation_ = QString::fromStdString(translation);
    } catch (std::out_of_range const &) {
        currentTranslation_ = notARealText;
        return;
    }

    data->deleteLater(); // schedule the object to be deleted upon returning to the event loop

    insertVocab(currentEnglishText_, currentTranslation_);
}

void VocabularyCollector::createSaveFileDialog() const {
    auto fileName = QFileDialog::getSaveFileName();
    if (fileName.isEmpty()) {
        return;
    }
    exportToFile(fileName);    
}

void VocabularyCollector::createPrintDialog() const noexcept {
    QString text{ };
    QPrinter printer{ };
    QPrintDialog printDialog{ &printer };
    if (printDialog.exec() == QDialog::Accepted) {
        QTextStream textStream{ &text };
        printTableTo(textStream);

        QPainter painter{ };
        painter.begin(&printer);
        painter.drawText(QRectF{ QPointF{ 10.F, 10.F }, QPointF{ 5000.F, 5000.F } }, text);
        painter.end();
    } else {
        utils::showMsgBox(QString{ "Invalid printer was selected." });
    }

}

void VocabularyCollector::exportToFile(QString const &fileName) const {
    auto file = std::fopen(fileName.toUtf8().data(), "w");
    auto finalAct = utils::finally([file] { 
                                              if (file != nullptr) {
                                                  std::fclose(file);
                                              }
                                   });

    QTextStream fstream{ file, QIODevice::WriteOnly };

    if (fstream.status() != QTextStream::Status::Ok || file == nullptr) {
        utils::showMsgBox(QString{ "file could not be exported" });
        return;
    }
    
    printTableTo(fstream);   
}

void VocabularyCollector::printTableTo(QTextStream &fstream) const {
    fstream << "English\tGerman\n";

    std::unique_ptr<QTableWidgetItem> tempTableWidgetItem = nullptr;
    QString tempQString{ };

    auto list = ui.tableWidget->findItems(QString{ ".*" }, Qt::MatchRegExp);
    std::vector<std::pair<QString, QString>> vec{ };
    
    auto const rows = ui.tableWidget->rowCount();
    auto curRow = 0;
    
    auto it = std::begin(list);
    auto end = std::end(list);

    for (; it != end && curRow < rows; ++it, ++curRow) {
        vec.emplace_back((*it)->text(), "");
    }

    curRow = 0;
    for (; it != end; ++it, ++curRow) {
        vec.at(curRow).second = (*it)->text();
    }

    for (auto &&e : vec) {
        fstream << e.first << '\t' << e.second << '\n';
    }
}
