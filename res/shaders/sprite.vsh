#version 460 core
layout (location = 0) in vec4 aQuadPos; 
layout (location = 1) in float aX;     
layout (location = 2) in float aY;
layout (location = 3) in float adX;
layout (location = 4) in float adY;
layout (location = 5) in int aSprite;
uniform vec4[32] uSprite;
out vec2 TexCoord;
void main() {
    
    
    int spriteIndex = aSprite & 0xFFFF;
    int spriteOffset = (aSprite >> 16) & 0xFFFF;
    
    vec2 worldPos = aQuadPos.xy * uSprite[spriteIndex].zw;
    //TexCoord=vec2(0,0);
    mat2 rMat = mat2(
        adY, -adX,
        adX, adY
    );
    
    worldPos = (rMat * worldPos) + vec2(aX, aY);
    
    TexCoord = uSprite[spriteIndex].xy + aQuadPos.zw * uSprite[spriteIndex].zw;
    TexCoord.x += spriteOffset * uSprite[spriteIndex].z;
    
    gl_Position = vec4(worldPos, 0.0, 1.0); 
}       