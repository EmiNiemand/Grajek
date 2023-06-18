#version 430

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D texturePosition;
uniform sampler2D textureNormal;
uniform sampler2D textureColor;

// prewitt
mat3 sx = mat3(
    1.0, 1.0, 1.0,
    0.0, 0.0, 0.0,
    -1.0, -1.0, -1.0
);

mat3 sy = mat3(
    1.0, 0.0, -1.0,
    1.0, 0.0, -1.0,
    1.0, 0.0, -1.0
);

int SIZE = 5;
float SEPARATION = 3.0f;
float THRESHOLD = 0.5f;
float AMOUNT = 2.0f;

void main()
{
    float value = 0.0;
    float count = 0.0;

    vec3 diffuse = texture(screenTexture, TexCoords.st).rgb;
    vec2 texSize = textureSize(screenTexture, 0).xy + 2;
    vec2 fragCoord = gl_FragCoord.xy;
    vec2 texCoord = fragCoord / texSize;

    vec4 result = vec4(0);
    vec4 color = vec4(0.0);

    for (int i = -SIZE; i <= SIZE; ++i) {
        for (int j = -SIZE; j <= SIZE; ++j) {
            color = texture(screenTexture, (fragCoord + (vec2(i, j) * SEPARATION)) / texSize);

            value = max(color.r, max(color.g, color.b));
            if (value < THRESHOLD) { color = vec4(0); }

            result += color;
            count  += 1.0;
        }
    }

    mat3 I;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            vec3 sam  = texelFetch(textureColor, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0 ).rgb;
            I[i][j] = length(sam);
        }
    }

    float gx = dot(sx[0], I[0]) + dot(sx[1], I[1]) + dot(sx[2], I[2]);
    float gy = dot(sy[0], I[0]) + dot(sy[1], I[1]) + dot(sy[2], I[2]);

    float g = sqrt(pow(gx, 2.0) + pow(gy, 2.0));

    g = smoothstep(0.2, 0.8, g);

    vec3 edgeColor = vec3(0.1, 0.1, 0.1);
    result /= count;

    color = mix(vec4(0), result, AMOUNT);

    float minSeparation = 0.5;
    float maxSeparation = 1.0;
    float minDistance = 0.0;
    float maxDistance = 1.0;
    int size = 1;
    vec3 colorModifier = vec3(0.01, 0.01, 0.01);

    float near = 0.1f;
    float far = 7.5f;

    vec4 position = texture(texturePosition, texCoord);

    float depth = clamp(1.0 - ((far - position.y) / (far - near)), 0.0, 1.0);

    float separation = mix(maxSeparation, minSeparation, depth);

    float mx = 0.0;

    for (int i = 0; i <= size; ++i) {
        for (int j = 0; j <= size; ++j) {
            texCoord = (fragCoord + (vec2(i, j) * separation)) / texSize;

            vec4 positionTemp = texture(texturePosition, texCoord);

            mx = max(mx, abs(position.y - positionTemp.y));
        }
    }

    float diff = smoothstep(minDistance, maxDistance, mx);
    vec3 lineColor = texture(screenTexture, texCoord).rgb * colorModifier;

    color = vec4(mix(color.rgb, lineColor, clamp(diff, 0.0, 1.0)), 1.0);

    FragColor = color;
}