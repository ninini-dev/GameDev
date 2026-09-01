#version 460 core

in vec2 TexCoord;
uniform sampler2D colorTex0;
out vec4 FragColor;
void main(){
	FragColor = texture(colorTex0, TexCoord.xy);
    //FragColor=vec4(TexCoord.xy,0,1);
}