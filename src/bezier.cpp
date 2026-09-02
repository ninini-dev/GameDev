#include <own/bezier.h>
#include <iostream>
using namespace std;
using namespace glm;
vec2 Bezier::Calculate(float t, EaseType ease) {
    t = Bezier::ease(ease, t);
    float c = 1 - t;
    return p0 * c * c * c + p1 * c * c * t * 3.f + p2 * c * t * t * 3.f + p3 * t * t * t;
}
void Bezier::Displace(vec2 offset) {
    p0 += offset;
    p1 += offset;
    p2 += offset;
    p3 += offset;
}
MovePath::MovePath() {
    this->bezier = { {-.1,-.1},{-.1,-.1},{.1,.1},{.1,.1} };
    this->duration = 1;
    this->ease = Lineal;
}