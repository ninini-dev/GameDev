#version 330 core
layout (location = 0) in vec2 aPos; 
layout (location=1) in float aA;
layout (location=2) in float aF;
layout (location=3) in float aL;
layout (location=4) in float aW;

out DATA
{
	float l;
    float aspect;
    float w;
} data_out;


void main()
{
    gl_Position = vec4(aPos.x,aPos.y,0, 1.0);
    data_out.l=(gl_VertexID-aF*3)/3/aL;
    data_out.aspect=aA;
    data_out.w=aW;
}