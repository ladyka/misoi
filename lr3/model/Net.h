#ifndef NET_H
#define NET_H

#include <initializer_list>

#include "data/img.h"

class Net
{
public:
    Net(std::initializer_list<ImgData>);

    ImgData filter(const ImgData& img);
private:
    std::array<std::array<double, 16*16>, 16*16> _W;
};

#endif // NET_H
