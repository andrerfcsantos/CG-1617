#pragma once

using namespace std;

class Light {

public:
    string type;
	GLfloat pos[4] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat amb[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat diff[4] = {1.0, 1.0, 1.0, 1.0};
	GLfloat spec[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat attenuation[3] = { 1.0, 0.0, 0.0 };
    GLfloat spotDir[3] = {0.0, 0.0, -1.0};
    GLfloat spotCutoff = 180.0;
    GLfloat spotExponent = 0.0;

    Light() {
    }

    void apply(int light) {
        glLightfv(GL_LIGHT0 + light, GL_POSITION, pos);
        glLightfv(GL_LIGHT0 + light, GL_DIFFUSE, diff);
		glLightfv(GL_LIGHT0 + light, GL_AMBIENT, amb);
		glLightf(GL_LIGHT0 + light, GL_CONSTANT_ATTENUATION, attenuation[0]);
		glLightf(GL_LIGHT0 + light, GL_LINEAR_ATTENUATION, attenuation[1]);
		glLightf(GL_LIGHT0 + light, GL_QUADRATIC_ATTENUATION, attenuation[2]);
        if (type == "SPOTLIGHT") {
            glLightfv(GL_LIGHT0 + light, GL_SPOT_DIRECTION, spotDir);
            glLightf(GL_LIGHT0 + light, GL_SPOT_CUTOFF, spotCutoff);
            glLightf(GL_LIGHT0 + light, GL_SPOT_EXPONENT, spotExponent);
        }
    }
};