#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec4 color = vec4(1,1,1,1);
uniform vec3 lightPos;    // pozycja œwiat³a punktowego w przestrzeni œwiata
uniform vec3 cameraPos;   // pozycja kamery (opcjonalna, do przysz³ych rozszerzeñ)

//Atrybuty
layout (location=0) in vec4 vertex;
layout (location=1) in vec4 normal;

//Zmienne interpolowane
out vec4 i_color;

void main(void) {
    gl_Position = P * V * M * vertex;

    // Pozycja fragmentu w przestrzeni œwiata
    vec3 fragPos = vec3(M * vertex);

    // Przekszta³cenie normalnej
    vec3 norm = normalize(mat3(transpose(inverse(M))) * vec3(normal));

    // Kierunek œwiat³a od fragmentu do Ÿród³a œwiat³a
    vec3 lightDir = normalize(lightPos - fragPos);

    // Oœwietlenie Lambertowskie
    float diff = max(dot(norm, lightDir), 0.0);
    i_color = vec4(color.rgb * diff, color.a);
}
