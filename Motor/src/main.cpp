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
#include "../../utils/Figura.h"
#include "pugixml.hpp"

using namespace std;

vector<Ponto3D> pontos_originais;
vector<Ponto3D> pontos;
CoordsEsfericas camara = CoordsEsfericas(10.0, 0.0, M_PI / 3.0f);
GLenum modoPoligonos = GL_LINE;
GLenum modoFace = GL_FRONT;
float cameraSpeed = 6.0f;
float bg_red =0.0, bg_green =0.0, bg_blue = 0.0;
float pt_red = 1.0, pt_green = 1.0, pt_blue = 1.0;
int mouse_x, mouse_y;

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
}

void ptColor_menu_func(int opt) {
	switch (opt) {
		case 1:  pt_red = 0; pt_green = 0; pt_blue = 0; break;
		case 2:  pt_red = 1; pt_green = 1; pt_blue = 1; break;
		case 3:  pt_red = 1; pt_green = 0; pt_blue = 0; break;
		case 4:  pt_red = 0; pt_green = 1; pt_blue = 0; break;
		case 5:  pt_red = 0; pt_green = 0; pt_blue = 1; break;
	}
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

	glPolygonMode(modoFace, modoPoligonos);

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
	Figura fig;
	switch (key) {
		case GLUT_KEY_F1:
			//geraPlanoY(Ponto3D o, float comp, float larg, int divsx, int divsz, int orientacao);
			pontos = fig.geraPlanoY(Ponto3D{0,0,0}, 4,3,5,2,1).getPontos();
			break;
		case GLUT_KEY_F2:
			//Figura& geraCirculo(Ponto3D o, float raio, int fatias, int orientacao)
			pontos = fig.geraCirculo(Ponto3D{ 0,0,0 }, 3,25,1).getPontos();
			break;
		case GLUT_KEY_F3:
			//Figura& geraCaixa(Ponto3D o,float comprimento, float largura, float altura, int divsx, int divsz, int divsy)
			pontos = fig.geraCaixa(Ponto3D{ 0,0,0 }, 4, 3, 2, 2, 1, 5).getPontos();
			break;
		case GLUT_KEY_F4:
			//Figura& geraCilindro(Ponto3D o, float raio, float altura, int fatias, int camadas)
			pontos = fig.geraCilindro(Ponto3D{ 0,0,0 }, 2, 4, 25, 25).getPontos();
			break;
		case GLUT_KEY_F5:
			//Figura& geraCone(Ponto3D o, float raio, int altura, int fatias, int camadas)
			pontos = fig.geraCone(Ponto3D{ 0,0,0 }, 2,5,30,30).getPontos();
			break;
		case GLUT_KEY_F6:
			//Figura& geraEsfera(Ponto3D &o,float raio, int fatias, int camadas)
			pontos = fig.geraEsfera(Ponto3D{ 0,0,0 }, 3,50,50).getPontos();
			break;
		case GLUT_KEY_F7:
			// Figura& geraTorus(Ponto3D o, float R, float r, int fatias, int camadas)
			pontos = fig.geraTorus(Ponto3D{ 0,0,0 }, 5, 1, 25, 25).getPontos();
			break;
		case GLUT_KEY_F8:
			// Figura& geraElipsoide(Ponto3D o, float a, float b, float c, int fatias, int camadas)
			pontos = fig.geraElipsoide(Ponto3D{ 0,0,0 }, 1, 2, 3, 25,25).getPontos();
			break;
		case GLUT_KEY_F9:
			//Figura& geraFitaMobius(Ponto3D o, float R, float largura, float divsl, float divscomp)
			pontos = fig.geraFitaMobius(Ponto3D{ 0,0,0 }, 3, 2, 10, 50).getPontos();
			break;
		case GLUT_KEY_F10:
			//Figura& geraSeashell(Ponto3D o, float p_a, float p_b, float p_c, int n, float divsU, float divsV)
			pontos = fig.geraSeashell(Ponto3D{ 0,0,0 }, 0.5,3,1,3,50,100).getPontos();
			break;
		case GLUT_KEY_F11:
			break;
		case GLUT_KEY_F12:
			pontos = pontos_originais; break;
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
		while (fich_inp >> x >> y >> z) {
			pontos_originais.push_back(Ponto3D{ x,y,z });
		}
	}
	pontos = pontos_originais;
}

void criaMenus() {
	
	int polMode_menu = glutCreateMenu(polMode_menu_func);
	glutAddMenuEntry("Fill", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Point", 3);
	int faceMode_menu = glutCreateMenu(faceMode_menu_func);
	glutAddMenuEntry("Front", 1);
	glutAddMenuEntry("Back", 2);
	glutAddMenuEntry("Front and Back", 3);
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
	glutAddSubMenu("Face Mode", faceMode_menu);
	glutAddSubMenu("Camera Speed", camSpeed_menu);
	glutAddSubMenu("Background Color", bgColor_menu);
	glutAddSubMenu("Points Color", ptColor_menu);
	glutAddMenuEntry("Exit", 1);
}

int main(int argc, char **argv) {
	leXML();
	std::cout << "Numero pontos lidos:" << pontos_originais.size() << std::endl;
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
