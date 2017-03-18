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
#include "../../utils/Grupo.h"

using namespace std;
vector<pugi::xml_node> stack;
vector<Ponto3D> pontos;
CoordsEsfericas camara = CoordsEsfericas(10.0, 0.0, M_PI / 3.0f);
GLenum modoPoligonos = GL_LINE;
GLenum modoFace = GL_FRONT;
float cameraSpeed = 6.0f;
float bg_red =0.0, bg_green =0.0, bg_blue = 0.0;
float pt_red = 1.0, pt_green = 1.0, pt_blue = 1.0;
int mouse_x, mouse_y;
pugi::xml_document doc;

void percorreXML() {
	std::string modelo_prefix("../Modelos/");
	auto scene_node = doc.child("scene");

	for (auto it = scene_node.begin(); it != scene_node.end(); ++it) {
		string nome_ficheiro = it->attribute("file").value();

		ifstream fich_inp(modelo_prefix + nome_ficheiro);
		while (fich_inp >> x >> y >> z) {
			pontos.push_back(Ponto3D{ x,y,z });
		}
	}

}

void main_menu_func(int opt) {
	switch (opt) {
	case 1:  exit(0); break;
	}
}

void polMode_menu_func(int opt) {
	switch (opt) {
	case 1:  modoPoligonos = GL_FILL; break;
	case 2:  modoPoligonos = GL_LINE; break;
	case 3:  modoPoligonos = GL_POINT; break;
	}
	glutPostRedisplay();
}

void faceMode_menu_func(int opt) {
	switch (opt) {
	case 1:  modoFace = GL_FRONT; break;
	case 2:  modoFace = GL_BACK; break;
	case 3:  modoFace = GL_FRONT_AND_BACK; break;
	}
	glutPostRedisplay();
}

void camSpeed_menu_func(int opt) {
	switch (opt) {
	case 1:  cameraSpeed = 1.0; break;
	case 2:  cameraSpeed = 3.0; break;
	case 3:  cameraSpeed = 6.0; break;
	case 4:  cameraSpeed = 9.0; break;
	case 5:  cameraSpeed = 12.0; break;
	}
}

void bgColor_menu_func(int opt) {
	switch (opt) {
		case 1:  bg_red = 0; bg_green = 0; bg_blue = 0; break;
		case 2:  bg_red = 1; bg_green = 1; bg_blue = 1; break;
		case 3:  bg_red = 1; bg_green = 0; bg_blue = 0; break;
		case 4:  bg_red = 0; bg_green = 1; bg_blue = 0; break;
		case 5:  bg_red = 0; bg_green = 0; bg_blue = 1; break;
	}
	glutPostRedisplay();
}

void ptColor_menu_func(int opt) {
	switch (opt) {
		case 1:  pt_red = 0; pt_green = 0; pt_blue = 0; break;
		case 2:  pt_red = 1; pt_green = 1; pt_blue = 1; break;
		case 3:  pt_red = 1; pt_green = 0; pt_blue = 0; break;
		case 4:  pt_red = 0; pt_green = 1; pt_blue = 0; break;
		case 5:  pt_red = 0; pt_green = 0; pt_blue = 1; break;
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
	glClearColor(bg_red, bg_green, bg_blue,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camara.cCartesianas.x,camara.cCartesianas.y,camara.cCartesianas.z,
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	// put the geometric transformations here

	glColor3f(pt_red, pt_green, pt_blue);
	glBegin(GL_TRIANGLES);
	for (auto it = pontos.begin(); it != pontos.end(); ++it) {
		glVertex3f(it->x, it->y, it->z);
	}
	glEnd();

	// End of frame
	glutSwapBuffers();
}

void mouse_motion_func(int x, int y) {
	int deltaX = x - mouse_x;
	int deltaY = y - mouse_y;
	float cf = 0.009;

	if (deltaX > 0) camara.paraEsquerda(deltaX * cf * cameraSpeed * M_PI / 360.0);
	else camara.paraDireita(abs(deltaX) * cf * cameraSpeed * M_PI / 360.0);

	if(deltaY >0) camara.paraCima(deltaY * cf * cameraSpeed * M_PI / 360.0);
	else camara.paraBaixo(abs(deltaY) * cf * cameraSpeed * M_PI / 360.0);
	
}

void mouse_events_func(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			mouse_x = x;
			mouse_y = y;
		}
		break;
	case GLUT_MIDDLE_BUTTON: 
		break;
	case GLUT_RIGHT_BUTTON: 
		break;
	}
}

void teclas_normais_func(unsigned char key, int x, int y) {
	switch (tolower(key)) {
	case 'w': camara.paraCima(cameraSpeed * M_PI / 360.0);  break;
	case 's': camara.paraBaixo(cameraSpeed * M_PI / 360.0); break;
	case 'a': camara.paraEsquerda(cameraSpeed * M_PI / 360.0);  break;
	case 'd': camara.paraDireita(cameraSpeed * M_PI / 360.0); break;
	case 'e': camara.aproximar(0.5); break;
	case 'q': camara.afastar(0.5); break;
	case 'j': --cameraSpeed; break;
	case 'k': cameraSpeed=6; break;
	case 'l': ++cameraSpeed; break;
	}

	glutPostRedisplay();
}

void teclas_especiais_func(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_F1: break;
		case GLUT_KEY_F2: break;
		case GLUT_KEY_F3: break;
		case GLUT_KEY_F4: break;
		case GLUT_KEY_F5: break;
		case GLUT_KEY_F6: break;
		case GLUT_KEY_F7: break;
		case GLUT_KEY_F8: break;
		case GLUT_KEY_F9: break;
		case GLUT_KEY_F10: break;
		case GLUT_KEY_F11: break;
		case GLUT_KEY_F12: break;
		case GLUT_KEY_LEFT:
			camara.paraEsquerda(cameraSpeed * M_PI / 360.0);
			break;
		case GLUT_KEY_UP:
			camara.paraCima(cameraSpeed * M_PI / 360.0);
			break;
		case GLUT_KEY_RIGHT:
			camara.paraDireita(cameraSpeed * M_PI / 360.0);
			break;
		case GLUT_KEY_DOWN:
			camara.paraBaixo(cameraSpeed * M_PI / 360.0);
			break;
		case GLUT_KEY_PAGE_UP: break;
		case GLUT_KEY_PAGE_DOWN: break;
		case GLUT_KEY_HOME: break;
		case GLUT_KEY_END: break;
		case GLUT_KEY_INSERT: break;
	}
	
}

void leXML() {
	float x, y, z;
	
	std::string ficheiro(modelo_prefix + "scene.xml");
	pugi::xml_parse_result result = doc.load_file(ficheiro.c_str());
	
	if (!result) {
		std::cout << "Erro no parsing do ficheiro." << std::endl;
		return;
	}
}



void criaMenus() {
	
	int polMode_menu = glutCreateMenu(polMode_menu_func);
	glutAddMenuEntry("Fill", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Point", 3);
	int camSpeed_menu = glutCreateMenu(camSpeed_menu_func);
	glutAddMenuEntry("Very Slow", 1);
	glutAddMenuEntry("Slow", 2);
	glutAddMenuEntry("Normal", 3);
	glutAddMenuEntry("Fast", 4);
	glutAddMenuEntry("Very Fast", 5);
	int bgColor_menu = glutCreateMenu(bgColor_menu_func);
	glutAddMenuEntry("Black", 1);
	glutAddMenuEntry("White", 2);
	glutAddMenuEntry("Red", 3);
	glutAddMenuEntry("Green", 4);
	glutAddMenuEntry("Blue", 5);
	int ptColor_menu = glutCreateMenu(ptColor_menu_func);
	glutAddMenuEntry("Black", 1);
	glutAddMenuEntry("White", 2);
	glutAddMenuEntry("Red", 3);
	glutAddMenuEntry("Green", 4);
	glutAddMenuEntry("Blue", 5);

	int main_menu = glutCreateMenu(main_menu_func);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutAddSubMenu("Polygon Mode", polMode_menu);
	glutAddSubMenu("Camera Speed", camSpeed_menu);
	glutAddSubMenu("Background Color", bgColor_menu);
	glutAddSubMenu("Points Color", ptColor_menu);
	glutAddMenuEntry("Exit", 1);
}

int main(int argc, char **argv) {
	leXML();
	std::cout << "Numero pontos lidos:" << pontos.size() << std::endl;
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
	glutKeyboardFunc(teclas_normais_func);
	glutSpecialFunc(teclas_especiais_func);
	glutMouseFunc(mouse_events_func);
	glutMotionFunc(mouse_motion_func);

	criaMenus();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
