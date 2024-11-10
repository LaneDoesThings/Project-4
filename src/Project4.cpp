/*
CS445/545 Prog 4 for Lane Wright

The fish are updated using the keyboard and the keyboard_CB function to update
thier global position with 'h', 'j', 'u', and 'n' respectively. The fan is
animated by incrementing the global rotation angle in timer_CB and uses ambient
lighting. The timer is drawn using a bitmap font and decremented in the timer_CB
function.

The display_CB function is used to draw the fish, fan, and tank as well as to
upadte the display.

The events that are used are a display event, timer event, and keyboard event
with their respective callbacks mentioned above.
*/

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup-aug24.h"
#include <iostream>
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
int countdown = 30;
bool stop = false;
int angle = 0;

// Function that uses a bitmap font to draw the countdown in the upper left
// corner
void drawTime() {
  glColor3f(1.0f, 1.0f, 1.0f);
  glRasterPos2i(275, 190);
  glutBitmapString(GLUT_BITMAP_8_BY_13,
                   (const unsigned char *)std::to_string(countdown).c_str());
}

// Draws the fish passed to it with the given color and gives it a yellow tail
// if tail is true
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

// Draws the blades of the fan as well as rotates them based on angle
void drawFan() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glPushMatrix();

  glRotatef(angle, 0.0, 0.0, 1.0);
  glBegin(GL_TRIANGLES);
  glVertex3f(0.0, 0.0, -300.0);
  glVertex3f(50.0 - 5.0, 0.0 + 5.0, -300.0);
  glVertex3f(25.0 + 5.0, 43.3 - 5.0, -300.0);

  glVertex3f(0.0, 0.0, -300.0);
  glVertex3f(25.0 - 5.0, 43.3, -300.0);
  glVertex3f(-25.0 + 5.0, 43.3, -300.0);

  glVertex3f(0.0, 0.0, -300.0);
  glVertex3f(-25.0 - 5.0, 43.3 - 5.0, -300.0);
  glVertex3f(-50 + 5.0, 0.0 + 5.0, -300.0);

  glVertex3f(0.0, 0.0, -300.0);
  glVertex3f(-50.0 + 5.0, 0.0 - 5.0, -300.0);
  glVertex3f(-25.0 - 5.0, -43.3 + 5.0, -300.0);

  glVertex3f(0.0, 0.0, -300.0);
  glVertex3f(-25.0 + 5.0, -43.3, -300.0);
  glVertex3f(25.0 - 5.0, -43.3, -300.0);

  glVertex3f(0.0, 0.0, -300.0);
  glVertex3f(25.0 + 5.0, -43.3 + 5.0, -300.0);
  glVertex3f(50.0 - 5.0, 0.0 - 5.0, -300.0);
  glEnd();
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glPopMatrix();
}

// Callback for display event that calls the functions that will draw all the
// objects in the scene
void display_CB() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  drawFan();
  glCallList(1);
  drawFish(fish1, {.r = 255.0, .g = 165.0, .b = 0.0}, true);
  drawFish(fish2, {.r = 105.0, .g = 105.0, .b = 105.0}, false);
  drawTime();

  glutSwapBuffers();
}

// Callback for timer event that handles the framerate the countdown timer and
// the timer for rotating the fans
void timer_CB(int id) {
  // Framerate
  if (id == 0) {
    glutTimerFunc(1000 / framerate, timer_CB, 0);
    glutPostRedisplay();
  }
  // Countdown
  if (id == 1) {
    if (countdown == 0) {
      stop = true;
      return;
    }
    countdown--;
    glutTimerFunc(1000, timer_CB, 1);
  }
  // Rotation of fans
  if (id == 2) {
    if (!stop) {
      angle = (angle + 10) % 360;
      glutTimerFunc(55, timer_CB, 2);
    }
  }
}

// Checks if a collision will happen in the given direction and returns the
// result
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
    return NULL; // If no direction was given return null to crash because
                 // something went very wrong
  }
}

// Callback for keyboard event that handles moving the fish
void keyboard_CB(unsigned char key, int x, int y) {
  if (!stop) {
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
}

int main(int argc, char *argv[]) {
  char canvas_Name[] = "Project 4 - Lane Wright";
  glutInit(&argc, argv);
  my_setup(canvas_Width, canvas_Height, canvas_Name);
  gluLookAt(200.0, 0.0, 500.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glGenLists(1);

  // Lighting setup
  float lightpos[3] = {0.0, 0.0, 0.0};
  float lightcolor[4] = {0.5, 0.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightcolor);

  float materialcolor[4] = {1.0, 1.0, 1.0, 1.0};
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialcolor);

  // Fish Tank
  glNewList(1, GL_COMPILE);
  glPushMatrix();
  glColor3f(0.0, 1.0, 1.0);
  glTranslatef(0.0, 0.0, -175.0);
  glutWireCube(tankSize);
  glPopMatrix();
  glEndList();

  // Event setup
  glutDisplayFunc(display_CB);
  glutTimerFunc(1000 / framerate, timer_CB, 0);
  glutTimerFunc(1000, timer_CB, 1);
  glutTimerFunc(55, timer_CB, 2);
  glutKeyboardFunc(keyboard_CB);

  glutMainLoop();
  return 0;
}