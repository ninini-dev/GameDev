#version 330 core

layout(location=0) in vec2 pos;
layout(location=1) in float color;

out DATA{
    float color;
} data_out;

void main(){
    gl_Position=vec4(pos,0,1);
    data_out.color=color;
}