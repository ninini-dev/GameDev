#version 460 core
layout (location = 0) in vec4 aQuadPos; 
layout (location = 1) in float aX;     
layout (location = 2) in float aY;
layout (location = 5) in int aSprite;
uniform vec4[32] uSprite;
uniform float uTime;
out vec2 TexCoord;
flat out int insID;
void main() {
    
    
    int spriteIndex = aSprite & 0xFFFF;
    int spriteOffset = (aSprite >> 16) & 0xFFFF;
    
    vec2 worldPos = aQuadPos.xy * uSprite[spriteIndex].zw;
    //TexCoord=vec2(0,0);
  
    insID = 5-gl_InstanceID%6;
    float mask = float(insID != 0);
    float nY=aY;
    nY-=insID*.05f;
    nY+=0.05f*mask;
    nY-=uTime*.01f*mask;
    worldPos = worldPos + vec2(aX, nY);//
    
    TexCoord = uSprite[spriteIndex].xy + aQuadPos.zw * uSprite[spriteIndex].zw;
    TexCoord.x += spriteOffset * uSprite[spriteIndex].z;
    
    gl_Position = vec4(worldPos, 0.0, 1.0); 
}       