#include "CSCI4229.h"

/*
 *  Scene of 3D object
 *
 *  Demonstrates how to draw objects in 3D.
 *
 *  Key bindings:
 *  a          Toggle axes
 *  arrows     Change view angle
 *  0          Reset view angle
 *  ESC        Exit
 */


 int th=0;         //  Azimuth of view angle
 int ph=0;         //  Elevation of view angle
 //double zh=0;      //  Rotation of teapot
 int axes=1;       //  Display axes
 int mode=1;       //  What to display

 int move=1;       //  Move light
 int fov=55;       //  Field of view (for perspective)
 int light=1;      //  Lighting
 double asp=1;     //  Aspect ratio
 double dim=4.0;   //  Size of world
 // Light values
 int one       =   1;  // Unit value
 int distance  =   5;  // Light distance
 int inc       =  10;  // Ball increment
 int smooth    =   1;  // Smooth/Flat shading
 int local     =   0;  // Local Viewer Model
 int emission  =   0;  // Emission intensity (%)
 int ambient   =  30;  // Ambient intensity (%)
 int diffuse   = 100;  // Diffuse intensity (%)
 int specular  =   0;  // Specular intensity (%)
 int shininess =   0;  // Shininess (power of two)
 float shiny   =   1;  // Shininess (value)
 int zh        =  90;  // Light azimuth
 float ylight  =   0;  // Elevation of light
 unsigned int texture[4]; // Texture Names

 /*
  *  Convenience routine to output raster text
  *  Use VARARGS to make this more flexible
  */
 #define LEN 8192  //  Maximum length of text string
 void Print(const char* format , ...)
 {
    char    buf[LEN];
    char*   ch=buf;
    va_list args;
    //  Turn the parameters into a character string
    va_start(args,format);
    vsnprintf(buf,LEN,format,args);
    va_end(args);
    //  Display the characters one at a time at the current raster position
    while (*ch)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
 }

 /*
  *  Draw a house
  *     at (x,y,z)
  *     dimensions (dx,dy,dz)
  *     rotated th about the y axis
  */
 static void house(double x,double y,double z,
                  double dx,double dy,double dz,
                  double th)
 {
   // Dimensions for house
   double wd = 1;
   double ht = 1;

   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   // Draws the front of the house which are polygons
   glBegin(GL_POLYGON);
   //  Front
   glColor3f(1,0,0);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-wd,-ht, 1);
   glTexCoord2f(1,0); glVertex3f(wd,-ht, 1);
   glTexCoord2f(1,1); glVertex3f(wd,ht, 1);
   glTexCoord2f(0.5,1); glVertex3f(0,+2, 1);
   glTexCoord2f(0,1); glVertex3f(-wd,ht, 1);
   glEnd();
   //  Back
   glBegin(GL_POLYGON);
   glColor3f(0,0,1);
   glNormal3f( 0, 0, -1);
   glTexCoord2f(0,0); glVertex3f(wd,-ht,-1);
   glTexCoord2f(1,0); glVertex3f(-wd,-ht,-1);
   glTexCoord2f(1,1); glVertex3f(-wd,ht,-1);
   glTexCoord2f(0.5,1); glVertex3f(0, +2, -1);
   glTexCoord2f(0,1); glVertex3f(wd,ht,-1);
   glEnd();
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   //draws squares
   glBegin(GL_QUADS);
   //  Right
   glColor3f(1,1,0);
   glNormal3f( 1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(wd,-ht,+1);
   glTexCoord2f(1,0); glVertex3f(wd,-ht,-1);
   glTexCoord2f(1,1); glVertex3f(wd,ht,-1);
   glTexCoord2f(0,1); glVertex3f(wd,ht,+1);
   //  Left
   glColor3f(0,1,0);
   glNormal3f( -1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-wd,-ht,-1);
   glTexCoord2f(1,0); glVertex3f(-wd,-ht,+1);
   glTexCoord2f(1,1); glVertex3f(-wd,ht,+1);
   glTexCoord2f(0,1); glVertex3f(-wd,ht,-1);
   //  Roof Right
   glColor3f(0,1,1);
   glNormal3f(0.5,1.5,0);
   glTexCoord2f(0,0); glVertex3f(wd+0.5,ht-0.5,+1);
   glTexCoord2f(1,0); glVertex3f(wd+0.5,ht-0.5,-1);
   glTexCoord2f(1,1); glVertex3f(0,+2,-1);
   glTexCoord2f(0,1); glVertex3f(0,+2,+1);
   //  Roof Left
   glColor3f(0,1,1);
   glNormal3f(-0.5,1.5,0);
   glTexCoord2f(0,0); glVertex3f(-wd -0.5,ht-0.5,-1);
   glTexCoord2f(1,0); glVertex3f(-wd -0.5,ht-0.5,+1);
   glTexCoord2f(1,1); glVertex3f(0,+2,+1);
   glTexCoord2f(0,1); glVertex3f(0,+2,-1);
   //  Bottom
   glColor3f(1,0,1);
   glNormal3f(0,1,0);
   glTexCoord2f(0,0); glVertex3f(-wd,-ht,-1);
   glTexCoord2f(1,0); glVertex3f(wd,-ht,-1);
   glTexCoord2f(1,1); glVertex3f(wd,-ht,+1);
   glTexCoord2f(0,1); glVertex3f(-wd,-ht,+1);
   // Chimney Front
   glColor3f(1,0,1);
   glNormal3f( 0.82, 1.25, -0.25);
   glTexCoord2f(0,0); glVertex3f(+0.75,+1, -0.25);
   glTexCoord2f(1,0); glVertex3f(+1,+1, -0.25);
   glTexCoord2f(1,1); glVertex3f(+1,+1.5,-0.25);
   glTexCoord2f(0,1); glVertex3f(+0.75,+1.5, -0.25);
   // Chimney Right
   glColor3f(1,1,0);
   glNormal3f( 1, 1.25, -0.32);
   glTexCoord2f(0,0); glVertex3f(+1,+1, -0.25);
   glTexCoord2f(1,0); glVertex3f(+1,+1, -0.5);
   glTexCoord2f(1,1); glVertex3f(+1,+1.5,-0.5);
   glTexCoord2f(0,1); glVertex3f(+1,+1.5, -0.25);
   // Chimney Back
   glColor3f(0,0,1);
   glNormal3f( 0.82, 1.25, -0.5);
   glTexCoord2f(0,0); glVertex3f(+1,+1, -0.5);
   glTexCoord2f(1,0); glVertex3f(+0.75,+1, -0.5);
   glTexCoord2f(1,1); glVertex3f(+0.75,+1.5, -0.5);
   glTexCoord2f(0,1); glVertex3f(+1,+1.5,-0.5);
   // Chimney Left
   glColor3f(0,1,0);
   glNormal3f( 0.75, 1.25, -0.32);
   glTexCoord2f(0,0); glVertex3f(+0.75,+1, -0.5);
   glTexCoord2f(1,0); glVertex3f(+0.75,+1, -0.25);
   glTexCoord2f(1,1); glVertex3f(+0.75,+1.5, -0.25);
   glTexCoord2f(0,1); glVertex3f(+0.75,+1.5, -0.5);
   // Chimney Top
   glColor3f(0,1,1);
   glNormal3f( 0.82, 1.5, -0.32);
   glTexCoord2f(0,0); glVertex3f(+0.75,+1.5, -0.25);
   glTexCoord2f(1,0); glVertex3f(+1,+1.5,-0.25);
   glTexCoord2f(1,1); glVertex3f(+1,+1.5,-0.5);
   glTexCoord2f(0,1); glVertex3f(+0.75,+1.5, -0.5);
   //  End
   glEnd();
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
 }
 /*
  *  Draw vertex in polar coordinates with normal
  */
 static void Vertex(double th,double ph)
 {
    double x = Sin(th)*Cos(ph);
    double y = Cos(th)*Cos(ph);
    double z =         Sin(ph);
    //  For a sphere at the origin, the position
    //  and normal vectors are the same
    glNormal3d(x,y,z);
    glVertex3d(x,y,z);
 }

/*
 * DRAW A BALL @ (X,Y,Z) WITH RADIUS r
 */
 static void ball(double x,double y,double z,double r)
 {
    int th,ph;
    float yellow[] = {1.0,1.0,0.0,1.0};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glScaled(r,r,r);
    //  White ball
    glColor3f(1,1,1);
    glMaterialf(GL_FRONT,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
    glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
    //  Bands of latitude
    for (ph=-90;ph<90;ph+=inc)
    {
       glBegin(GL_QUAD_STRIP);
       for (th=0;th<=360;th+=2*inc)
       {
          Vertex(th,ph);
          Vertex(th,ph+inc);
       }
       glEnd();
    }
    //  Undo transofrmations
    glPopMatrix();
 }
 /*
  *  Draw solid airplane
  *    at (x,y,z)
  *    nose towards (dx,dy,dz)
  *    up towards (ux,uy,uz)
  */
 static void SolidPlane(double x,double y,double z,
                        double dx,double dy,double dz,
                        double ux,double uy, double uz)
 {
    // Dimensions used to size airplane
    const double wid=0.05;
    const double nose=+0.50;
    const double cone= 0.20;
    const double wing= 0.00;
    const double strk=-0.20;
    const double tail=-0.50;
    //  Unit vector in direction of flght
    double D0 = sqrt(dx*dx+dy*dy+dz*dz);
    double X0 = dx/D0;
    double Y0 = dy/D0;
    double Z0 = dz/D0;
    //  Unit vector in "up" direction
    double D1 = sqrt(ux*ux+uy*uy+uz*uz);
    double X1 = ux/D1;
    double Y1 = uy/D1;
    double Z1 = uz/D1;
    //  Cross product gives the third vector
    double X2 = Y0*Z1-Y1*Z0;
    double Y2 = Z0*X1-Z1*X0;
    double Z2 = X0*Y1-X1*Y0;
    //  Rotation matrix
    double mat[16];
    mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
    mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
    mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
    mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;

    //  Save current transforms
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glMultMatrixd(mat);
    //  Nose (4 sided)
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[2]);

    glColor3f(0,0,1);
    glBegin(GL_TRIANGLES);
    glNormal3d(nose, 0.0, 1);
    glTexCoord2f(0,0); glVertex3d(nose, 0.0, 0.0);
    glNormal3d(cone,-wid, 1);
    glTexCoord2f(1,0); glVertex3d(cone, wid, wid);
    glNormal3d(cone,-wid, 1);
    glTexCoord2f(0.5,1); glVertex3d(cone,-wid, wid);

    glNormal3d(nose, 0.0, 1);
    glTexCoord2f(0,0); glVertex3d(nose, 0.0, 0.0);
    glNormal3d(nose, wid, 1);
    glTexCoord2f(1,0); glVertex3d(cone, wid,-wid);
    glNormal3d(cone,-wid, 1);
    glTexCoord2f(0.5,1); glVertex3d(cone,-wid,-wid);

    glNormal3d(nose, 0.0, 1);
    glTexCoord2f(0,0); glVertex3d(nose, 0.0, 0.0);
    glNormal3d(cone, wid, 1);
    glTexCoord2f(1,0); glVertex3d(cone, wid, wid);
    glNormal3d(cone, wid, 1);
    glTexCoord2f(0.5,1); glVertex3d(cone, wid,-wid);

    glNormal3d(nose, 0.0, 1);
    glTexCoord2f(0,0); glVertex3d(nose, 0.0, 0.0);
    glNormal3d(cone,-wid, 1);
    glTexCoord2f(1,0); glVertex3d(cone,-wid, wid);
    glNormal3d(cone,-wid, 1);
    glTexCoord2f(0.5,1); glVertex3d(cone,-wid,-wid);
    glEnd();

    //  Fuselage (square tube)
    glBegin(GL_QUADS);
    glNormal3d((cone+tail)/2,0,wid);
    glTexCoord2f(0,0); glVertex3d(cone, wid, wid);
    glTexCoord2f(1,0); glVertex3d(cone,-wid, wid);
    glTexCoord2f(1,1); glVertex3d(tail,-wid, wid);
    glTexCoord2f(0,1); glVertex3d(tail, wid, wid);

    glNormal3d((cone+tail)/2,0,-wid);
    glTexCoord2f(0,0); glVertex3d(cone, wid,-wid);
    glTexCoord2f(1,0); glVertex3d(cone,-wid,-wid);
    glTexCoord2f(1,1); glVertex3d(tail,-wid,-wid);
    glTexCoord2f(0,1); glVertex3d(tail, wid,-wid);

    glNormal3d((cone+tail)/2, wid, 0);
    glTexCoord2f(0,0); glVertex3d(cone, wid, wid);
    glTexCoord2f(1,0); glVertex3d(cone, wid,-wid);
    glTexCoord2f(1,1); glVertex3d(tail, wid,-wid);
    glTexCoord2f(0,1); glVertex3d(tail, wid, wid);

    glNormal3d((cone+tail)/2, -wid, 0);
    glTexCoord2f(0,0); glVertex3d(cone,-wid, wid);
    glTexCoord2f(1,0); glVertex3d(cone,-wid,-wid);
    glTexCoord2f(1,1); glVertex3d(tail,-wid,-wid);
    glTexCoord2f(0,1); glVertex3d(tail,-wid, wid);

    glNormal3d(tail, 0, 0);
    glTexCoord2f(0,0); glVertex3d(tail,-wid, wid);
    glTexCoord2f(1,0); glVertex3d(tail, wid, wid);
    glTexCoord2f(1,1); glVertex3d(tail, wid,-wid);
    glTexCoord2f(0,1); glVertex3d(tail,-wid,-wid);
    glEnd();
    //  Wings (plane triangles)
    glColor3f(1,1,0);
    glBegin(GL_TRIANGLES);
    glNormal3d(wing, 0.0, 1);
    glTexCoord2f(0,0); glVertex3d(wing, 0.0, wid);
    glNormal3d(tail, 0.0, 1);
    glTexCoord2f(1,0); glVertex3d(tail, 0.0, wid);
    glNormal3d(tail, 0.0, 1);
    glTexCoord2f(0.5,1); glVertex3d(tail, 0.0, 0.5);

    glNormal3d(wing, 0.0, 1);
    glTexCoord2f(0,0); glVertex3d(wing, 0.0,-wid);
    glNormal3d(tail, 0.0, 1);
    glTexCoord2f(1,0); glVertex3d(tail, 0.0,-wid);
    glNormal3d(tail, 0.0, 1);
    glTexCoord2f(0.5,1); glVertex3d(tail, 0.0,-0.5);
    glEnd();
    //  Vertical tail (plane triangle)
    glColor3f(1,0,0);
    glBegin(GL_TRIANGLES);
    glNormal3d(strk, 0.0, 1);
    glTexCoord2f(0,0); glVertex3d(strk, 0.0, 0.0);
    glNormal3d(tail, 0.3, 1);
    glTexCoord2f(1,0); glVertex3d(tail, 0.3, 0.0);
    glNormal3d(tail, 0.0, 1);
    glTexCoord2f(0.5,1); glVertex3d(tail, 0.0, 0.0);
    glEnd();
    //  Undo transformations
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
 }



 /*
  *  OpenGL (GLUT) calls this routine to display the scene
  */
 void display()
 {
    const double len=2.0;  //  Length of axes
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    //  Undo previous transformations
    glLoadIdentity();
    //  Set view angle
    // glRotatef(ph,1,0,0);
    // glRotatef(th,0,1,0);
    double Ex = -2*dim*Sin(th)*Cos(ph);
    double Ey = +2*dim        *Sin(ph);
    double Ez = +2*dim*Cos(th)*Cos(ph);
    gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
    //Smooth shading
    glShadeModel(GL_SMOOTH);

    //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light position
      float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);



    //  Draws 2 houses and a plane
    house(1,0,0 , 0.6,0.6,0.3 , 15);
    house(0,0,0.5 , 0.2,0.2,0.2 , 45);
    SolidPlane( -1, 1, 0 , 1,0,0 , 0, 5, 5);


    //  White
    glColor3f(1,1,1);
    //  Draw axes
    if (axes)
    {
       glBegin(GL_LINES);
       glVertex3d(0.0,0.0,0.0);
       glVertex3d(len,0.0,0.0);
       glVertex3d(0.0,0.0,0.0);
       glVertex3d(0.0,len,0.0);
       glVertex3d(0.0,0.0,0.0);
       glVertex3d(0.0,0.0,len);
       glEnd();
       //  Label axes
       glRasterPos3d(len,0.0,0.0);
       Print("X");
       glRasterPos3d(0.0,len,0.0);
       Print("Y");
       glRasterPos3d(0.0,0.0,len);
       Print("Z");
    }
    glWindowPos2i(5,5);
    Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=Perpective Light=%s",
      th,ph,dim,fov,light?"On":"Off");
    if (light)
    {
       glWindowPos2i(5,45);
       Print("Distance=%d Elevation=%.1f",distance,ylight);
       glWindowPos2i(5,25);
       Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
    }

    //  Render the scene and make it visible
    ErrCheck("display");
    glFlush();
    //  Make the rendered scene visible
    glutSwapBuffers();
 }

 /*
  *  GLUT calls this routine when an arrow key is pressed
  */
 void special(int key,int x,int y)
 {
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
       th += 5;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
       th -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
       ph += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
       ph -= 5;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
 }

 /*
  *  GLUT calls this routine when a key is pressed
  */
 void key(unsigned char ch,int x,int y)
 {
   //  Exit on ESC
  if (ch == 27)
     exit(0);
  //  Reset view angle
  else if (ch == '0')
     th = ph = 0;
  //  Toggle axes
  else if (ch == 'x' || ch == 'X')
     axes = 1-axes;
  //  Toggle lighting
  else if (ch == 'l' || ch == 'L')
     light = 1-light;
  //  Toggle light movement
  else if (ch == 'm' || ch == 'M')
     move = 1-move;
  //  Move light
  else if (ch == '<')
     zh += 1;
  else if (ch == '>')
     zh -= 1;
  //  Change field of view angle
  else if (ch == '-' && ch>1)
     fov--;
  else if (ch == '+' && ch<179)
     fov++;
  //  Light elevation
  else if (ch=='[')
     ylight -= 0.1;
  else if (ch==']')
     ylight += 0.1;
  //  Ambient level
  else if (ch=='a' && ambient>0)
     ambient -= 5;
  else if (ch=='A' && ambient<100)
     ambient += 5;
  //  Diffuse level
  else if (ch=='d' && diffuse>0)
     diffuse -= 5;
  else if (ch=='D' && diffuse<100)
     diffuse += 5;
  //  Specular level
  else if (ch=='s' && specular>0)
     specular -= 5;
  else if (ch=='S' && specular<100)
     specular += 5;
  //  Emission level
  else if (ch=='e' && emission>0)
     emission -= 5;
  else if (ch=='E' && emission<100)
     emission += 5;
  //  Shininess level
  else if (ch=='n' && shininess>-1)
     shininess -= 1;
  else if (ch=='N' && shininess<7)
     shininess += 1;
  //  Translate shininess power to value (-1 => 0)
  shiny = shininess<0 ? 0 : pow(2.0,shininess);
    //  Switch display mode
    glutPostRedisplay();
 }

 /*
  *  GLUT calls this routine when the window is resized
  */
 void reshape(int width,int height)
 {
   //  Ratio of the width to the height of the window
  asp = (height>0) ? (double)width/height : 1;
  //  Set the viewport to the entire window
  glViewport(0,0, width,height);
  //  Set projection
  Project(fov,asp,dim);

 }

 /*
  *  GLUT calls this toutine when there is nothing else to do
  */
  void idle()
  {
     //  Elapsed time in seconds
     double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
     zh = fmod(90*t,360.0);
     //  Tell GLUT it is necessary to redisplay the scene
     glutPostRedisplay();
  }


 /*
  *  Start up GLUT and tell it what to do
  */
 int main(int argc,char* argv[])
 {
    //  Initialize GLUT and process user parameters
    glutInit(&argc,argv);
    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    //  Create the window
    glutCreateWindow("MattWaymouth_HW6");
    //  Tell GLUT to call "display" when the scene should be drawn
    glutDisplayFunc(display);
    //  Tell GLUT to call "reshape" when the window is resized
    glutReshapeFunc(reshape);
    //  Tell GLUT to call "special" when an arrow key is pressed
    glutSpecialFunc(special);
    //  Tell GLUT to call "key" when a key is pressed
    glutKeyboardFunc(key);
    //  Tell GLUT to call "idle" when there is nothing else to do
    glutIdleFunc(idle);
    texture[0] = LoadTexBMP("crate.bmp");
    texture[1] = LoadTexBMP("bricks.bmp");
    texture[2] = LoadTexBMP("metallplates.bmp");
    //  Pass control to GLUT so it can interact with the user
    glutMainLoop();
    return 0;
 }
