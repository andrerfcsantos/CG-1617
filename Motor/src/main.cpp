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
#include "tree.hh"
#include "../../utils/Transformacoes.h"
#include <deque>
#include <string>     // std::string, std::stof


using namespace std;
using namespace pugi;
vector<pugi::xml_node> stack;
vector<Ponto3D> pontos;
CoordsEsfericas camara = CoordsEsfericas(5.0, 0.0, M_PI / 3.0f);
GLenum modoPoligonos = GL_LINE;
GLenum modoFace = GL_FRONT;
float cameraSpeed = 6.0f;
float bg_red =0.0, bg_green =0.0, bg_blue = 0.0;
float pt_red = 1.0, pt_green = 1.0, pt_blue = 1.0;
int mouse_x, mouse_y;
pugi::xml_document doc;
tree<Grupo> arvoreG;
deque<tree<Grupo>::iterator> stack_group;
deque<tree<Grupo>::iterator> stack_group_read;
deque<xml_node> stack_xmlnode_read;
std::string modelo_prefix("../../Modelos/");
int p = 0;


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

void desenhaGrupo(tree<Grupo>::iterator it_grupo) {

	
	glPushMatrix();
	Translacao tr;
	Rotacao rr;
	Escala er;
	for (auto it = (*it_grupo).transformacoes.begin(); it != (*it_grupo).transformacoes.end(); ++it) {
        switch (it->tipo) {

            case TRANSLACAO:
				tr = it->Tr.t;
                glTranslatef(tr.tx, tr.ty, tr.tz);
                break;
            case ROTACAO:
				rr = it->Tr.r;
                glRotatef(rr.rang, rr.rx, rr.ry, rr.rz);
                break;
            case ESCALA:
				er = it->Tr.e;
                glScalef(er.sx, er.sy, er.sz);
                break;
            default:
                break;
        }

    }
    glBegin(GL_TRIANGLES);
	//glTranslatef(2.0f, 6.0f, 1.0f);
	for (auto it = (*it_grupo).pontos.begin(); it != (*it_grupo).pontos.end(); ++it) {
		glVertex3f(it->x, it->y, it->z);
	}
    glEnd();

	for (auto chld_it = arvoreG.child(it_grupo, 0); chld_it != chld_it.end(); ++chld_it) {
		desenhaGrupo(chld_it);
	}

	glPopMatrix();

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
	
	tree<Grupo>::iterator head = arvoreG.begin();
	desenhaGrupo(head);
	

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

void percorreArvore() {
	for (auto it = arvoreG.begin(); it != arvoreG.end(); ++it) {
		std::cout << *it << std::endl;
	}
}

Grupo XMLtoGrupo(xml_node node) {
	Grupo res;
	for (auto it = node.begin(); it != node.end(); ++it) {
		string node_name = it->name();

		if (node_name == "translate") {
			Transformacao trans(TRANSLACAO);
			trans.Tr.t.tx = trans.Tr.t.ty = trans.Tr.t.tz = 0;
			for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
			{
				string name = ait->name();
				float fl = stof(ait->value());
				if (name == "X") trans.Tr.t.tx = fl;
				if (name == "Y") trans.Tr.t.ty = fl;
				if (name == "Z") trans.Tr.t.tz = fl;
			}
			res.transformacoes.push_back(trans);
		}
		if (node_name == "rotate") {
			Transformacao trans(ROTACAO);
			trans.Tr.r.ry = 1;
			trans.Tr.r.rang = trans.Tr.r.rx = trans.Tr.r.rz =0 ;
			trans.tipo = ROTACAO;
			for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
			{
				string name = ait->name();
				float fl = stof(ait->value());
				if (name == "angle") trans.Tr.r.rang = fl;
				if (name == "axisX") trans.Tr.r.rx = fl;
				if (name == "axisY") trans.Tr.r.ry = fl;
				if (name == "axisZ") trans.Tr.r.rz = fl;
			}
			res.transformacoes.push_back(trans);
		}
		if (node_name == "scale") {
			Transformacao trans(ESCALA);
			trans.Tr.e.sx = trans.Tr.e.sy = trans.Tr.e.sz = 1;
			trans.tipo = ESCALA;
			for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
			{
				string name = ait->name();
				float fl = stof(ait->value());
				if (name == "X") trans.Tr.e.sx = fl;
				if (name == "Y") trans.Tr.e.sy = fl;
				if (name == "Z") trans.Tr.e.sz = fl;
			}
			res.transformacoes.push_back(trans);
		}

		if (node_name == "models") {
			
			for (pugi::xml_node nfile : it->children("model"))
			{
				float x, y, z;
				string nome_ficheiro = nfile.attribute("file").value();
				ifstream fich_inp(modelo_prefix + nome_ficheiro);
				res.ficheiros.push_back(nome_ficheiro);

				while (fich_inp >> x >> y >> z) {
					res.pontos.push_back(Ponto3D{ x,y,z });
				}
			}
		}
	}
	return res;
}

void leXML() {
	float x, y, z;
	std::string nomeFicheiro("sistema_solar.xml");

	std::string ficheiro(modelo_prefix + nomeFicheiro);
	pugi::xml_parse_result result = doc.load_file(ficheiro.c_str());
	
	if (!result) {
		modelo_prefix = "../Modelos/";
		ficheiro = modelo_prefix + nomeFicheiro;
		result = doc.load_file(ficheiro.c_str());
		if (!result) {
			std::cout << "Erro no parsing do ficheiro." << std::endl;
			return;
		}
	}

	Grupo g;
	tree<Grupo>::iterator na, na_aux;
	xml_node nx;
	xml_node root_group = doc.child("scene").child("group");

	g = XMLtoGrupo(root_group);
	na = arvoreG.set_head(g);
	stack_group_read.push_back(na);
	stack_xmlnode_read.push_back(root_group);

	while (!stack_group_read.empty()) {
		nx = stack_xmlnode_read.back();
		stack_xmlnode_read.pop_back();
		
		na = stack_group_read.back();
		stack_group_read.pop_back();
		for (pugi::xml_node xml_child : nx.children("group")) {
			g = XMLtoGrupo(xml_child);
			na_aux= arvoreG.append_child(na, g);
			stack_group_read.push_back(na_aux);
			stack_xmlnode_read.push_back(xml_child);
		}
		
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
	percorreArvore();
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
