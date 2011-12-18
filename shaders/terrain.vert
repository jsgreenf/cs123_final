varying float lightIntensity;

uniform sampler2D planetTexture;
uniform int xDim;
uniform int yDim;
uniform float timer;
uniform int terrain_smoothing;

const float M_PI = 3.14159265;

float blurIntensity(sampler2D texture, int width){

    float xStep = 1.0/xDim;
    float yStep = 1.0/yDim;

    int numSamples = pow(2*width+1,2);
    float intensity = 0;

    for (int i = -width; i <= width; i++){
        for (int j = -width; j <= width; j++){

            vec2 texCoords = vec2(gl_TexCoord[0].s, gl_TexCoord[0].t);
            texCoords.x += i*xStep;
            //if (texCoords.x >= 1.0) texCoords.x -= 1.0;
            texCoords.y += j*yStep;
            //if (texCoords.y >= 1.0) texCoords.y -= 1.0;

            vec4 sample = texture2D(texture, texCoords);

            intensity += (.299*sample.x+.587*sample.y+.114*sample.z);
        }
    }

return intensity/numSamples;

}

void main()
{
    vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
    vec3 light = normalize(vec3(cos(timer/180.0*M_PI), 0, -sin(timer/180.0*M_PI)) - (gl_ModelViewMatrix * gl_Vertex)).xyz;
    lightIntensity = max(0.0, dot(normal, light));
    
    gl_TexCoord[0] = gl_MultiTexCoord0;    

    // take a blur of the surrounding terrain to determine the overall intensity and elevation
    float grayScale = pow(blurIntensity(planetTexture, terrain_smoothing)-.5,3);

    // set new height of the vertex based on the topography
    gl_Vertex.xyz = gl_Vertex.xyz+(gl_Vertex.xyz*(grayScale)*5);

    gl_Position = ftransform();
}
