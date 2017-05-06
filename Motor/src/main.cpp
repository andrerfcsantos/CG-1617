#include <stdlib.h>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "../../utils/CoordsEsfericas.h"
#include "../../utils/Camara.h"
#include "../../utils/Grupo.h"
#include "../../utils/Light.h"
#include "pugixml.hpp"
#include "tree.hh"
#include "../../utils/Transformacoes.h"
#include "../../utils/DefsDesenho.h"
#include "../../utils/Desenho.h"
#include "../../utils/CatmullRom.h"
#include <utility>
#include <deque>
#include <string>     // std::string, std::stof


using namespace std;
using namespace pugi;
vector<pugi::xml_node> stack;
vector<Coordenadas3D> pontos;
Camara camara = Camara(Coordenadas3D{ 7.5,5,7 }, M_PI, M_PI / 2 + M_PI / 6);
//Camara camara = Camara(Coordenadas3D{-8,0,0},M_PI/2, M_PI/2+0.01);
//Camara camara = Camara(Coordenadas3D{0,2,4},0, M_PI/2);
GLenum modoPoligonos = GL_LINE;
GLenum modoFace = GL_FRONT;
float cameraSpeed = 10.0f;
float bg_red =0.0, bg_green =0.0, bg_blue = 0.0;
float pt_red = 1.0, pt_green = 1.0, pt_blue = 1.0;
bool cor_desactivada = false, polygon_desactivado=false;
bool modoImediato = false;
int mouse_x, mouse_y;
pugi::xml_document doc;
tree<Grupo> arvoreG;
deque<tree<Grupo>::iterator> stack_group;
deque<tree<Grupo>::iterator> stack_group_read;
std::vector<Light> luzes;
deque<xml_node> stack_xmlnode_read;
std::string modelo_prefix("../../Modelos/");
int p = 0;
float up[3] = { 0,1,0 };

// FPS
int tempo_inicial;
float fps;
char fps_str[25];
int time_fps = 0, timebase = 0, frame = 0;
int npontos;

void renderCatmullRomCurve(std::vector<Coordenadas3D> p) {
	int c_points = 100;
	float dt = 1.0f / c_points;
	float res[3], deriv[3];
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < c_points; i++) {
		getGlobalCatmullRomPoint(p, dt*i, res, deriv);
		glVertex3f(res[0], res[1], res[2]);
	}
	glEnd();
}

void main_menu_func(int opt) {
	switch (opt) {
	case 1:  exit(0); break;
	}
}

void polMode_menu_func(int opt) {
	switch (opt) {
	case 1:  modoPoligonos = GL_FILL; polygon_desactivado = true; break;
	case 2:  modoPoligonos = GL_LINE; polygon_desactivado = true; break;
	case 3:  modoPoligonos = GL_POINT; polygon_desactivado = true; break;
	case 4:  polygon_desactivado = false; break;
	}
	glutPostRedisplay();
}

void vertexMode_menu_func(int opt) {
	switch (opt) {
	case 1:  modoImediato = true; break;
	case 2:  modoImediato = false; break;
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
	case 1:  cameraSpeed = 3.0; break;
	case 2:  cameraSpeed = 6.0; break;
	case 3:  cameraSpeed = 10.0; break;
	case 4:  cameraSpeed = 15.0; break;
	case 5:  cameraSpeed = 30.0; break;
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
	case 1:  pt_red = 0; pt_green = 0; pt_blue = 0; cor_desactivada = true; break;
		case 2:  pt_red = 1; pt_green = 1; pt_blue = 1; cor_desactivada = true; break;
		case 3:  pt_red = 1; pt_green = 0; pt_blue = 0; cor_desactivada = true; break;
		case 4:  pt_red = 0; pt_green = 1; pt_blue = 0; cor_desactivada = true; break;
		case 5:  pt_red = 0; pt_green = 0; pt_blue = 1; cor_desactivada = true; break;
		case 6: cor_desactivada = false; break;
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
	bool temRotacao = false;
	glPushMatrix();
	Translacao tr;
	Rotacao rr;
	Escala er;

	for (auto it = (*it_grupo).transformacoes.begin(); it != (*it_grupo).transformacoes.end(); ++it) {

        switch (it->tipo) {

            case TRANSLACAO:
				tr = it->Tr.t;
				if (tr.time == -1) {
					glTranslatef(tr.tx, tr.ty, tr.tz);
				}
				else {
					for (auto it = it_grupo->catmullDes.begin(); it != it_grupo->catmullDes.end(); ++it) {
						DefsDesenho &def = it->defsDesenho;
						vector<Coordenadas3D> &pts = it->pontos;

						if (polygon_desactivado == false) glPolygonMode(GL_FRONT, def.modoPoligonos);
						else glPolygonMode(GL_FRONT, modoPoligonos);

						if (cor_desactivada == false) glColor3f(def.red, def.green, def.blue);
						else glColor3f(pt_red, pt_green, pt_blue);


						if (modoImediato) {
							glBegin(it->defsDesenho.modoDesenho);
							for (auto it_pts = pts.begin(); it_pts != pts.end(); ++it_pts) {
								glVertex3f(it_pts->x, it_pts->y, it_pts->z);
							}
							glEnd();
						}
						else {
							glBindBuffer(GL_ARRAY_BUFFER, it->nBuffPontos);
							glVertexPointer(3, GL_FLOAT, 0, 0);
							glDrawArrays(it->defsDesenho.modoDesenho, 0, it->pontos.size());
						}
					}

					float t;
					float res[3], deriv[3];
					float X[3], Z[3];
					float m[16];
					t = (float) fmodf(((glutGet(GLUT_ELAPSED_TIME) - tempo_inicial)/1000.0f), tr.time) / (tr.time+0.0f);
					if (tr.ccw == false) {
						t = 1 - t;
					}
					// X = norm(deriv)
					getGlobalCatmullRomPoint(tr.ctrlPoints,t, res, X);
					normalize(X);
					// Z = norm(deriv x up)
					cross(X, up, Z);
					normalize(Z);
					// up = norm(deriv x Z)
					cross(Z, X, up);
					normalize(up);
					// buildRotMatrix()
					buildRotMatrix(X, up, Z, m);
					glTranslatef(res[0], res[1], res[2]);
					glMultMatrixf(m);

				}
                break;
            case ROTACAO:

				rr = it->Tr.r;
				if (rr.time == -1) {
					glRotatef(rr.rang, rr.rx, rr.ry, rr.rz);
				}
				else {
					float t = (float)fmodf(((glutGet(GLUT_ELAPSED_TIME) - tempo_inicial)/ 1000.0f), rr.time) / (rr.time + 0.0f);
					if (rr.ccw == false) {
						t = 1 - t;
					}
					float ang = (float) (360.0f * t);
					glRotatef(ang, rr.rx, rr.ry, rr.rz);
				}

                break;
            case ESCALA:
				er = it->Tr.e;
                glScalef(er.sx, er.sy, er.sz);
                break;
            default:
                break;
        }

    }
    
	

	for (auto it = it_grupo->desenhos.begin(); it != it_grupo->desenhos.end(); ++it) {
		DefsDesenho &def = it->defsDesenho;
		vector<Coordenadas3D> &pts = it->pontos;

		if(polygon_desactivado == false) glPolygonMode(GL_FRONT, def.modoPoligonos);
		else glPolygonMode(GL_FRONT, modoPoligonos);
		
		if (cor_desactivada == false) glColor3f(def.red, def.green, def.blue);
		else glColor3f(pt_red, pt_green, pt_blue);
		

		if (modoImediato) {
			glBegin(it->defsDesenho.modoDesenho);
			for (auto it_pts = pts.begin(); it_pts != pts.end(); ++it_pts) {
				glVertex3f(it_pts->x, it_pts->y, it_pts->z);
			}
			glEnd();
		}
		else {
			glBindBuffer(GL_ARRAY_BUFFER, it->nBuffPontos);
			glVertexPointer(3, GL_FLOAT, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, it->nBuffNormal);
			glNormalPointer(GL_FLOAT, 0, 0);

			glDrawArrays(it->defsDesenho.modoDesenho, 0, it->pontos.size());
		}
		
	}

	for (auto chld_it = arvoreG.child(it_grupo, 0); chld_it != chld_it.end(); ++chld_it) {
		desenhaGrupo(chld_it);
	}

	glPopMatrix();

}

void calculaFPS() {
	frame++;
	time_fps = glutGet(GLUT_ELAPSED_TIME);
	if (time_fps - timebase > 1000) {
		fps = frame*1000.0 / (time_fps - timebase);
		timebase = time_fps;
		frame = 0;
		sprintf(fps_str, "%.2f fps", fps);
		glutSetWindowTitle(fps_str);
	}
}

void renderScene(void) {

	calculaFPS();

	glPolygonMode(GL_FRONT, modoPoligonos);

	// clear buffers
	glClearColor(bg_red, bg_green, bg_blue,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	Coordenadas3D la = camara.p + camara.v_d;
	gluLookAt(camara.p.x, camara.p.y, camara.p.z,
		      la.x,la.y,la.z,
			  0.0f,1.0f,0.0f);
	
	tree<Grupo>::iterator head = arvoreG.begin();
	desenhaGrupo(head);

	glutSwapBuffers();
}

void mouse_motion_func(int x, int y) {
	int deltaX = x - mouse_x;
	int deltaY = y - mouse_y;
	float cf = 0.005;

	if (deltaX > 0) camara.lookDireita(deltaX * cf * cameraSpeed * M_PI / 360.0);
	else camara.lookEsquerda(abs(deltaX) * cf * cameraSpeed * M_PI / 360.0);

	if(deltaY >0) camara.lookBaixo(deltaY * cf * cameraSpeed * M_PI / 360.0);
	else camara.lookCima(abs(deltaY) * cf * cameraSpeed * M_PI / 360.0);
	
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
	case 'w': camara.frente(cameraSpeed * M_PI / 360.0);  break;
	case 's': camara.tras(cameraSpeed * M_PI / 360.0); break;
	case 'a': camara.esquerda(cameraSpeed * M_PI / 360.0);  break;
	case 'd': camara.direita(cameraSpeed * M_PI / 360.0); break;
	case 'e': camara.baixo(cameraSpeed * M_PI / 360.0); break;
	case 'q': camara.cima(cameraSpeed * M_PI / 360.0); break;
	case 'j': cameraSpeed-=3; break;
	case 'k': cameraSpeed=10; break;
	case 'l': cameraSpeed+=3; break;
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
			camara.lookEsquerda(cameraSpeed * M_PI / 360.0);
			break;
		case GLUT_KEY_UP:
			camara.lookCima(cameraSpeed * M_PI / 360.0);
			break;
		case GLUT_KEY_RIGHT:
			camara.lookDireita(cameraSpeed * M_PI / 360.0);
			break;
		case GLUT_KEY_DOWN:
			camara.lookBaixo(cameraSpeed * M_PI / 360.0);
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

void XMLtoLights(xml_node node) {
	for (auto it = node.begin(); it != node.end(); ++it) {
		Light l;
		for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait) {
			string name = ait->name();
			if (name == "type") {
				l.type = ait->value();
				if (l.type == "DIRECTIONAL") {
					l.pos[3] = 0.0;
				}
			}
			if (name == "posX") {
				l.pos[0] = stof(ait->value());
			}
			if (name == "posY") {
				l.pos[1] = stof(ait->value());
			}
			if (name == "posZ") {
				l.pos[2] = stof(ait->value());
			}
			if (name == "diffR") {
				l.diff[0] = stof(ait->value());
			}
			if (name == "diffG") {
				l.diff[1] = stof(ait->value());
			}
			if (name == "diffB") {
				l.diff[2] = stof(ait->value());
			}
			if (name == "ambR") {
				l.amb[0] = stof(ait->value());
			}
			if (name == "ambG") {
				l.amb[1] = stof(ait->value());
			}
			if (name == "ambB") {
				l.amb[2] = stof(ait->value());
			}
			if (name == "emisR") {
				l.emiss[0] = stof(ait->value());
			}
			if (name == "emisG") {
				l.emiss[1] = stof(ait->value());
			}
			if (name == "emisB") {
				l.emiss[2] = stof(ait->value());
			}
			if (name == "spotX") {
				l.spotDir[0] = stof(ait->value());
			}
			if (name == "spotY") {
				l.spotDir[1] = stof(ait->value());
			}
			if (name == "spotZ") {
				l.spotDir[2] = stof(ait->value());
			}
			if (name == "cutoff") {
				l.cutoff = stof(ait->value());
			}
			if (name == "exponent") {
				l.exponent = stof(ait->value());
			}
			if (name == "const_Att") {
				l.attenuation[0] = stof(ait->value());
			}
			if (name == "linear_Att") {
				l.attenuation[1] = stof(ait->value());
			}
			if (name == "quad_Att") {
				l.attenuation[2] = stof(ait->value());
			}
		}
		luzes.push_back(l);
	}
}

Grupo XMLtoGrupo(xml_node node) {
	Grupo res;
	GLuint buffer_pontos[1];
	GLuint buffer_normais[1];
	GLuint buffer_coords_textura[1];


	res.nome = node.attribute("name").value();

	for (auto it = node.begin(); it != node.end(); ++it) {
		string node_name = it->name();

		if (node_name == "translate") {
			Transformacao trans(TRANSLACAO);
			trans.Tr.t.tx = trans.Tr.t.ty = trans.Tr.t.tz = 0;
			trans.Tr.t.time = -1;

			for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
			{
				string name = ait->name();
				if (name == "time") {
					Desenho desenho;
					desenho.defsDesenho.red = 0.39215686274509803f;
					desenho.defsDesenho.green = 0.39215686274509803f;
					desenho.defsDesenho.blue = 0.39215686274509803f;
					desenho.defsDesenho.modoPoligonos = GL_LINE;

					trans.Tr.t.time = stof(ait->value());
					for (pugi::xml_node it_p : it->children("point")) {
						float x=0, y=0, z=0;
						Coordenadas3D ponto;
						ponto.x=stof(it_p.attribute("X").value());
						ponto.y=stof(it_p.attribute("Y").value());
						ponto.z=stof(it_p.attribute("Z").value());
						trans.Tr.t.ctrlPoints.push_back(ponto);
						
					}

					string str_direction = it->attribute("direction").value();

					trans.Tr.t.ccw = (str_direction == "cw") ? false : true ;

					string str_drawCatmullCurve = it->attribute("drawCatmullCurve").value();

					if (str_drawCatmullCurve == "true") {
						string str_npts = it->attribute("nrCatmullPointsToDraw").value();
						int c_points = (str_npts != "") ? stoi(str_npts): trans.Tr.t.ctrlPoints.size();
						float dt = 1.0f / c_points;
						float resultado[3], deriv[3];
						for (int i = 0; i < c_points; i++) {
							getGlobalCatmullRomPoint(trans.Tr.t.ctrlPoints, dt*i, resultado, deriv);
							desenho.pontos.push_back(Coordenadas3D{ resultado[0],resultado[1],resultado[2] });
						}

						glGenBuffers(1, buffer_pontos);
						glBindBuffer(GL_ARRAY_BUFFER, buffer_pontos[0]);
						glBufferData(GL_ARRAY_BUFFER, sizeof(float) * desenho.pontos.size() * 3, &desenho.pontos[0], GL_STATIC_DRAW);
						desenho.nBuffPontos = buffer_pontos[0];
						desenho.defsDesenho.modoDesenho = GL_LINE_LOOP;
						res.catmullDes.push_back(desenho);
					}

				}
				else {
					if (name == "X") trans.Tr.t.tx = stof(ait->value());
					if (name == "Y") trans.Tr.t.ty = stof(ait->value());
					if (name == "Z") trans.Tr.t.tz = stof(ait->value());
				}
			}
			res.transformacoes.push_back(trans);
		}
		if (node_name == "rotate") {
			Transformacao trans(ROTACAO);
			trans.Tr.r.ry = 0;
			trans.Tr.r.rang = trans.Tr.r.rx = trans.Tr.r.rz =0 ;
			trans.Tr.r.time = -1.0f;

			trans.tipo = ROTACAO;
			for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
			{
				string name = ait->name();
				if (name == "angle") trans.Tr.r.rang = stof(ait->value());
				if (name == "time") {
					trans.Tr.r.time = stof(ait->value());
					string str_direction = it->attribute("direction").value();
					trans.Tr.r.ccw = (str_direction == "cw") ? false : true;
				}
				if (name == "X") trans.Tr.r.rx = stof(ait->value());
				if (name == "Y") trans.Tr.r.ry = stof(ait->value());
				if (name == "Z") trans.Tr.r.rz = stof(ait->value());
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
				if (name == "uniform") {
					trans.Tr.e.sx = fl;
					trans.Tr.e.sy = fl;
					trans.Tr.e.sz = fl;
					break;
				}
				if (name == "X") trans.Tr.e.sx = fl;
				if (name == "Y") trans.Tr.e.sy = fl;
				if (name == "Z") trans.Tr.e.sz = fl;
			}
			res.transformacoes.push_back(trans);
		}

		if (node_name == "models") {
			for (pugi::xml_node nfile : it->children("model")){
				Desenho desenho;
				for (pugi::xml_attribute_iterator ait = nfile.attributes_begin(); ait != nfile.attributes_end(); ++ait) {
					string name = ait->name();
					float fl;
					if (name == "mode") {
						string mode = ait->value();
						if (mode == "FILL") desenho.defsDesenho.modoPoligonos= GL_FILL;
						if (mode == "LINE") desenho.defsDesenho.modoPoligonos = GL_LINE;
						if (mode == "POINT") desenho.defsDesenho.modoPoligonos = GL_POINT;
					}
					if (name == "red") {
						fl = stof(ait->value());
						desenho.defsDesenho.red = fl;
					}
					if (name == "green") {
						fl = stof(ait->value());
						desenho.defsDesenho.green = fl;
					}
					if (name == "blue") {
						fl = stof(ait->value());
						desenho.defsDesenho.blue = fl;
					}
				}

				float x, y, z,nx,ny,nz,t1,t2;
				string nome_ficheiro = nfile.attribute("file").value();
				ifstream fich_inp(modelo_prefix + nome_ficheiro);
				res.ficheiros.push_back(nome_ficheiro);

				while (fich_inp >> x >> y >> z >> nx >> ny >> nz >> t1 >> t2) {
					desenho.pontos.push_back(Coordenadas3D{ x,y,z });
					desenho.normais.push_back(Coordenadas3D{nx,ny,nz});
					desenho.coordsText.push_back(CoordsTextura{t1,t2});
					npontos++;
				}
				
				glGenBuffers(1, buffer_pontos);
				glBindBuffer(GL_ARRAY_BUFFER, buffer_pontos[0]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * desenho.pontos.size() * 3, &desenho.pontos[0], GL_STATIC_DRAW);
				desenho.nBuffPontos = buffer_pontos[0];
				desenho.defsDesenho.modoDesenho = GL_TRIANGLES;

				glGenBuffers(1, buffer_normais);
				glBindBuffer(GL_ARRAY_BUFFER, buffer_normais[0]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * desenho.pontos.size() * 3, &desenho.pontos[0], GL_STATIC_DRAW);
				desenho.nBuffNormal = buffer_normais[0];

				res.desenhos.push_back(desenho);
			}
			
			
		}
	}
	return res;
}

void leXML() {
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

	xml_node root_lights = doc.child("scene").child("lights");
	XMLtoLights(root_lights);

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
	glutAddMenuEntry("Original", 4);
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
	glutAddMenuEntry("Original", 6);
	int vertexMode_menu = glutCreateMenu(vertexMode_menu_func);
	glutAddMenuEntry("Immediate", 1);
	glutAddMenuEntry("VBO's", 2);

	int main_menu = glutCreateMenu(main_menu_func);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutAddSubMenu("Vertex Mode", vertexMode_menu);
	glutAddSubMenu("Polygon Mode", polMode_menu);
	glutAddSubMenu("Camera Speed", camSpeed_menu);
	glutAddSubMenu("Background Color", bgColor_menu);
	glutAddSubMenu("Points Color", ptColor_menu);
	glutAddMenuEntry("Exit", 1);
}

int main(int argc, char **argv) {

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(75,50);
	glutInitWindowSize(1250,700);
	glutCreateWindow("CG@DI-UM");

#ifndef __APPLE__
	glewInit();
#endif
		
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

	//glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	leXML();

	if (luzes.size() > 0) {
		glEnable(GL_LIGHTING);
		for (int i = 0; i < luzes.size(); ++i) {
			luzes[i].apply(i);
			glEnable(GL_LIGHT0 + i);
		}
	}

	std::cout << "npontos: " << npontos << std::endl;
	tempo_inicial = glutGet(GLUT_ELAPSED_TIME);

	glutMainLoop();
	
	return 1;
}
