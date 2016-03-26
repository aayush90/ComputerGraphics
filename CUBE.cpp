// CUBE.cpp : Defines the entry point for the console application.
//
#include "CUBE.h"



// GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
//   {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
//   {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
// GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
//   {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
//   {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
// GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */

GLfloat **normal;
GLint **face;
GLfloat **vertex;
int c=0;

void init(void) 
{
	/*v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;*/


   GLfloat light_diffuse[] = {0.5, 1.0, 1.0, 1.0};  /* Red diffuse light. */
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
   glClearColor (0.5, 0.5, 0.5, 0.0);
   glShadeModel (GL_SMOOTH);

   GLfloat sd[] = {-1.0, -1.0, -1.0};
glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, sd);
glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40.0);
glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);

   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);

   glMatrixMode(GL_PROJECTION);
  gluPerspective( /* field of view in degree */ 50.0,
    /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */

  /* Adjust cube position to be asthetic angle. */
  glTranslatef(0.0, 0.0, -1.0);
  glRotatef(60, 1.0, 0.0, 0.0);
  glRotatef(-40, 0.0, 0.0, 1.0);
}

void
drawBox(void)
{

  for (int i = 0; i < 6*c; i++) {
    glBegin(GL_QUADS);
    glNormal3fv(&normal[i][0]);
    glVertex3fv(&vertex[face[i][0]][0]);
    glVertex3fv(&vertex[face[i][1]][0]);
    glVertex3fv(&vertex[face[i][2]][0]);
    glVertex3fv(&vertex[face[i][3]][0]);
    glEnd();
  }
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   drawBox();

  glutSwapBuffers();
   glFlush ();
       // output(80, 2800, "Welcome to movelight.");
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
         1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   else
      glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
         1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

/*int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   //glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}*/


void project(int argc, char** argv, GLfloat **v, GLfloat **n, GLint **faces, int cubes)
{   c=cubes;
   normal = new GLfloat*[6*cubes];
  face = new GLint*[6*cubes];
  vertex = new GLfloat*[8*cubes];

  for (int i = 0;i < 6*cubes;++i){
    normal[i] = new GLfloat[3];
    face[i] = new GLint[4];
  }
  for (int i = 0; i < 8*cubes; ++i){
    vertex[i] = new GLfloat[3];
  }

  for(int i=0;i<6*cubes;i++){
    normal[i][0] = n[i][0]; normal[i][1] = n[i][1]; normal[i][2] = n[i][2];
    face[i][0] = faces[i][0]; face[i][1] = faces[i][1]; face[i][2] = faces[i][2]; face[i][3] = faces[i][3];
  }
  for(int i=0;i<8*cubes;i++){
    vertex[i][0] = v[i][0]; vertex[i][1] = v[i][1]; vertex[i][2] = v[i][2];
  }



   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Testing OpenGL");
   init ();
   glutDisplayFunc(display); 
   //glutReshapeFunc(reshape);
   glutMainLoop();
}