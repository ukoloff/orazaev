/**
    @brief RAII ideome for ofstream.

    @author Aman Orazaev
*/
#include <fstream>
#include <sstream>


class TOfstreamGuard {
public:
    TOfstreamGuard(const std::string filename)
        : ofstream_(filename)
    { }

    template <typename T>
    std::ostream& operator<< (const T& something) {
        return ofstream_ << something;
    }

    ~TOfstreamGuard() {
        if (ofstream_.is_open()) {
            ofstream_.close();
        }
    }

private:
    TOfstreamGuard(const TOfstreamGuard&) = delete;
    TOfstreamGuard& operator=(const TOfstreamGuard&) = delete;
private:
    std::ofstream ofstream_;
};
