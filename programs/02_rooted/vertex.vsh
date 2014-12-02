attribute vec4 qt_Vertex;
attribute vec4 qt_Color;
varying vec4 col;

void main(void)
{
    col = qt_Color;
    gl_Position = qt_Vertex;
}
