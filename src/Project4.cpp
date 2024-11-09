#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup-aug24.h"
#include <iostream>
#include <math.h>
#include <string>

#define canvas_Width 800
#define canvas_Height 600
#define framerate 60

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Color {
  float r;
  float g;
  float b;
};

struct Fish {
  float x;
  float y;
  float z;
  float width;
  float height;
  float depth;
};

Fish fish1 = {.x = 0.0,
              .y = 0.0,
              .z = -175.0,
              .width = 75.0 / 2.0,
              .height = 30.0 / 2.0,
              .depth = 25.0 / 2.0};
Fish fish2 = {.x = 0.0,
              .y = -50.0,
              .z = -175.0,
              .width = 50.0 / 2.0,
              .height = 25.0 / 2.0,
              .depth = 20.0 / 2.0};

float tankSize = 250.0;

void drawFish(Fish &fish, Color color, bool tail) {
  glPushMatrix();
  glColor3ub(color.r, color.g, color.b);
  glTranslatef(fish.x, fish.y, fish.z);
  glScalef(fish.width, fish.height, fish.depth);
  glutWireOctahedron();
  glPopMatrix();
  glBegin(GL_LINES);
  glVertex3f(fish.x + fish.width, fish.y, fish.z);
  glVertex3f(fish.x + fish.width + 15.0, fish.y - 7.5, fish.z);
  glVertex3f(fish.x + fish.width + 15.0, fish.y - 7.5, fish.z);
  glVertex3f(fish.x + fish.width + 15.0, fish.y + 7.5, fish.z);
  glVertex3f(fish.x + fish.width + 15.0, fish.y + 7.5, fish.z);
  glVertex3f(fish.x + fish.width, fish.y, fish.z);
  glEnd();
  if (tail) {
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(fish.x + fish.width + 2.5, fish.y, fish.z);
    glVertex3f(fish.x + fish.width + 12.5, fish.y, fish.z);
    glEnd();
  }
}

void display_CB() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glCallList(1);
  drawFish(fish1, {.r = 255.0, .g = 165.0, .b = 0.0}, true);
  drawFish(fish2, {.r = 105.0, .g = 105.0, .b = 105.0}, false);

  glutSwapBuffers();
}

void timer_CB(int id) {
  if (id == 0) {
    glutTimerFunc(1000 / framerate, timer_CB, 0);
    glutPostRedisplay();
  }
}

bool collide(Direction dir) {
  switch (dir) {
  case UP:
    return (fish2.y + fish2.height < fish1.y - fish1.height) &&
           (fish2.y + fish2.height + 10.0 >= fish1.y - fish1.height) &&
           (fish2.x + fish2.width + 15.0 > fish1.x - fish1.width) &&
           (fish2.x - fish2.width < fish1.x + fish1.width + 15.0);
  case DOWN:
    return (fish2.y - fish2.height > fish1.y + fish1.height) &&
           (fish2.y - fish2.height - 10.0 <= fish1.y + fish1.height) &&
           (fish2.x + fish2.width + 15.0 > fish1.x - fish1.width) &&
           (fish2.x - fish2.width < fish1.x + fish1.width + 15.0);
  case LEFT:
    return (fish1.x - fish1.width > fish2.x + fish2.width + 15.0) &&
           (fish1.x - fish1.width - 10.0 <= fish2.x + fish2.width + 15.0) &&
           (fish1.y + fish1.height + 15.0 > fish2.y - fish2.height) &&
           (fish1.y - fish1.height < fish2.y + fish2.height + 15.0);
  case RIGHT:
    return (fish1.x + fish1.width + 15.0 < fish2.x - fish2.width) &&
           (fish1.x + fish1.width + 10.0 + 15.0 >= fish2.x - fish2.width) &&
           (fish1.y + fish1.height + 15.0 > fish2.y - fish2.height) &&
           (fish1.y - fish1.height < fish2.y + fish2.height + 15.0);
  default:
    return NULL;
  }
}

void keyboard_CB(unsigned char key, int x, int y) {
  if (key == 'h') {
    if ((fish1.x - fish1.width) - 10.0 > (-tankSize / 2.0f) && !collide(LEFT))
      fish1.x -= 10.0;
  } else if (key == 'j') {
    if ((fish1.x + fish1.width) + 10.0 < (tankSize / 2.0f) && !collide(RIGHT))
      fish1.x += 10.0;
  } else if (key == 'u') {
    if ((fish2.y + fish2.height) + 10.0 < (tankSize / 2.0f) && !collide(UP))
      fish2.y += 10.0;
  } else if (key == 'n') {
    if ((fish2.y - fish2.height) - 10.0 > (-tankSize / 2.0f) && !collide(DOWN))
      fish2.y -= 10.0;
  }
}

int main(int argc, char *argv[]) {
  char canvas_Name[] = "Project 4 - Lane Wright";
  glutInit(&argc, argv);
  my_setup(canvas_Width, canvas_Height, canvas_Name);
  gluLookAt(200.0, 0.0, 500.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glGenLists(5);

  // Fish Tank
  glNewList(1, GL_COMPILE);
  glPushMatrix();
  glColor3f(0.0, 1.0, 1.0);
  glTranslatef(0.0, 0.0, -175.0);
  glutWireCube(tankSize);
  glPopMatrix();
  glEndList();

  glutDisplayFunc(display_CB);
  glutTimerFunc(1000 / framerate, timer_CB, 0);
  glutKeyboardFunc(keyboard_CB);

  glutMainLoop();
  return 0;
}