#pragma once
#include <glm/vec2.hpp>
using namespace glm;

typedef enum EaseType {
    Lineal,
    EaseInSine, EaseOutSine, EaseInOutSine,
    EaseTypeCount
};

struct Bezier {
public:
    vec2 p0, p1, p2, p3;
    vec2 Calculate(float t,EaseType ease=Lineal);
    void Displace(vec2 offset);
    static float ease(EaseType easeType, float t) {
        switch (easeType)
        {
        case EaseInSine:
            return easeInSine(t);
            break; 
        case EaseOutSine:
                return easeOutSine(t);
                break;
        default:
            return t;
            break;
        }
    }
    static float easeInSine(float t) { return Bezier{ vec2(0, 0), vec2(0.12, 0), vec2(0.39, 0), vec2(1, 1)}.Calculate(t).y; }
    static float easeOutSine(float t) { return Bezier{ vec2(0, 0), vec2(0.61, 1), vec2(0.88, 1), vec2(1, 1)}.Calculate(t).y; }
}; 

struct MovePath {
    Bezier bezier;
    EaseType ease;
    float duration;
    MovePath();
};