#include <stdlib.h>
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "../../utils/CoordsEsfericas.h"
#include "pugixml.hpp"

using namespace std;

vector<Ponto3D> pontos;
CoordsEsfericas camara;
GLenum modoPoligonos;

void menuDrawing(int opt) {
	switch (opt) {
	case 0: modoPoligonos = GL_FILL; break;
	case 1:  modoPoligonos = GL_LINE; break;
	case 2:  modoPoligonos = GL_POINT; break;
	}
	glutPostRedisplay();
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {

	glPolygonMode(GL_FRONT, modoPoligonos);

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camara.cCartesianas.x,camara.cCartesianas.y,camara.cCartesianas.z,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	// put the geometric transformations here

	glBegin(GL_TRIANGLES);

	for (auto it = pontos.begin(); it != pontos.end(); ++it) {
		glVertex3f(it->x, it->y, it->z);
	}
	
	glEnd();

	// End of frame
	glutSwapBuffers();
}

// write function to process keyboard events
void f_teclas_normais(unsigned char key, int x, int y) {
	switch (key) {
	case 'w': camara.paraCima(6 * M_PI / 360.0);  break;
	case 's': camara.paraBaixo(6 * M_PI / 360.0); break;
	case 'a': camara.paraEsquerda(6 * M_PI / 360.0);  break;
	case 'd': camara.paraDireita(6 * M_PI / 360.0); break;
	case 'e': camara.aproximar(0.5); break;
	case 'q': camara.afastar(0.5); break;
	}

	glutPostRedisplay();
}

void leXML() {
	float x, y, z;
	std::string modelo_prefix("../Modelos/");
	std::string ficheiro(modelo_prefix + "figura.xml");

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(ficheiro.c_str());
	
	if (!result) std::cout << "No result! :(" << std::endl;

	auto scene_node = doc.child("scene");

	for (auto it = scene_node.begin(); it != scene_node.end(); ++it) {
		string nome_ficheiro = it->attribute("file").value();

		ifstream fich_inp(modelo_prefix + nome_ficheiro);
		while (fich_inp) {
			fich_inp >> x >> y >> z;
			pontos.push_back(Ponto3D{ x,y,z });
		}
	}

}

int main(int argc, char **argv) {
	modoPoligonos = GL_LINE;
	leXML();
	camara = CoordsEsfericas(10.0, 0.0, M_PI / 3.0f);
// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,500);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	
// put here the registration of the keyboard callbacks
	glutKeyboardFunc(f_teclas_normais);

	int menu = glutCreateMenu(menuDrawing);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutAddMenuEntry("FILL", 0);
	glutAddMenuEntry("LINE", 1);
	glutAddMenuEntry("POINT", 2);
//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
