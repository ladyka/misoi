#include "Net.h"

#include <numeric>
#include <vector>
#include <algorithm>

Net::Net(std::initializer_list<ImgData> list) {
    for (size_t i = 0; i < _W.size(); ++i) {
        for (size_t j = 0; j <= i; ++j) {

            if (i == j) {
                _W[i][j] = 0;
                continue;
            }

            auto w = double{std::accumulate(begin(list), end(list), 0.0,
                                     [&](double oldVal, const ImgData& img)-> double {
                                         return oldVal + toNet(img[i]) * toNet(img[j]);})
                     } / list.size();
            _W[i][j] = w;
            _W[j][i] = w;
        }
    }
}

inline
static int T(double d) {
    return d < 0 ? -1 : 1;
}

ImgData Net::filter(const ImgData &img) {
    auto next = 0u;
    auto Y = std::vector<ImgData::value_type>{};
    Y.reserve(img.size());
    std::transform(begin(img), end(img), std::back_inserter(Y), &toNet);
    auto changed = false;

    while(true) {
        auto yD = 0.0;
        for (auto j = 0u; j < Y.size(); ++j) {
            yD += _W[j][next] * Y[j];
        }
        auto y = T(yD);

        if (y != Y[next]) {
            changed = true;
        }

        if (next + 1 == Y.size() && !changed) {
            break;
        }

        Y[next] = y;
        if (next + 1 == Y.size()) {
            next = 0;
            changed = false;
        } else {
            ++next;
        }
    }

    auto res = ImgData{{0}};
    std::transform(begin(Y), end(Y), begin(res), &toBin);
    return res;
}

