#include "stdafx.h"
#include <string>
#include <vector>
#include <QString>

namespace utils {
    enum class HttpErrorCode {
        invalidAPIKey = 401,
        blockedAPIKey = 402,
        exceededDailyLimitRequests = 403,
        exceededDailyLimitAmount = 404,
        exceededMaximumTextLen = 413,
        failedToTranslate = 422,
        translationNotSupported = 501,
        httpOK = 200,
    };

    void replaceSubstring(std::string &modifyMe,
                          std::string const &replaceMe,
                          std::string const &replaceWithThis);

    std::vector<std::vector<std::string>> getRegexMatchesInString(std::string const &stringToSearchIn,
                                                                  std::string const &regexString);
    void showMsgBox(QString const &text);
    void showMsgBox(HttpErrorCode errCode);

    namespace detail {
        template <class Callable>
        struct FinalAction final {
            explicit FinalAction(Callable f) : clean{ std::move(f) } { }

            ~FinalAction() {
                clean();
            }

        private:
            Callable clean;
        }; 
    } 

    template <class Callable>
    detail::FinalAction<Callable> finally(Callable &&f) {
        return detail::FinalAction<Callable>(std::forward<Callable>(f));
    }
}
