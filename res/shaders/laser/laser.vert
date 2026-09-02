#version 330 core
layout (location = 0) in vec2 aPos; // Input vertex position
//layout (location = 1) in vec2 aUv;
uniform float vertexCount;
uniform float color;
uniform float first;
out vec2 uv;
void main()
{
    gl_Position = vec4(aPos.x,aPos.y,0, 1.0);

    int fix=gl_VertexID%400;
    int a=(fix+1)%6;
    if(a>2)uv.y=0.0625f*(color+1);
    else uv.y=0.0625f*color;
    
    uv.x = (((fix+1)/3 + (fix+1)%3)-1-first)/(vertexCount-1);
}