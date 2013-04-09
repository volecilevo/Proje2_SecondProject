#version 330

// vertex shaderindan gelen veriler
varying float varyingColor;

// fragment shader main metodu
void main(void)
{
    // gl_FragColor ekrana basilacak rengi secer
	// Posizyon y olarak 0 konumuna yakınken cos(0)=1 ve diğerleri 0 a yakın olacağından, dolayısıyla
	// renk background ile aynı renk olacağından, if kontrolü ile bu durumun önüne geçilmiştir.
	if((varyingColor<0.288 && varyingColor>0 )||(varyingColor>-0.288 && varyingColor<0 )){
		gl_FragColor = vec4(sin(varyingColor),cos(varyingColor)-0.12,cos(varyingColor)-0.3,1.0);
	}
	else{
		gl_FragColor = vec4(sin(varyingColor),cos(varyingColor),cos(varyingColor)-0.3,1.0);
	}
}
