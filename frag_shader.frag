#version 330

// vertex shaderindan gelen veriler
varying vec3 normal;
varying float pos;

// fragment shader main metodu
void main(void)
{
    // gl_FragColor ekrana basilacak rengi secer
    gl_FragColor = vec4(pos*5,pos*5,pos*5,1.0);
    //gl_FragColor = vec4(normalize(normal),1.0);
}
