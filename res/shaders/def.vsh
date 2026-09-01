#version 460 core
layout (location=0) in vec2 aPos;
uniform int[100] uOff;
uniform int[100] uLen;
out vec2 TexCoord;
void main(){
	float u = float((gl_VertexID >> 1) - uOff[gl_DrawID >> 1]) / float(uLen[gl_DrawID >> 1]);
    float v = float(gl_VertexID % 2) * 1.0 / 16.0;

    TexCoord = vec2(u, v);
	gl_Position=vec4(aPos,0,1);	
}