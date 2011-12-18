uniform samplerCube CubeMap;

varying vec3 normal, lightDir, r;

void main (void)
{
        vec4 final_color = .4*vec4(.5,.7,.7,1) + .6*textureCube( CubeMap, r);
	vec3 N = normalize(normal);
        vec3 R = normalize(r);
	vec3 L = normalize(lightDir);
        float lambertTerm = dot(r,L);
	if(lambertTerm > 0.0)
	{
		// Specular
                //final_color += pow(lambertTerm,1) * textureCube( CubeMap, r) * 5;
                final_color += pow(lambertTerm,7) * vec4(1,1,1,1);
	}
        final_color.a = .9;
        gl_FragColor = final_color;

}
