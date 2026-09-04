#version 460 core

in vec2 TexCoord;
uniform sampler2D colorTex0;
out vec4 FragColor;
flat in int insID;
uniform float uTime;
void main(){
	FragColor = texture(colorTex0, TexCoord.xy);
    
    FragColor=mix(FragColor,vec4(.06,0.7,1.0,FragColor.a),(insID == 0) ? 0.0:.5f);

    FragColor.a*=mix(1.0,0.0,insID/6.0);
    
    FragColor.a-= (insID == 0) ? 0.0 :uTime*0.0416667;//1/24
}