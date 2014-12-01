#version 400

in vec2 frag_uv;

out vec4 colour;

uniform sampler2D texSampler;

void main()
{
    colour = texture(texSampler, frag_uv).rgba;
    //float col = texture(texSampler, frag_uv).r;
    //colour = vec4(col, col, col, 1.0f);
}