varying vec2 v_uv;
uniform float u_time;
uniform int subTask;


void main()
{
    if (subTask == 1){
	    vec4 z= mix(vec4(1,0,0,1), vec4(0,0,1,1), v_uv.x);

	    gl_FragColor = z;
    }
    
    else if (subTask == 2){
        vec2 center = vec2(0.5);
	    float dist_ = distance(center, v_uv);
	    vec3 final_color = vec3(dist_);

	    gl_FragColor = vec4(final_color,0.0);
    }
	
    else if (subTask == 3){
        float redLines = step(0.075, mod(v_uv.x, 0.1));  
        float blueLines = step(0.025, mod(v_uv.y, 0.1));  
        vec3 color = vec3(redLines, 0.0, 1.0 - blueLines); 

        gl_FragColor = vec4(color, 1.0);
    }

    else if (subTask == 4){
        float pixelSize = 0.05;

        // Sets each uv to its corresponding pixel
        vec2 pixelCoord = floor(v_uv / pixelSize) * pixelSize;

        // Interpolate black and green in y axis
        vec3 color = mix(vec3(0.0, .0, 0.0), vec3(0.0, 1.0, 0.0), pixelCoord.y);
        // Interpolate red and yellow in y axis
        vec3 color2 = mix(vec3(1.0, 0.0, 0.0), vec3(1.0, 1.0, 0.0), pixelCoord.y);
        // Interpolate the two previous interpolations in the x axis
        vec3 color3 = mix(color, color2, (pixelCoord.x));

        gl_FragColor = vec4(color3, 1.0);
    }    
	
    else if (subTask == 5){
        vec3 white = vec3(1.0, 1.0, 1.0);
        vec3 black = vec3(0.0, 0.0, 0.0);
                        
        float xline = floor(v_uv.x * 40.0);
        float patt = mod(xline + floor(v_uv.y * 20.0), 2.0);
        vec3 color = mix(white, black, step(0.5, patt)); 

        gl_FragColor = vec4(color, 1.0);
    }
    
    else if (subTask == 6){
        float amplitude = 0.4;
        // Calculate a step function based on the y-coordinate of the UV texture coordinates and a sine wave
        float y = step(v_uv.y, amplitude * sin(2 * 3.14159 * v_uv.x) + 0.5);

        // Calculate the distance between the y-coordinate of the UV coordinates and the value of y
        float distance = (v_uv.y - y);

        // Interpolate between green and black based on the distance
        vec3 color = mix(vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 0.0), distance);

        gl_FragColor = vec4(color, 1.0);
    }

    else{
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
