varying vec2 v_uv;
uniform sampler2D u_texture;
uniform int subTask;

void main()
{
    vec4 color = texture2D(u_texture, v_uv);
    //gl_FragColor = vec4(color);
	vec2 pixelSize = vec2(0.05, 0.05);

	// Calculamos la coordenada de la esquina inferior izquierda del píxel
    	vec2 pixelCoord = floor(v_uv / pixelSize) * pixelSize;

	// Muestreamos el color de la textura en la coordenada del píxel
	vec4 pixelColor = texture2D(u_texture, pixelCoord);

	// Asignamos el color del píxel al fragmento
	gl_FragColor = pixelColor;
}

