#include "stdafx.h"
#include "vocabularycollector.h"
#include <QTableWidgetItem>
#include <QNetworkRequest>
#include <QApplication>
#include "utils.h"
#include <fstream>
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QInputDialog>
#include <algorithm>
#include <sstream>

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

    connect(ui.pushButtonAdd, SIGNAL(clicked(bool)),
            this, SLOT(insertVocabViaButton()));

    connect(clipboard_, SIGNAL(dataChanged()),
            this, SLOT(insertVocabFromClipboard()));

    connect(&netWorkAccessManager_, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(netWorkManagerGotData(QNetworkReply *)));

    connect(ui.actionSave_As, SIGNAL(triggered(bool)),
            this, SLOT(createSaveFileDialog()));

    connect(ui.actionPrint_Now, SIGNAL(triggered(bool)),
            this, SLOT(createPrintDialog()));

    connect(ui.pushButtonDeleteEntry, SIGNAL(clicked(bool)),
            this, SLOT(createDeleteEntryDialog()));

    connect(this, SIGNAL(signalDeleteRow(int)),
            ui.tableWidget, SLOT(removeRow(int)));

    connect(ui.actionImport_from_file, SIGNAL(triggered(bool)),
            this, SLOT(importFromFileDialog()));
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
    data->deleteLater(); // schedule the object to be deleted upon returning to the event loop

    QString str{ data->readAll() };

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
    static auto constexpr fontSize = 26;
    QString text{ };
    QPrinter printer{ };
    printer.setPaperSize(QPrinter::A4);

    QPrintDialog printDialog{ &printer };
    if (printDialog.exec() == QDialog::Accepted) {
        QTextStream textStream{ &text };
        printTableTo(textStream);

        QFont font{ QString{ "Arial" }, fontSize };

        QPainter painter{ };
        painter.setFont(font);
        painter.begin(&printer);
        auto finalAct = utils::finally([&painter] {
            painter.end();
        });
        painter.drawText(QRectF{ QPointF{ 10.F, 10.F }, QPointF{ 5000.F, 5000.F } }, text);
    } else {
        utils::showMsgBox(QString{ "Invalid printer was selected." });
    }
}

void VocabularyCollector::createDeleteEntryDialog() {
    static auto constexpr title = "Delete entry";
    static auto constexpr text = "Which row should be removed?";    
    static auto constexpr step = 1;
    static auto constexpr offset = 1; // subtract this from the int returned to convert to 0 based indexing.
    static auto constexpr defaultMin = 1;
    auto const parent = this;

    auto const maxValue = ui.tableWidget->rowCount();
    auto const minValue = std::min(defaultMin, maxValue);
    auto const startValue = minValue;
    
    bool ok{ }; // will be true if user clicked OK; will be false otherwise
    auto rowToDelete = QInputDialog::getInt(parent, title, text,
                                            startValue, minValue, maxValue,
                                            step, &ok) - offset;

    if (ok) { // if the user hit OK; delete the row.
        emit signalDeleteRow(rowToDelete);
    }
}

void VocabularyCollector::importFromFileDialog() {
    auto fileName = QFileDialog::getOpenFileName(this, // parent
                                                "Open file to import", // caption
                                                QString{ }, // dir
                                                "Text document(*.txt)"); // filter

    if (!fileName.isNull()) {
        auto file = fileName.toStdString();
        importFromFile(file);
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
    static auto constexpr fieldWidth = 25;

    auto finalAct = utils::finally([&fstream] {
        fstream << flush;
        fstream.reset();        
    });

    fstream << qSetFieldWidth(fieldWidth) << left << "English"
            << right << "German" << "\n\n";
    
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
        fstream << left << e.first 
                << right << e.second << '\n';
    }
}

void VocabularyCollector::importFromFile(std::string const &file) {
    std::ifstream ifs{ file };
    if (!ifs) {
        throw std::runtime_error{ "could not open file in importFromFile" };
    }

    std::ostringstream ostr{ };
    ostr << ifs.rdbuf();

    auto content = ostr.str();

    auto strings = utils::split(content, std::string{ " " });
    strings.erase(std::begin(strings), std::begin(strings) + 2);
    strings.erase(std::remove(std::begin(strings), std::end(strings), "\n"), std::end(strings));
    for (auto &&e : strings) {
        utils::removeNewlinesFromString(e);
    }
    

    if (strings.size() % 2 != 0) {
        return;
    }

    auto it1 = std::begin(strings);
    auto end = std::end(strings);

    for (; it1 != end; ++++it1) {
        insertVocab(QString::fromStdString(*it1), QString::fromStdString(*(it1 + 1)));
    }
}

