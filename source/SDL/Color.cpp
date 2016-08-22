#include "Color.h"
#include <stdio.h>


Color Color::getColor(SDL_PixelFormat *fmt, Uint32 pixel)
{
    return Color((Uint8)(((pixel & fmt->Rmask) >> fmt->Rshift) << fmt->Rloss),
                 (Uint8)(((pixel & fmt->Gmask) >> fmt->Gshift) << fmt->Gloss),
                 (Uint8)(((pixel & fmt->Bmask) >> fmt->Bshift) << fmt->Bloss),
                 (Uint8)(((pixel & fmt->Amask) >> fmt->Ashift) << fmt->Aloss),
                 fmt
                 );

    #if 0

    Color rt;

    rt.setFormat(fmt);

    /* Get Red component */
    rt.r = (Uint8)(((pixel & fmt->Rmask) >> fmt->Rshift) << fmt->Rloss);

    /* Get Green component */
    rt.g = (Uint8)(((pixel & fmt->Gmask) >> fmt->Gshift) << fmt->Gloss);

    /* Get Blue component */
    rt.b = (Uint8)(((pixel & fmt->Bmask) >> fmt->Bshift) << fmt->Bloss);

    /* Get Alpha component */
    rt.a = (Uint8)(((pixel & fmt->Amask) >> fmt->Ashift) << fmt->Aloss);

    return rt;

    Uint32 temp;

    /* Get Red component */
    temp = pixel & fmt->Rmask;  /* Isolate red component */
    temp = temp >> fmt->Rshift; /* Shift it down to 8-bit */
    temp = temp << fmt->Rloss;  /* Expand to a full 8-bit number */
    rt.r = (Uint8)temp;

    /* Get Green component */
    temp = pixel & fmt->Gmask;  /* Isolate green component */
    temp = temp >> fmt->Gshift; /* Shift it down to 8-bit */
    temp = temp << fmt->Gloss;  /* Expand to a full 8-bit number */
    rt.g = (Uint8)temp;

    /* Get Blue component */
    temp = pixel & fmt->Bmask;  /* Isolate blue component */
    temp = temp >> fmt->Bshift; /* Shift it down to 8-bit */
    temp = temp << fmt->Bloss;  /* Expand to a full 8-bit number */
    rt.b = (Uint8)temp;

    /* Get Alpha component */
    temp = pixel & fmt->Amask;  /* Isolate alpha component */
    temp = temp >> fmt->Ashift; /* Shift it down to 8-bit */
    temp = temp << fmt->Aloss;  /* Expand to a full 8-bit number */
    rt.a = (Uint8)temp;

    return rt;

    #endif
}


Uint32 createUint32Color(SDL_PixelFormat* fmt, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return (r << fmt->Rshift) | (g << fmt->Gshift) | (b << fmt->Bshift) | (a << fmt->Ashift);

/// TODO retirar essa parte
    Uint32 pixel = 0;
    /* Get Red component */
    pixel |= r << fmt->Rshift;
    //printf("R:%X => %X (%d)\n", temp, pixel, fmt->Rshift);

    /* Get Green component */
    pixel |= g << fmt->Gshift;
    //printf("G:%X => %X (%d)\n", temp, pixel, fmt->Gshift);

    /* Get Blue component */
    pixel |= b << fmt->Bshift;
    //printf("B:%X => %X (%d)\n", temp, pixel, fmt->Bshift);

    /* Get Alpha component */
    pixel |= a << fmt->Ashift;

    return pixel;
}


RGB fromHSV(int h, double s, double v)
{
// source: http://www.rapidtables.com/convert/color/hsv-to-rgb.htm
    double r = 0.0, g = 0.0, b = 0.0;
    v *= 100.0;
    double C = v * s;
    //X = C × (1 - |(H / 60º) mod 2 - 1|)
    double X = double(h) / 60.0;
    X = fmod(X, 2.0);
    X = X - 1.0;
    X = ABS(X);
    X = 1 - X;
    X = C * X;
    double m = (v - C);

    switch (h)
    {
        case 0 ... 59:
            r = C;
            g = X;
            b = 0;
            break;
        case 60 ... 119:
            r = X;
            g = C;
            b = 0;
            break;
        case 120 ... 179:
            r = 0;
            g = C;
            b = X;
            break;
        case 180 ... 239:
            r = 0;
            g = X;
            b = C;
            break;
        case 240 ... 299:
            r = X;
            g = 0;
            b = C;
            break;
        case 300 ... 359:
            r = C;
            g = 0;
            b = X;

            break;
    }

#define result(x) int((x + m)*0xff/0x64)
    char error[1024];
    sprintf(error, "(%d, %.3f, %.3f) = (%03d, %03d, %03d)",
                h, s, v, result(r), result(g), result(b));
    if (result(r) > 255) printf("%s = (r: %u)\n", error, Uint8(result(r))); //Exception::throwIt(error);
    if (result(g) > 255) printf("%s = (g: %u)\n", error, Uint8(result(g))); //Exception::throwIt(error);
    if (result(b) > 255) printf("%s = (b: %u)\n", error, Uint8(result(b))); //Exception::throwIt(error);

    RGB rgb;
//    rgb.r = int((r + m)*255);
    rgb.r = result(r);
//    rgb.g = int((g + m)*255);
    rgb.g = result(g);
//    rgb.b = int((b + m)*255);
    rgb.b = result(b);


    return rgb;
    #undef result
};
HSV fromRGB(int r, int g, int b)
{
//source: http://www.rapidtables.com/convert/color/rgb-to-hsv.htm

    HSV hsv;
    double R = double(r)/255;
    double G = double(g)/255;
    double B = double(b)/255;
    //printf("%f : %f : %f\n", R, G, B);

    double Cmax = MAX3(R, G, B);
    double Cmin = MIN3(R, G, B);
    double delta = Cmax - Cmin;
    //printf("%f : %f : %f\n", Cmax, Cmin, delta);

    if (delta == 0)
    {
        hsv.hue = 0;
    }
    else if (Cmax == R)
    {
        hsv.hue = int (60 * (fmod(((G - B)/delta), 6)));
    }
    else if (Cmax == G)
    {
        hsv.hue = int(60 * (((B - R) / delta) + 2));
    }
    else if (Cmax == B)
    {
        hsv.hue = int(60 * (((R - G) / delta) + 4));
    }

        // Devido a um erro de calculo proveniente desse site
        // http://www.rapidtables.com/convert/color/rgb-to-hsv.htm
        // Tive que adicionar essa gambiarra
        if (hsv.hue < 0)
            hsv.hue = 359 - (hsv.hue*-1);

    if (Cmax == 0)
    {
        hsv.sat = 0;
    }
    else
    {
        hsv.sat = (delta/Cmax);
    }

    hsv.val = Cmax;

    return hsv;
}

HSV fromRGB(RGB rgb) {return fromRGB(rgb.r, rgb.g, rgb.b);}
RGB fromHSV(HSV hsv) {return fromHSV(hsv.hue, hsv.sat, hsv.val);}

const Color Color::WHITE(0xff, 0xff, 0xff);
const Color Color::W_TRANSPARENT(0xff, 0xff, 0xff, 0x00);
const Color Color::BLACK(0x0, 0x0, 0x0);

bool Color::operator==(Color color)
{
    return (color.r == r && color.g == g && color.b == b && color.a == a);
}
