#include "data_types.h"

std::ostream& operator << (std::ostream& os,  coordinate &c)
{
    os << "(" << c.x << ", "  << c.y << ")";
    return os;
}
