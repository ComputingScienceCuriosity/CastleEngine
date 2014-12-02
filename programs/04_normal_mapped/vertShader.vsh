void main(void)
{
	gl_Position = ftransform() ;
	
	vec4 N = gl_ModelViewMatrix * vec4(gl_Normal, 0.);
	gl_FrontColor = N;
}
