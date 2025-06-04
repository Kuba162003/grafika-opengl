#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec4 color = vec4(1,1,1,1);
uniform vec3 lightPos;    // pozycja �wiat�a punktowego w przestrzeni �wiata
uniform vec3 cameraPos;   // pozycja kamery (opcjonalna, do przysz�ych rozszerze�)

//Atrybuty
layout (location=0) in vec4 vertex;
layout (location=1) in vec4 normal;

//Zmienne interpolowane
out vec4 i_color;

void main(void) {
    gl_Position = P * V * M * vertex;

    // Pozycja fragmentu w przestrzeni �wiata
    vec3 fragPos = vec3(M * vertex);

    // Przekszta�cenie normalnej
    vec3 norm = normalize(mat3(transpose(inverse(M))) * vec3(normal));

    // Kierunek �wiat�a od fragmentu do �r�d�a �wiat�a
    vec3 lightDir = normalize(lightPos - fragPos);

    // O�wietlenie Lambertowskie
    float diff = max(dot(norm, lightDir), 0.0);
    i_color = vec4(color.rgb * diff, color.a);
}
