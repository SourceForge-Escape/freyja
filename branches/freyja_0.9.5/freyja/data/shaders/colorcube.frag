// This is a lame colorcube shader I whipped up, because I love colorcubes
void main (void)
{
	float b = 1.0 - (gl_TexCoord[0].s + gl_TexCoord[0].t);
	vec3 color = { gl_TexCoord[0].s, gl_TexCoord[0].t, b };

	gl_FragColor = vec4(color, 1.0);
}
