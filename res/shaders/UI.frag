#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

uniform bool isText;
uniform sampler2D texture1;
uniform vec3 textColor;
uniform vec3 color;
uniform float alpha;

void main()
{
    if (isText) {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture1, TexCoord).r);
        vec4 texColor = vec4(textColor, 1.0) * sampled;
        if(texColor.a < 0.5)
            discard;
        FragColor = vec4(texColor.rgb, 1.0f);
    }
    else {
        vec4 texColor = texture(texture1, TexCoord);
        if(texColor.a < 0.5)
            discard;
        FragColor = texColor * vec4(color, alpha);
    }
}