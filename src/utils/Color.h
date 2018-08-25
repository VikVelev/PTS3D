#ifndef COLOR_H
#define COLOR_H

#ifndef NULL
#define NULL (void *)0
#endif

#ifndef PI
#define PI 3.1415926f
#endif

struct Color {
    float r, g, b;

    Color();
    Color(const Color& c);
    Color(float r, float g, float b);
    Color(float color);

	virtual ~Color();
};


#endif // COLOR_H