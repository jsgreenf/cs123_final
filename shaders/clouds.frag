uniform sampler2D cloudTexture;

void main()
{
    vec4 sample = texture2D(cloudTexture, gl_TexCoord[0].st);
    sample.a = pow((.299*sample.x+.587*sample.y+.114*sample.z),2);
    gl_FragColor = sample;
}
