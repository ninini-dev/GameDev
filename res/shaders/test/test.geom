#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

in DATA
{
	float l;
    float aspect;
    float w;
} data_in[];

out vec2 uv;

void main(){
    float w=data_in[0].w;
    float off=data_in[0].aspect/16.0;

    vec4 prevD= gl_in[1].gl_Position-gl_in[0].gl_Position;
    vec4 curD=gl_in[2].gl_Position-gl_in[1].gl_Position;
    vec4 prevN=vec4(prevD.y,-prevD.x,0,0);
    vec4 curN=vec4(curD.y,-curD.x,0,0);
    prevN/=length(prevN);
    curN/=length(curN);
    prevN*=w;
    curN*=w;

    float l=data_in[2].l;

    gl_Position=gl_in[2].gl_Position-curN;
    uv=vec2(l,off);
    EmitVertex();
    gl_Position=gl_in[2].gl_Position+curN;
    uv=vec2(l,off+0.0625);
    EmitVertex();

    l=data_in[1].l;

    gl_Position=gl_in[1].gl_Position-prevN;
    uv=vec2(l,off);
    EmitVertex();
    gl_Position=gl_in[1].gl_Position+prevN;
    uv=vec2(l,off+0.0625);
    EmitVertex();

    EndPrimitive();
    
}
