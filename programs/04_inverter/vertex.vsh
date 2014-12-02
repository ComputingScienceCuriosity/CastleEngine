attribute vec4 qt_Vertex;
attribute vec4 qt_Color;
varying vec4 col;

void main(void)
{
    col = vec4(1.0, 1.0, 1.0, 1.0) - qt_Color;
    gl_Position = qt_Vertex;
}
