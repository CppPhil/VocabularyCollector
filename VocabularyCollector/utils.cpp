#include "stdafx.h"
#include "utils.h"
#include <regex>
#include <QMessageBox>
#include <string>
#include <algorithm>

namespace utils {

    void replaceSubstring(std::string &modifyMe,
        std::string const &replaceMe,
        std::string const &replaceWithThis) {
        using StringType = std::string;
        if (modifyMe == replaceMe) {
            modifyMe = replaceWithThis;
            return;
        }
        auto index = static_cast<StringType::size_type>(0U);
        for (;;) {
            index = modifyMe.find(replaceMe, index);
            if (index == StringType::npos) {
                break;
            }
            modifyMe.replace(index, replaceMe.length(), replaceWithThis);
            index += replaceWithThis.length();
        }
    }

    std::vector<std::vector<std::string>> getRegexMatchesInString(std::string const &stringToSearchIn,
        std::string const &regexString) {
        using string_type = std::string;
        using vector = std::vector<string_type>;
        using container_type = std::vector<vector>;
        using sregex_iterator = std::sregex_iterator;

        container_type retVector{};
        std::smatch smatch{};
        std::regex regex{ regexString };
        auto itBeg = std::begin(regexString);
        auto itEnd = std::end(regexString);
        auto amountOfCharacterGroups = std::count(itBeg, itEnd, '(');
        auto temp = std::count(itBeg, itEnd, ')');
        amountOfCharacterGroups = std::min(amountOfCharacterGroups, temp);
        retVector.resize(amountOfCharacterGroups + 1U);

        for (auto it =
            sregex_iterator{
            std::begin(stringToSearchIn),
            std::end(stringToSearchIn),
            regex },
            iterEnd = sregex_iterator{};
            it != iterEnd;
            ++it) {
            smatch = *it;
            for (decltype(amountOfCharacterGroups) i{}; i <= amountOfCharacterGroups; ++i) {
                retVector[i].push_back(smatch[i].str());
            }
        }
        return retVector;
    }

    void showMsgBox(QString const &text) {
        QMessageBox messageBox{ };
        messageBox.setText(text);
        messageBox.exec();
    }

    QString httpErrCodeToQString(HttpErrorCode errCode) {
        switch (errCode) {
        case HttpErrorCode::blockedAPIKey:
            return "blocked API key";
        case HttpErrorCode::exceededDailyLimitAmount:
            return "Exceeded the daily limit on the amount of translated text";
        case HttpErrorCode::exceededDailyLimitRequests:
            return "Exceeded the daily limit on the number of requests";
        case HttpErrorCode::exceededMaximumTextLen:
            return "Exceeded the maximum text size";
        case HttpErrorCode::failedToTranslate:
            return "The text cannot be translated";
        case HttpErrorCode::invalidAPIKey:
            return "Invalid API key";
        case HttpErrorCode::translationNotSupported:
            return "The specified translation direction is not supported";
        default:
            return "";
        }
    }

    void showMsgBox(HttpErrorCode errCode) {
        QMessageBox messageBox{ };
        auto str = httpErrCodeToQString(errCode);
        if (!str.isEmpty()) {
            messageBox.setText(str);
            messageBox.exec();
        }
    }

    void removeNewlinesFromString(std::string &s) {
        s.erase(std::remove(std::begin(s), std::end(s), '\n'), std::end(s));
    }
}
