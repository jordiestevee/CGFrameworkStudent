varying vec2 v_uv;
uniform sampler2D u_texture;
uniform int subTask;
uniform float rotationAngle;


void main()
{
    vec4 color = texture2D(u_texture, v_uv);

	// ROTATION
    if (subTask == 1){

		// Rotate the image around its center
		// Sample the texture
        float rotatedX = cos(rotationAngle) * (v_uv.x - 0.5) - sin(rotationAngle) * (v_uv.y - 0.5) + 0.5;
        float rotatedY = sin(rotationAngle) * (v_uv.x - 0.5) + cos(rotationAngle) * (v_uv.y - 0.5) + 0.5;

        // Sample the texture using the rotated UV coordinates
        vec4 color = texture(u_texture, vec2(rotatedX, rotatedY));

        // Output the color
        gl_FragColor = color;
    }

	// PIXELIZATION
    else if (subTask == 2){

		// Pixelates the image by dividing it into squares of pixelSize
        vec2 pixelSize = vec2(0.05, 0.05);

        // Calculate the coordinate of the bottom-left corner of the pixel
        vec2 pixelCoord = floor(v_uv / pixelSize) * pixelSize;

        // Sample the color of the texture at the pixel coordinate
        vec4 pixelColor = texture2D(u_texture, pixelCoord);

        // Assign the color of the pixel to the fragment
        gl_FragColor = pixelColor;
    }

    else{
        gl_FragColor = vec4(color);
    }
}

