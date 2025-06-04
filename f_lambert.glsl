#version 330

out vec4 pixelColor;

// Dane z vertex shadera
in vec4 i_color;

// Uniformy
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec4 color;

void main(void) {
    // Pozycja aktualnego fragmentu w przestrzeni �wiata (nie mamy jej, wi�c uproszczenie)
    // Zak�adamy, �e normalnie by�aby przekazywana jako interpolowana warto��, np. `fragPos`
    // Tu upro�cimy: przyjmujemy, �e fragmenty s� blisko kamery
    vec3 fragPos = vec3(0.0, 0.0, 0.0); // <- Najlepiej przekaza� realn� pozycj� fragmentu!

    float distance = length(lightPos - fragPos);

    // Parametry os�abienia �wiat�a
    float constant = 1.0;
    float linear = 0.22;
    float quadratic = 0.20;

    float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);

    vec3 baseColor = i_color.rgb * attenuation;

    pixelColor = vec4(baseColor, color.a);
}
