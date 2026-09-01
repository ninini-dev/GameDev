#version 460 core

in vec2 TexCoord;
uniform sampler2D colorTex0;
out vec4 FragColor;
void main(){
	FragColor = texture(colorTex0, TexCoord);
	//FragColor.r=DRAWID;
	//FragColor=vec4(1,0,0,1);
}