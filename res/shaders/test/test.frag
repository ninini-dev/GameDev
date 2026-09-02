#version 330 core
out vec4 FragColor;
in vec2 uv;

uniform sampler2D tex;

void main()
{ 
    FragColor = texture(tex,vec2(uv.x,uv.y));
    //FragColor = vec4(uv.x,uv.y,0,1);
    
}