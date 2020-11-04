
#include "Functions.h"

using namespace cocos2d;


float scaleValue(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {
    
    if (fabs(inputMin - inputMax) < FLT_EPSILON){
        return outputMin;
    } else {
        float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
        
        if( clamp ){
            if(outputMax < outputMin){
                if( outVal < outputMax )outVal = outputMax;
                else if( outVal > outputMin )outVal = outputMin;
            }else{
                if( outVal > outputMax )outVal = outputMax;
                else if( outVal < outputMin )outVal = outputMin;
            }
        }
        return outVal;
    }
    
}

float perlinNoise( float x ) {
    int i0 = FASTFLOOR(x);
    int i1 = i0 + 1;
    float x0 = x - i0;
    float x1 = x0 - 1.0f;
    float t1 = 1.0f - x1*x1;
    float n0, n1;
    
    float t0 = 1.0f - x0*x0;
    /*  if(t0 < 0.0f) t0 = 0.0f; // this never happens for the 1D case */
    t0 *= t0;
    n0 = t0 * t0 * grad1(perm[i0 & 0xff], x0);
    
    /*  if(t1 < 0.0f) t1 = 0.0f; // this never happens for the 1D case */
    t1 *= t1;
    n1 = t1 * t1 * grad1(perm[i1 & 0xff], x1);
    /* The maximum value of this noise is 8*(3/4)^4 = 2.53125 */
    /* A factor of 0.395 would scale to fit exactly within [-1,1], but */
    /* we want to match PRMan's 1D noise, so we scale it down some more. */
    return 0.25f * (n0 + n1);
}


cocos2d::Color3B color_soundObject0() {
    cocos2d::Color3B color = cocos2d::Color3B( 255, 0, 0 );
    return color;
}
cocos2d::Color3B color_soundObject1() {
    cocos2d::Color3B color = cocos2d::Color3B( 150, 150, 150 );
    return color;
}
cocos2d::Color3B color_soundObject2() {
    cocos2d::Color3B color = cocos2d::Color3B( 255, 255, 0 );
    return color;
}
cocos2d::Color3B color_soundObject3() {
    cocos2d::Color3B color = cocos2d::Color3B( 0, 255, 0 );
    return color;
}
cocos2d::Color3B color_soundObject4() {
    cocos2d::Color3B color = cocos2d::Color3B( 0, 255, 255 );
    return color;
}
cocos2d::Color3B color_soundObject5() {
    cocos2d::Color3B color = cocos2d::Color3B( 0, 100, 255 );
    return color;
}
cocos2d::Color3B color_soundObject6() {
    cocos2d::Color3B color = cocos2d::Color3B( 255, 0, 255 );
    return color;
}
