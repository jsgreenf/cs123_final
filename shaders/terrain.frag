
uniform float dirtMin, dirtMax;
uniform float grassMin, grassMax;
uniform float rockMin, rockMax;
uniform float snowMin, snowMax;
    
uniform sampler2D dirtTexture;
uniform sampler2D grassTexture;
uniform sampler2D rockTexture;
uniform sampler2D snowTexture;

varying float height;
varying float intensity;

vec4 oneit(float regionMax,float regionMin,sampler2D regionTexture)
{
    float regionRange = regionMax - regionMin;
    float regionWeight = (regionRange - abs(height - regionMax))/regionRange;
    regionWeight = max(0.0,regionWeight);
    vec4 sample = texture2D(regionTexture,gl_TexCoord[0].st);
    vec4 result = sample * regionWeight;
    return result;
}   
vec4 sampleTextures()

{
    vec4 sum = oneit(dirtMax,dirtMin,dirtTexture) + oneit(grassMax,grassMin,grassTexture)+oneit(rockMax,rockMin,rockTexture)+oneit(snowMax,snowMin,snowTexture);
    return sum;
   
    
    
    
}
 
void main()
{   
    gl_FragColor = sampleTextures() * intensity;
}
