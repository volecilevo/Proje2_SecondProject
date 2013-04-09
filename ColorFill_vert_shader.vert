#version 330

// sabit degiskenler
uniform mat4 modelViewProjectionMatrix;
uniform float color;

// diziden alinacak degiskenler
attribute vec4 Position;


varying float varyingColor;


// vertex shader main metodu
void main()
{	
    // gl_Position ekranda noktanin nerede olacagini belirtir.
    gl_Position = modelViewProjectionMatrix * Position;
	// varyingColor değerini y ekseni üzerindeki konumun mutlak değerinden alacaktır.
	varyingColor = abs(gl_Position.y);
}