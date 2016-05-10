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

    template <class CharType>
    auto split(std::basic_string<CharType> const &str,
               std::basic_string<CharType> const &delims)
    -> std::vector<std::basic_string<CharType>> {
        using StringType = std::basic_string<CharType>;
        using container_type = std::vector<StringType>;
        container_type result{ };
        result.reserve(7U);
        auto lambda{
            [&](auto c) {
                return std::any_of(std::begin(delims), std::end(delims), [c](auto ch) {
                                       return ch == c;
                                   });
            }
        };

        auto const begin{ std::begin(str) };
        auto const end{ std::end(str) };
        auto the_next_one{ begin };

        for (auto pos{ begin }; pos != end; pos = the_next_one + 1) {
            the_next_one = std::find_if(pos, end, lambda);
            result.emplace_back(pos, the_next_one);
            if (the_next_one == end) break;

        }

        result.erase(std::remove_if(std::begin(result), std::end(result), [](auto &&e) {
                                        return e.empty();
                                    }), std::end(result));

        result.shrink_to_fit();
        return result;
    } 

    void removeNewlinesFromString(std::string &);
}
