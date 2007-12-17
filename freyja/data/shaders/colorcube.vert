// This is a lame colorcube shader I whipped up, because I love colorcubes
void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
