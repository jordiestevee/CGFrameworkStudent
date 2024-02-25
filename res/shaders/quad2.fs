varying vec2 v_uv;
uniform sampler2D u_texture;
uniform int subTask;

void main()
{
    vec4 color = texture2D(u_texture, v_uv);

    if (subTask == 1){
        float grayscale = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));

        // Set the output color to the grayscale value 
        gl_FragColor = vec4(grayscale, grayscale, grayscale, color.a);
    }

    else if (subTask == 2){
        vec3 invertedColor = 1.0 - color.rgb;

        // Set the output color to the inverted color (keeping the alpha channel the same)
        gl_FragColor = vec4(invertedColor, color.a);
    }

    else if  ( subTask == 3){
        // Take the green channel value
        float greenTone = color.g; 
        vec3 green = vec3(0,1,0);
        vec3 greenFilter = green * greenTone;

        // Set the output color to have tones of green 
        gl_FragColor = vec4(greenFilter, color.a);
    }

    else if (subTask == 4){
        // Apply black and white, with higher illumination
        float m = (color.x + color.y + color.z)/3.0;
        color.x = m*3.0;
        color.y = m*3.0;
        color.z = m*3.0;

        float intensity = (color.x+color.y+color.z)/3.0;
        vec4 finalcolor = color*step(1.25,intensity)*color;

        gl_FragColor = finalcolor;
    }

    else if (subTask == 5){
        vec2 center = vec2(0.5, 0.5);
        float distanceToCenter = distance(center, v_uv);
        float gradientRadius = 0.7;
        
        // Calculate the gradient strength based on the distance to the center
        float gradientStrength = 1.0 - smoothstep(0.0, gradientRadius, distanceToCenter);
        
        // Apply the gradient effect to darken the sides and fade away towards the center
        vec3 finalColor = color.rgb * gradientStrength;
        
        // Set the output color
        gl_FragColor = vec4(finalColor, color.a);
    }
    
    else if (subTask == 6){
        float blur = 0.01;
        int numSamples = 5;
        vec3 finalColor = vec3(0.0);
        
        // Apply blur
        for (int i = -numSamples/2; i <= numSamples/2; ++i) {
            for (int j = -numSamples/2; j <= numSamples/2; ++j) {
                vec4 SampleColor = texture2D(u_texture, v_uv + vec2(float(i), float(j)) * blur);
                finalColor += SampleColor.rgb;
            }
        }
        // Normalize by number of samples
        finalColor /= float(numSamples * numSamples); 
        gl_FragColor = vec4(finalColor, color.a);
    }

    else{
        gl_FragColor = vec4(color);
    }

}