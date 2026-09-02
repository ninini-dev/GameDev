#version 330 core 

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in DATA
{
	vec2 uvPos;
	vec2 uvSize;
	vec2 size;
    float ag;
} data_in[];

out vec2 uv;

void main(){
    vec4 pos = gl_in[0].gl_Position; 
    float w = data_in[0].size.x/2;
    float h = data_in[0].size.y/2;
    vec2 uvP=data_in[0].uvPos;
    vec2 uvS=data_in[0].uvSize;
    
    float ag=data_in[0].ag;
    mat2 mat=mat2(cos(ag),sin(ag),-sin(ag),cos(ag));

    gl_Position=pos+vec4(vec2(-w,h)*mat,0,0);
    uv=uvP+vec2(0,uvS.y);
    EmitVertex();

    gl_Position=pos+vec4(vec2(w,h)*mat,0,0);
    uv=uvP+vec2(uvS.x,uvS.y);
    EmitVertex();

    gl_Position=pos+vec4(vec2(-w,-h)*mat,0,0);
    uv=uvP+vec2(0,0);
    EmitVertex();

    gl_Position=pos+vec4(vec2(w,-h)*mat,0,0);
    uv=uvP+vec2(uvS.x,0);
    EmitVertex();

    EndPrimitive();
}