varying vec2 v_uv;
uniform sampler2D u_texture;
uniform int subTask;
uniform float rotationAngle;
uniform float u_time;


void main()
{
    vec4 color = texture2D(u_texture, v_uv);

	// ROTATION
    if (subTask == 1){

        float angle = u_time * rotationAngle; 
        float rotatedX = cos(angle) * (v_uv.x - 0.5) - sin(angle) * (v_uv.y - 0.5) + 0.5;
        float rotatedY = sin(angle) * (v_uv.x - 0.5) + cos(angle) * (v_uv.y - 0.5) + 0.5;

        // Sample the texture using the rotated UV coordinates
        vec4 color = texture(u_texture, vec2(rotatedX, rotatedY));

        // Output the color
        gl_FragColor = color;

    }

	// PIXELIZATION
    else if (subTask == 2){

        // Define the size of each pixel
        vec2 pixelSize = vec2(0.05, 0.05);
        // Calculate time-dependent pixelation factor
        float pixelationFactor = abs(sin(u_time));


        // Calculate quantized texture coordinates
        vec2 pixelCoord = floor(v_uv / pixelSize) * pixelSize;

        // Adjust quantized coordinates based on pixelation factor
        vec2 pixelatedUV = mix(v_uv, pixelCoord, pixelationFactor);


        // Sample texture at pixelated coordinates
        vec4 color = texture2D(u_texture, pixelatedUV);

        gl_FragColor = color;


    }

    else{
        gl_FragColor = vec4(color);
    }
}