varying vec3 normal, lightDir, r;

uniform float timer;
uniform float cameraX;
uniform float cameraY;
uniform float cameraZ;

const float M_PI = 3.14159265;

const vec3 L = vec3(0.,0.,1.);
void main()
{	
    float sunX = cos(timer/180.0*M_PI)*5;
    float sunY = 0;
    float sunZ = -sin(timer/180.0*M_PI)*5;

    vec3 cameraPos = vec3(cameraX, cameraY, cameraZ);
    vec3 sunPos = vec3(sunX, sunY, sunZ);

    gl_Position = ftransform();
    vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
    lightDir = vec3(sunPos - vVertex);
    vec4 eyeVec = gl_ProjectionMatrixInverse*vec4(0,0,-1,0);
    normal = normalize( gl_NormalMatrix * gl_Normal );
    vec3 I = normalize(vVertex - eyeVec.xyz); // Eye to vertex
    r = reflect(I,normal);
}

