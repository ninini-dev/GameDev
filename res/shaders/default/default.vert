#version 330 core

uniform vec4 aspects[30];
uniform vec2 sizes[30];

layout (location=0) in vec2 aPos;
layout (location=1) in float aAsp;
layout (location=2) in float aFrame;
layout (location=3) in float aAg;

out DATA
{
	vec2 uvPos;
	vec2 uvSize;
	vec2 size;
    float ag;
} data_out;

void main(){
	gl_Position = vec4(aPos.x,aPos.y,0, 1.0);
	data_out.size=sizes[int(aAsp)];
	vec2 uvS=aspects[int(aAsp)].zw;
	data_out.uvPos=aspects[int(aAsp)].xy+vec2(uvS.x*aFrame,-uvS.y);
	data_out.uvSize=uvS;
	data_out.ag=aAg;
}