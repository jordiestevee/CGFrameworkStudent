varying vec2 v_uv;
uniform sampler2D u_texture;

void main()
{
    vec4 color = texture2D(u_texture, v_uv);
    //a
    /*float grayscale = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    
    // Set the output color to the grayscale value 
    gl_FragColor = vec4(grayscale, grayscale, grayscale, color.a);*/
        
    //b    
    /*vec3 invertedColor = 1.0 - color.rgb;
    
    // Set the output color to the inverted color (keeping the alpha channel the same)
    gl_FragColor = vec4(invertedColor, color.a);*/

    //c
    /*float greenTone = color.g; // Take the green channel value
    vec3 green = vec3(0,1,0);
    vec3 greenFilter = green * greenTone;

    // Set the output color to have tones of green (keeping the red and blue channels unchanged)
    gl_FragColor = vec4(greenFilter, color.a);*/

    //d

    //e 

    //f
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