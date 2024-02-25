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
        float greenTone = color.g; // Take the green channel value
        vec3 green = vec3(0,1,0);
        vec3 greenFilter = green * greenTone;

        // Set the output color to have tones of green (keeping the red and blue channels unchanged)
        gl_FragColor = vec4(greenFilter, color.a);
    }
    else if (subTask == 5){
        // Define the center of the gradient 
        vec2 center = vec2(0.5, 0.5);
        
        // Calculate the distance from the current texture coordinate to the center
        float distanceToCenter = distance(center, v_uv);
        
        // Define the radius of the gradient (you can adjust this as needed)
        float gradientRadius = 0.7;
        
        // Calculate the gradient strength based on the distance to the center
        float gradientStrength = 1.0 - smoothstep(0.0, gradientRadius, distanceToCenter);
        
        // Apply the gradient effect to darken the sides and fade away towards the center
        vec3 finalColor = color.rgb * gradientStrength;
        
        // Set the output color
        gl_FragColor = vec4(finalColor, color.a);
    }
    else if (subTask == 6){
        // Define blur (adjust as needed)
        float blur = 0.01;

        // Define the number of samples
        int numSamples = 5;

        // Initialize final color
        vec3 finalColor = vec3(0.0);
        
        // Apply blur
        for (int i = -numSamples/2; i <= numSamples/2; ++i) {
            for (int j = -numSamples/2; j <= numSamples/2; ++j) {
                vec4 SampleColor = texture2D(u_texture, v_uv + vec2(float(i), float(j)) * blur);
                finalColor += SampleColor.rgb;
            }
        }
        finalColor /= float(numSamples * numSamples); // Normalize by number of samples
        gl_FragColor = vec4(finalColor, color.a);
    }
    else {
        gl_FragColor = vec4(color);
    }

}