#version 330 core

uniform vec4 aspects[30];
uniform vec2 sizes[30];

/*vec4 aspects[]=vec4[](
		vec4(0,	0.375,	0.0625,	0.0625),
		vec4(0,	0.3125,	0.0625,	0.0625),
		vec4(0,	.25,    0.0625,	0.0625),
		vec4(0,	0.1875,	0.0625,	0.0625),
		vec4(0,	1,	0.09375, 0.0625),
		vec4(0,	0.8125,	0.09375, 0.0625),
		vec4(0.625,	1, 0.09375, 0.09375),
		vec4(0.625,	0.71875, 0.09375, 0.09375),
		vec4(0,	0.125,	0.125,	0.125)
);*/

layout (location=0) in vec2 aPos;
layout (location=1) in float aAsp;
layout (location=2) in float aFrame;
layout (location=3) in float aAg;

out vec2 uv;

vec2 muv[4]=vec2[](
vec2(0,0),
vec2(1,0),
vec2(0,1),
vec2(1,1)
);
vec2 mp[4]=vec2[](
vec2(-.5,.5),
vec2(.5,.5),
vec2(-.5,-.5),
vec2(.5,-.5)
);
void main(){
	int i=int(aAsp);
	//int i=gl_InstanceID%9;

	vec2 uvPos=vec2(aspects[i].x,aspects[i].y);
	vec2 uvSize=vec2(aspects[i].z,-aspects[i].w);
	
	vec2 iuv=muv[gl_VertexID];

	uv=vec2(uvPos.x+uvSize.x*(iuv.x+int(aFrame)),uvPos.y+uvSize.y*iuv.y);


		//float aAgF=aAg-1.570796326679;
	gl_Position=vec4(aPos+mp[gl_VertexID]*sizes[i]*mat2(cos(aAg),sin(aAg),-sin(aAg),cos(aAg)),0,1);
	
}
