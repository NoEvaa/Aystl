#pragma once

namespace iin
{
template <typename T>
inline T ayMax(T a, T b)
{
    return a < b ? b : a;
}
}
