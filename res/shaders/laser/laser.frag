#version 330 core
out vec4 FragColor;

uniform sampler2D tex;

in vec2 uv;
void main()
{
    FragColor = texture(tex,vec2(uv.x,uv.y));//+vec4(0,.5f,0,.5f);
}