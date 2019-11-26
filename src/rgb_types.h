#ifndef rgb_types_h
#define rgb_types_h

#include <iostream>


class rgb_16
{
    public:
        rgb_16(ushort r, ushort g, ushort b);
        rgb_16();
        
        ushort R;
        ushort G;
        ushort B;
        
        rgb_16 operator += (const rgb_16 &rhs);
        rgb_16  operator *= (const float &rhs);
        
        friend std::ostream& operator << (std::ostream& os, const rgb_16 &rgb);
};

 rgb_16 operator + (rgb_16 lhs, const rgb_16 &rhs);
 rgb_16 operator * (const float lhs, const rgb_16 &rhs);
 rgb_16 operator * ( rgb_16 lhs, const float &rhs); 

std::ostream& operator << (std::ostream& os, const rgb_16 &rgb);


class rgba_16
{
public:
    rgba_16(ushort r, ushort g, ushort b, ushort a);
    rgba_16();
    
    ushort R;
    ushort G;
    ushort B;
    ushort A;
    
    rgba_16 operator += (const rgba_16 &rhs);
    rgba_16  operator *= (const float &rhs);
    
    friend std::ostream& operator << (std::ostream& os, const rgba_16 &rgba);
};

rgba_16 operator + (rgba_16 lhs, const rgba_16 &rhs);
rgba_16 operator * (const float lhs, const rgba_16 &rhs);
rgba_16 operator * ( rgba_16 lhs, const float &rhs); 

std::ostream& operator << (std::ostream& os, const rgba_16 &rgba);


#endif
