/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"

GLuint tlo;
GLuint podloga;
GLuint sufit;

//Teksturowanie
GLuint ReadTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
	glEnable(GL_DEPTH_TEST);
	loadModels();
	tlo = ReadTexture("tlo.png");
	podloga = ReadTexture("podloga.png");
	sufit = ReadTexture("sufit.png");
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************	
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
	glDeleteTextures(1, &tlo);
	glDeleteTextures(1, &podloga);
	glDeleteTextures(1, &sufit);
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************	
}

float speed = 0;
float speed2 = 0;
float butterflyAngle = 0.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f); // patrzy "do przodu" wzdłuż +Z
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {
	const float cameraSpeed = 0.2f;

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_UP)
			cameraPos += cameraSpeed * cameraFront;
		if (key == GLFW_KEY_DOWN)
			cameraPos -= cameraSpeed * cameraFront;
		if (key == GLFW_KEY_LEFT)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (key == GLFW_KEY_RIGHT)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	// Obsługa WASD zostaje bez zmian
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) speed = 3.14;
		if (key == GLFW_KEY_D) speed = -3.14;
		if (key == GLFW_KEY_W) speed2 = -1;
		if (key == GLFW_KEY_S) speed2 = 1;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_D || key == GLFW_KEY_A)
			speed = 0;
		if (key == GLFW_KEY_W || key == GLFW_KEY_S)
			speed2 = 0;
	}
}



void LoadTexture(glm::mat4 P, glm::mat4 V, glm::mat4 M, GLuint tex) {
	spTextured->use();

	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, Models::cube.vertices);

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, Models::cube.texCoords);

	glEnableVertexAttribArray(spTextured->a("normal"));
	glVertexAttribPointer(spTextured->a("normal"), 3, GL_FLOAT, false, 0, Models::cube.normals);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(spTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, Models::cube.vertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("texCoord"));
	glDisableVertexAttribArray(spTextured->a("normal"));
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle, float angle2) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.4, 0.8f, 0.4f);

	//ruch motyla
	butterflyAngle += 0.01f; // Im mniejsza wartość, tym wolniejszy ruch
	if (butterflyAngle > 2 * 3.14159f) butterflyAngle -= 2 * 3.14159f;

	
	//ruch kamery
	glm::mat4 V = glm::lookAt(
		cameraPos,
		cameraPos + cameraFront,
		cameraUp);

	// Obracanie modelu
	V = glm::rotate(V, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	V = glm::rotate(V, angle2, glm::vec3(1.0f, 0.0f, 0.0f));


	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);

	spLambert->use();
	glm::vec3 lightDir = glm::normalize(glm::vec3(0.5f, 1.0f, 0.3f));
	glUniform3fv(spLambert->u("lightDir"), 1, glm::value_ptr(lightDir));
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V));

	// Pozycja światła punktowego (np. nad obiektem)
	glm::vec3 lightPos = glm::vec3(0.0f, 1.0f, 0.0f);
	glUniform3fv(spLambert->u("lightPos"), 1, glm::value_ptr(lightPos));

	// Pozycja kamery (opcjonalnie — na przyszłość)
	//glUniform3fv(spLambert->u("cameraPos"), 1, glm::value_ptr(cameraPos));


	//ruch motyla
	glm::mat4 M = glm::mat4(1.0f);
	float radius = 0.8f;
	float x = radius * cos(butterflyAngle);
	float z = radius * sin(butterflyAngle);
	M = glm::translate(M, glm::vec3(x, 0.0f, z));

	// Opcjonalnie: obrót tak, by motyl "patrzył" w kierunku ruchu
	M = glm::rotate(M, -butterflyAngle, glm::vec3(0.0f, 1.0f, 0.0f));

	M = glm::scale(M, glm::vec3(0.03f, 0.03f, 0.03f));

	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
	glUniform4f(spLambert->u("color"), 1.0, 0.5, 0.0, 1);
	ModelResources::motyl.drawSolid();

	//kwiatki
	//lodygi
	glm::mat4 K1 = glm::mat4(1.0f);
	K1 = glm::translate(K1, glm::vec3(0.8f, -1.0f, 0.0f));
	K1 = glm::scale(K1, glm::vec3(0.1f, 0.1f, 0.1f));
	K1 = glm::rotate(K1, 3.14f/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(K1));
	glUniform4f(spLambert->u("color"), 0.5, 1, 0.5, 1);
	ModelResources::lodyga.drawSolid();

	glm::mat4 K12 = glm::mat4(1.0f);
	K12 = glm::translate(K12, glm::vec3(-0.7f, -1.0f, 0.6f));
	K12 = glm::scale(K12, glm::vec3(0.1f, 0.1f, 0.1f));
	K12 = glm::rotate(K12, 3.14f / 4.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(K12));
	glUniform4f(spLambert->u("color"), 0.5, 1, 0.5, 1);
	ModelResources::lodyga.drawSolid();

	glm::mat4 K13 = glm::mat4(1.0f);
	K13 = glm::translate(K13, glm::vec3(0.1f, -1.0f, -0.6f));
	K13 = glm::scale(K13, glm::vec3(0.1f, 0.1f, 0.1f));
	K13 = glm::rotate(K13, 3.14f / 1.3f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(K13));
	glUniform4f(spLambert->u("color"), 0.5, 1, 0.5, 1);
	ModelResources::lodyga.drawSolid();
	
	//zalaznie
	glm::mat4 K2 = glm::mat4(1.0f);
	K2 = glm::translate(K1, glm::vec3(0.0f, 0.0f, 0.0f));
	K2 = glm::scale(K1, glm::vec3(0.0f, 0.0f, 0.0f));
	K2 = glm::rotate(K1, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(K2));
	glUniform4f(spLambert->u("color"), 1.0, 0.9, 0.0, 1);
	ModelResources::zalaznia.drawSolid();

	glm::mat4 K22 = glm::mat4(1.0f);
	K22 = glm::translate(K12, glm::vec3(0.0f, 0.0f, 0.0f));
	K22 = glm::scale(K12, glm::vec3(0.0f, 0.0f, 0.0f));
	K22 = glm::rotate(K12, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(K22));
	glUniform4f(spLambert->u("color"), 1.0, 0.9, 0.0, 1);
	ModelResources::zalaznia.drawSolid();

	glm::mat4 K23 = glm::mat4(1.0f);
	K23 = glm::translate(K13, glm::vec3(0.0f, 0.0f, 0.0f));
	K23 = glm::scale(K13, glm::vec3(0.0f, 0.0f, 0.0f));
	K23 = glm::rotate(K13, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(K23));
	glUniform4f(spLambert->u("color"), 1.0, 0.9, 0.0, 1);
	ModelResources::zalaznia.drawSolid();

	//platki
	glm::mat4 K3 = glm::mat4(1.0f);
	K3 = glm::translate(K1, glm::vec3(0.0f, 0.0f, 0.0f));
	K3 = glm::scale(K1, glm::vec3(0.0f, 0.0f, 0.0f));
	K3 = glm::rotate(K1, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(K3));
	glUniform4f(spLambert->u("color"), 1.0, 0.2, 0.0, 1);
	ModelResources::platki.drawSolid();

	glm::mat4 K32 = glm::mat4(1.0f);
	K32 = glm::translate(K12, glm::vec3(0.0f, 0.0f, 0.0f));
	K32 = glm::scale(K12, glm::vec3(0.0f, 0.0f, 0.0f));
	K32 = glm::rotate(K12, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(K32));
	glUniform4f(spLambert->u("color"), 0.0, 0.2, 0.9, 1);
	ModelResources::platki.drawSolid();

	glm::mat4 K33 = glm::mat4(1.0f);
	K33 = glm::translate(K13, glm::vec3(0.0f, 0.0f, 0.0f));
	K33 = glm::scale(K13, glm::vec3(0.0f, 0.0f, 0.0f));
	K33 = glm::rotate(K13, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(K33));
	glUniform4f(spLambert->u("color"), 0.4, 0.0, 0.6, 1);
	ModelResources::platki.drawSolid();

	//Tworzenie scian
	glm::mat4 M1 = glm::mat4(1.0f);
	M1 = glm::translate(M1, glm::vec3(0.0f, 1.0f, 0.0f));
	M1 = glm::scale(M1, glm::vec3(1.0f, 0.01f, 1.0f));
	LoadTexture(P, V, M1, sufit);
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M1));
	Models::cube.drawSolid();

	glm::mat4 M2 = glm::mat4(1.0f);
	M2 = glm::rotate(M2, 3.14f, glm::vec3(1.0f, 1.0f, 0.0f));
	M2 = glm::translate(M2, glm::vec3(0.0f, 1.0f, 0.0f));
	M2 = glm::scale(M2, glm::vec3(1.0f, 0.01f, 1.0f));
	LoadTexture(P, V, M2, tlo);
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M2));
	Models::cube.drawSolid();

	glm::mat4 M3 = glm::mat4(1.0f);
	M3 = glm::translate(M3, glm::vec3(0.0f, -1.0f, 0.0f));
	M3 = glm::scale(M3, glm::vec3(1.0f, 0.01f, 1.0f));
	LoadTexture(P, V, M3, podloga);
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M3));
	Models::cube.drawSolid();

	glm::mat4 M4 = glm::mat4(1.0f);
	M4 = glm::rotate(M4, 3.14f, glm::vec3(1.0f, 1.0f, 0.0f));
	M4 = glm::translate(M4, glm::vec3(0.0f, -1.0f, 0.0f));
	M4 = glm::scale(M4, glm::vec3(1.0f, 0.01f, 1.0f));
	LoadTexture(P, V, M4, tlo);
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M4));
	Models::cube.drawSolid();

	glm::mat4 M5 = glm::mat4(1.0f);
	M5 = glm::translate(M5, glm::vec3(0.0f, 0.0f, 1.0f));
	M5 = glm::rotate(M5, 3.14f/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	M5 = glm::scale(M5, glm::vec3(1.0f, 0.01f, 1.0f));
	LoadTexture(P, V, M5, tlo);
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M5));
	Models::cube.drawSolid();

	glfwSwapBuffers(window);
	
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(5000, 5000, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	initOpenGLProgram(window); //Operacje inicjujące
	float angle = 0;
	float angle2 = 0;
	glfwSetTime(0);

	//Główna pętla	
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{

		angle += speed * glfwGetTime();
		angle2 += speed2 * glfwGetTime();
		glfwSetTime(0);
		drawScene(window, angle, angle2); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
