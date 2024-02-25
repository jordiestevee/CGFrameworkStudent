varying vec2 v_uv;
uniform float u_time;
uniform int subTask;


void main()
{
	// ex 1
	// a ok
    if (subTask == 1){
	    vec4 z= mix(vec4(1,0,0,1), vec4(0,0,1,1), v_uv.x);
	    gl_FragColor = z;
    }
    
	// b ok
    else if (subTask == 2){
        vec2 center = vec2(0.5);
	    float dist_ = distance(center, v_uv);
	    vec3 final_color = vec3(dist_);
	    gl_FragColor = vec4(final_color,0.0);
    }
	
	// c ok
    else if (subTask == 3){
        float redLines = step(0.075, mod(v_uv.x, 0.1));  
        float blueLines = step(0.025, mod(v_uv.y, 0.1));  
        vec3 color = vec3(redLines, 0.0, 1.0 - blueLines);  
        gl_FragColor = vec4(color, 1.0);
        }

	//d falten quadrats
    else if (subTask == 4){
        vec3 color = mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), v_uv.x + v_uv.y);
        gl_FragColor = vec4(color, 1.0);
    }    
	
	//e ok
    else if (subTask == 5){
        vec3 white = vec3(1.0, 1.0, 1.0);
        vec3 black = vec3(0.0, 0.0, 0.0);
                        
        float xline = floor(v_uv.x * 40.0);
        float patt = mod(xline + floor(v_uv.y * 20.0), 2.0);
        vec3 color = mix(white, black, step(0.5, patt));                
        gl_FragColor = vec4(color, 1.0);
    }
    
    else if (subTask == 6){
        // Frecuencia del seno
        float frequency = 5.0;

        // Amplitud del seno
        float amplitude = 0.5;

        // Valor sinusoidal
        float sineValue = sin(v_uv.x * frequency * 2 * 3.141592) * amplitude;

        // Color verde degradado con negro
        vec4 color = vec4(0.0, 1.0 - abs(sineValue), 0.0, 1.0);
        gl_FragColor=color;
    }
}
