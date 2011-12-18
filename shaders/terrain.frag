uniform sampler2D planetTexture;

varying float lightIntensity;
 
void main()
{   
    gl_FragColor = texture2D(planetTexture, gl_TexCoord[0].st)*(.8+.4*lightIntensity);
}
