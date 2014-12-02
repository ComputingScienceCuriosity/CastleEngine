varying vec4 col;

void main(void)
{
    gl_FragColor = col * col * col * col * col;
}
