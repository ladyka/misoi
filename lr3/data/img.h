#ifndef IMG_H
#define IMG_H

#include <array>
#include <cassert>

using ImgData = std::array<int8_t, 16*16>;

extern const ImgData N;
extern const ImgData S;
extern const ImgData P;

inline
int8_t toNet(int8_t e) {
    assert(e == 0 || e == 1);
    return e == 0 ? -1 : e;
}

inline
int8_t toBin(int8_t e) {
    assert(e == -1 || e == 1);
    return e == -1 ? 0 : e;
}

#endif // IMG_H
