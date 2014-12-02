attribute vec3 qt_Vertex;
attribute vec3 qt_Normal;
uniform mat4 qt_ModelViewProjectionMatrix;
uniform vec3 light_direction;
uniform vec4 light_color;
varying vec4 col;

void main(void)
{
    //gl_Position = /*qt_ModelViewProjectionMatrix * */vec4(qt_Vertex, 1.0);
    gl_Position = gl_Vertex;
    gl_FrontColor = dot(light_direction, gl_Normal) * light_color;
}
