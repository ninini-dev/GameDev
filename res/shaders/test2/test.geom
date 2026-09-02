#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in DATA{
    float color;
} data_in[];
out vec4 color;

void main(){
    vec4 pos=gl_in[0].gl_Position;


    gl_Position=pos+vec4(-.1,.1,0,0);
    EmitVertex();

    color=vec4(gl_PrimitiveID*.5,0,0,1);
    gl_Position=pos+vec4(.1,.1,0,0);
    EmitVertex();

    color=vec4(gl_PrimitiveID*.5,0,0,1);
    gl_Position=pos+vec4(-.1,-.1,0,0);
    EmitVertex();

    color=vec4(gl_PrimitiveID*.5,0,0,1);
    gl_Position=pos+vec4(.1,-.1,0,0);
    EmitVertex();

    color=vec4(gl_PrimitiveID*.5,0,0,1);
    EndPrimitive();
}