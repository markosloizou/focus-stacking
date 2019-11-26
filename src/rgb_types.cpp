#include "rgb_types.h"


rgb_16::rgb_16(ushort r, ushort g, ushort b)
{
    R = r;
    G = g;
    B = b; 
}

rgb_16::rgb_16()
{
    R = 0;
    G = 0;
    B = 0;
}

rgb_16  rgb_16::operator  *= (const float &rhs)
{    
    R *= rhs;
    G *= rhs;
    B *= rhs;
    return *this;
}

rgb_16 rgb_16::operator += (const rgb_16 &rhs)
{
    R += rhs.R;
    G += rhs.G;
    B += rhs.B;
    
    return *this;
}

 rgb_16 operator + (rgb_16 lhs, const rgb_16 &rhs)
{
    lhs += rhs;
    return lhs;
}

 rgb_16 operator * ( const float lhs, const rgb_16 &rhs)
{
    rgb_16 c;
    c.R = rhs.R * lhs;
    c.G = rhs.G * lhs;
    c.B = rhs.B * lhs;
    return c;
}


 rgb_16 operator * (rgb_16 lhs, const float &rhs)
{
    lhs *= rhs;
    return lhs;
}

std::ostream& operator << (std::ostream& os, const rgb_16 &rgb)
{
    os << "(" << rgb.R << ", " << rgb.G << ", " << rgb.B << ")";
    return os;
}



rgba_16::rgba_16(ushort r, ushort g, ushort b, ushort a)
{
    R = r;
    G = g;
    B = b; 
    A = a;
}

rgba_16::rgba_16()
{
    R = 0;
    G = 0;
    B = 0;
    A = 0;
}

rgba_16  rgba_16::operator  *= (const float &rhs)
{    
    R *= rhs;
    G *= rhs;
    B *= rhs;
    A *= rhs;
    
    return *this;
}

rgba_16 rgba_16::operator += (const rgba_16 &rhs)
{
    R += rhs.R;
    G += rhs.G;
    B += rhs.B;
    A += rhs.A;
    
    return *this;
}

rgba_16 operator + (rgba_16 lhs, const rgba_16 &rhs)
{
    lhs += rhs;
    return lhs;
}

rgba_16 operator * ( const float lhs, const rgba_16 &rhs)
{
    rgba_16 c;
    c.R = rhs.R * lhs;
    c.G = rhs.G * lhs;
    c.B = rhs.B * lhs;
    c.A = rhs.A * lhs;
    
    return c;
}


rgba_16 operator * (rgba_16 lhs, const float &rhs)
{
    lhs *= rhs;
    return lhs;
}

std::ostream& operator << (std::ostream& os, const rgba_16 &rgba)
{
    os << "(" << rgba.R << ", " << rgba.G << ", " << rgba.B << ", " << rgba.A <<")";
    return os;
}
