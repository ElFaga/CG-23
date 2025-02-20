////////////////////////////////////////////////////////////////////////////////////        
// canvas.cpp
//
// This program allows the user to draw simple shapes on a canvas.
//
// Interaction:
// Left click on a box on the left to select a primitive.
// Then left click on the drawing area: once for point, twice for line or rectangle.
// Right click for menu options.
//
//  Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////// 

#include <cmath>
#include <cstdlib>
#include <vector>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

#define PI 3.14159265358979324
#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define MULTILINE 4
#define CIRCLE 5
#define ISO_TRIANGLE 6
#define RET_TRIANGLE 7
#define EQU_TRIANGLE 8
#define POLYGONREGULAR 9
#define TEXT 10
#define NUMBERPRIMITIVES 10

// Use the STL extension of C++.
using namespace std;

// Globals.
static GLsizei width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int tempX, tempY; // Co-ordinates of clicked point.
static int isGrid = 1, grid_up_down=0, mode=0; // Is there grid?
static float currentColor[][7] = {
   {1.0, 0.0, 0.0}, // Green
   {0.0, 1.0, 0.0}, // Blue
   {0.0, 0.0, 1.0}, // Yellow
   {0.98, 0.992, 0.058}, // Purple
   {0.372, 0.0, 0.627}, // Pink
   {1.0, 0.262, 0.643}, // Black
}; // Current color being used
static float *selectColor = currentColor[7];
bool fillShape = false; // Filled or wire shapes?
static int vert = 3; // Initial vertices from regular polygon 

// Point class.
class Point
{
public:
   Point(int xVal, int yVal, float *colorClass)
   {
	  x = xVal; y = yVal;
     this->color = colorClass;
   }
   void drawPoint(void); // Function to draw a point.
private:
   int x, y; // x and y co-ordinates of point.
   static float size; // Size of point.
   float *color;
};

float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint()
{  
   glColor3fv(this->color);
   glPointSize(size);
   glBegin(GL_POINTS);
      glVertex3f(x, y, 0.0);
   glEnd();   
}

// Vector of points.
vector<Point> points;

// Iterator to traverse a Point array.
vector<Point>::iterator pointsIterator; 

// Function to draw all points in the points array.
void drawPoints(void)
{
   // Loop through the points array drawing each point.
   pointsIterator = points.begin();
   while(pointsIterator != points.end() )
   {
      pointsIterator->drawPoint();
	  pointsIterator++;
   }
}
///////////////////////////////////////////////
// Line class.
class Line
{
public:
   Line(int x1Val, int y1Val, int x2Val, int y2Val, float *colorClass)
   {
	  x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
     this->color = colorClass;
   }
   void drawLine();
private:
   int x1, y1, x2, y2; // x and y co-ordinates of endpoints.
   float *color;
};


// Function to draw a line.
void Line::drawLine()
{
   glBegin(GL_LINES);
      glColor3fv(this->color);
      glVertex3f(x1, y1, 0.0);
      glVertex3f(x2, y2, 0.0);
   glEnd();
}

// Vector of lines.
vector<Line> lines;

// Iterator to traverse a Line array.
vector<Line>::iterator linesIterator;

// Function to draw all lines in the lines array.
void drawLines(void)
{
   // Loop through the lines array drawing each line.
   linesIterator = lines.begin();
   while(linesIterator != lines.end() )
   {
      linesIterator->drawLine();
	  linesIterator++;
   }
}
/////////////////////////////////////////
// Rectangle class.
class Rectangle
{
public:
   Rectangle(int x1Val, int y1Val, int x2Val, int y2Val, float *colorClass)
   {
	  x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
     full = fillShape;
     this->color = colorClass;
   }
   void drawRectangle();
private:
   int x1, y1, x2, y2; // x and y co-ordinates of diagonally opposite vertices.
   bool full;
   float *color;
};

// Function to draw a rectangle.
void Rectangle::drawRectangle()
{
   glColor3fv(this->color);
   if(!full) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(x1, y1, x2, y2);
}

// Vector of rectangles.
vector<Rectangle> rectangles;

// Iterator to traverse a Rectangle array.
vector<Rectangle>::iterator rectanglesIterator;

// Function to draw all rectangles in the rectangles array.
void drawRectangles(void)
{
   // Loop through the rectangles array drawing each rectangle.
   rectanglesIterator = rectangles.begin();
   while(rectanglesIterator != rectangles.end() )
   {
      rectanglesIterator->drawRectangle();
	  rectanglesIterator++;
   }
}
//////////////////////////////////////////////////
class Multiline
{
   public: 
   Multiline(int x1Val, int y1Val, int x2Val, int y2Val, float *colorClass)
   {
      x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
      this->color = colorClass;
   }
   void drawMultilines(void); // Function to draw a multiline object

   private:
      int x1, y1, x2, y2; // x and y co-ordinates of point
      float *color;
};

void Multiline::drawMultilines()
{
   glColor3fv(this->color);
   glBegin(GL_LINES);
      glVertex3f(x1, y1, 0.0);
      glVertex3f(x2, y2, 0.0);
   glEnd();
}

// Vector of Multilines.
vector<Multiline> multilines;

//Iterator to travese a multiline array
vector<Multiline>::iterator multilinesIterator;

//Function to draww all multilines in the multiline array
void drawMultilines(void)
{
   // Loop through the multilines array drawing each line
   multilinesIterator = multilines.begin();
   while(multilinesIterator != multilines.end())
   {
      multilinesIterator->drawMultilines();
      multilinesIterator++;
   }
}
/////////////////////////////////////////////////////
class Circle
{
   public:
      Circle(int x1Val, int y1Val, int x2Val, int y2Val, float *colorClass)
      {
         x1 = x1Val; y1 = y1Val; rad = ((abs(x2Val - x1Val))^2 + (abs(y2Val - y1Val))^2);
         full = fillShape;
         this->color = colorClass;
      }
      void drawCircle();
   private:
      int x1, y1;
      float rad;
      bool full;
      float *color;
};

void Circle::drawCircle()
{
   float t = 0;
   glColor3fv(this->color);
   if(!full) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_POLYGON);
   for(int i=0; i < 100; ++i)
   {
      glVertex3f(x1 + rad * cos(t), y1 + rad * sin(t), 0.0);
      t += 2* PI / 100;
   }
   glEnd();

}

// Vector of Circles.
vector<Circle> circles;

// Iterator to traverse a circle array
vector<Circle>::iterator circleIterator;

//FUnction to draw all circles in the circle array
void drawCircles(void)
{
   circleIterator = circles.begin();
   while(circleIterator != circles.end())
   {
      circleIterator->drawCircle();
      circleIterator++;
   }
}
////////////////////////////////
class Iso_Triangle
{
public:
   Iso_Triangle(int x1Val, int y1Val, int x2Val, int y2Val, float *colorClass)
   {
      x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
      dab = ((abs(x2 - x1))^2  + (abs(y2 - y1))^2);
      full = fillShape;
      this->color = colorClass;
   }
   void drawIsoTriangle();
private:
   int x1, y1, x2, y2;
   float *color;
   float dab;
   bool full;
};

void Iso_Triangle::drawIsoTriangle()
{
   glColor3fv(this->color);
   if(!full) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_POLYGON);
        glVertex3f(x1, y1, 0.0);
        glVertex3f(x2, y2, 0.0);
        glVertex3f(x2, y2, 0.0);
        glVertex3f(dab, y1, 0.0);
    glEnd();
}

// Vector of isoscles triangles
vector<Iso_Triangle> iso_triangles;

// Iterator to traverse a Isosceles Tirangles array.
vector<Iso_Triangle>::iterator iso_trianglesIterator;

// Function to draw all isoscles triangles in the isosceles triangles array.
void drawIsoTriangles(void)
{
    // Loop through the isosceles triangles array drawing each isosceles triangle
    iso_trianglesIterator = iso_triangles.begin();
    while(iso_trianglesIterator != iso_triangles.end())
    {
        iso_trianglesIterator->drawIsoTriangle();
        iso_trianglesIterator++;
    }

}
/////////////////////
class Ret_Triangle
{
public:
   Ret_Triangle(int x1Val, int y1Val, int x2Val, int y2Val, float *colorClass)
   {
      x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
      full = fillShape;
      this->color = colorClass;
   }
   void drawRetTriangle();
private:
   int x1, y1, x2, y2;
   bool full;
   float *color;
};

void Ret_Triangle::drawRetTriangle()
{
   int flag;
   glColor3fv(this->color);
   if(!full) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_POLYGON);
         
      if (x1 < x2 + 50 || x2 < x1 + 50) flag = 1;
      if (y1 < y2 + 100 || y2 < y1 + 100) flag = 2;         
      switch (flag)
      {
         case 0:
            glVertex2i(x1, y1);
            glVertex2i(x1, y2);
            glVertex2i(x2, y2);
            break;
         case 1:
            glVertex2i(x1, y1*2);
            glVertex2i(x1, y2);
            glVertex2i(x2, y2*2);
            break;
         case 2:        
            glVertex2i(x1, y1);
            glVertex2i(x1, y2);
            glVertex2i(x2+50, y2);
            break;
         default:
            break;
      }
         
      
   glEnd();
}
// Vector of retangle triangles
vector<Ret_Triangle> ret_triangles;

// Iterator to traverse a Retangle Triangles array
vector<Ret_Triangle>::iterator ret_trianglesIterator;

//Function to draw all retangle triangles in the retangle triangle array
void drawRetTriangles(void)
{
   //Loop through the retangles triangles in the retangle triangle array
   ret_trianglesIterator = ret_triangles.begin();
   while(ret_trianglesIterator != ret_triangles.end())
   {
      ret_trianglesIterator->drawRetTriangle();
      ret_trianglesIterator++;
   }
}
/////////////////////////////////
class Equ_Triangle
{
public:
   Equ_Triangle(int x1Val, int y1Val, int x2Val, int y2Val, float *colorClass)
   {
      x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
      full = fillShape;
      this->color = colorClass;
      dab = ((abs(x2 - x1))^2  + (abs(y2 - y1))^2);
   }
   void drawEquTriangle();
private:
   int x1, y1, x2, y2;
   bool full;
   float *color;
   float dab;
};

void Equ_Triangle::drawEquTriangle()
{
   glColor3fv(this->color);
   if(!full) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_POLYGON);
      glVertex3f(x1, y1, 0.0);
      glVertex3f(x2, y2, 0.0);
      glVertex3f(x1+x2/3, y1, 0.0);
   glEnd();
}
// Vector of equilateral triangles
vector<Equ_Triangle> equ_triangles;

// Iterator to traverse a Equilateral Triangles array
vector<Equ_Triangle>::iterator equ_trianglesIterator;

//Function to draw all equilateral triangles in the equilateral triangle array
void drawEquTriangles(void)
{
   //Loop through the equilateral triangles in the equilateral triangle array
   equ_trianglesIterator = equ_triangles.begin();
   while(equ_trianglesIterator != equ_triangles.end())
   {
      equ_trianglesIterator->drawEquTriangle();
      equ_trianglesIterator++;
   }
}
////////////////////////////
// Regular Polygon Class
class PolygonReg
{
public:
   PolygonReg(int x1Val, int y1Val, int vert, float rad, float *colorClass)
   {
      x1 = x1Val; y1 = y1Val;
      v = vert; r = rad; this->color = colorClass;
      full = fillShape;
   }
   void drawPolygonReg();
private:
   int x1, y1, v;
   float r;
   float *color;
   bool full;
};

// Function to draw a Regular Polygon
void PolygonReg::drawPolygonReg()
{
   float t = 0;
   int i;

   glColor3fv(this->color);
   if(!full) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_POLYGON);
      for(i = 0; i < v; ++i)
      {
         glVertex3f(x1 + r * cos(t), y1 + r * sin(t), 0.0);
         t += 2 * PI / v;
      }
   glEnd();

}

// Vector of Regular Polygons
vector<PolygonReg> PolygonsRegs;

// Iterator to traverse a Regular Polygon array
vector<PolygonReg>::iterator PolygonRegsIterator;

// Function to draw all Regular Polygons in the array
void drawPolygonReg(void)
{
   // Loop through the regular polygons array
   PolygonRegsIterator = PolygonsRegs.begin();
   while(PolygonRegsIterator != PolygonsRegs.end())
   {
      PolygonRegsIterator->drawPolygonReg();
      PolygonRegsIterator++;
   }
}
///////////////////////////
void writeBitmapString(void *font, char *msg)
{
   char *c;
   for (c = msg; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

class Text
{
public:
   Text(int x1Val, int y1Val, float *colorClass, void *font, char *msg)
   {
      this->font = font;
      this->msg = msg;
      x1 = x1Val; y1 = y1Val;
      this->color = colorClass;
   }
   void drawText();
private:
   float *color;
   void *font;
   char *msg;
   int x1, y1;

};

void Text::drawText()
{
   glColor3fv(this->color);
   glRasterPos3f(this->x1, this->y1, 0.0);
   char *c;
   for(c = msg; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

vector<Text> texts;

vector<Text>::iterator textIterator;

void drawTexts(void)
{
   textIterator = texts.begin();
   while(textIterator != texts.end())
   {
      textIterator->drawText();
      textIterator++;
   }
}

// Function to draw point selection box in left selection area.
void drawPointSelectionBox(void)
{  
   if (primitive == POINT) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.9*height, 0.1*width, height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.9*height, 0.1*width, height); 

   // Draw point icon.
   glPointSize(pointSize);
   glColor3fv(selectColor);
   glBegin(GL_POINTS);
      glVertex3f(0.05*width, 0.95*height, 0.0);
   glEnd();  
}

// Function to draw line selection box in left selection area.
void drawLineSelectionBox(void)
{  
   if (primitive == LINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

   // Draw line icon.
   glColor3fv(selectColor);
   glBegin(GL_LINES);
      glVertex3f(0.025*width, 0.825*height, 0.0);
      glVertex3f(0.075*width, 0.875*height, 0.0);
   glEnd();  
}

// Function to draw rectangle selection box in left selection area.
void drawRectangleSelectionBox(void)
{  
   if (primitive == RECTANGLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

   // Draw black boundary.
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

   // Draw rectangle icon.
   glColor3fv(selectColor);
   if(!fillShape) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.025*width, 0.735*height, 0.075*width, 0.765*height);
   glEnd();  
}

void drawMultilineSelectionBox(void)
{
   if (primitive == MULTILINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
   else glColor3f(0.8, 0.8, 0.8); // No highlight
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.6*height, 0.1 *width, 0.7*height);

   // Draw a black bonduary
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.6*height, 0.1 *width, 0.7*height);

   // Draw a multiline icon
  glColor3fv(selectColor);
   glBegin(GL_LINES);
      glVertex3f(0.025*width, 0.630*height, 0.0);
      glVertex3f(0.045*width, 0.680*height, 0.0);
      glVertex3f(0.045*width, 0.680*height, 0.0);
      glVertex3f(0.060*width, 0.650*height, 0.0);
      glVertex3f(0.060*width, 0.650*height, 0.0);
      glVertex3f(0.075*width, 0.687*height, 0.0);
   glEnd();
}

void drawCircleSelectionBox()
{
   float t = 0; // Angle parameter
   
   if (primitive == CIRCLE) glColor3f(1.0, 1.0, 1.0);
   else glColor3f(0.8, 0.8, 0.8);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

   // Draw black bonduary
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.5*height, 0.1*width, 0.6*height);

   // Draw circle icon
   glColor3fv(selectColor);
   if(!fillShape) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_POLYGON);
      for (int i = 0; i < 100; ++i)
      {
         glVertex3f(0.05 * width + 15 * cos(t), 0.55*height + 15 * sin(t), 0.0);
         t += 2 * PI / 100;
      }
   glEnd();
}

void drawIsoTriangleSelectionBox(void)
{
   if (primitive == ISO_TRIANGLE)glColor3f(1.0, 1.0, 1.0); // Highlit
    else glColor3f(0.8, 0.8, 0.8); // No Highlight.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.4*height, 0.1*width, 0.5*height);

    // Draw a black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.4*height, 0.1*width, 0.5*height);

    // Draw Iso Triangle icon.
   glColor3fv(selectColor);
   if(!fillShape) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_POLYGON);
        //Left Side
        glVertex3f(0.0325*width, 0.435*height, 0.0);
        glVertex3f(0.0525*width, 0.485*height, 0.0);
        //Right Side
        glVertex3f(0.0625*width, 0.435*height, 0.0);
        glVertex3f(0.0525*width, 0.485*height, 0.0);
        //Down Side
        glVertex3f(0.0325*width, 0.435*height, 0.0);
        glVertex3f(0.0625*width, 0.435*height, 0.0);
    glEnd();

}

void drawRetTriangleSelectionBox(void)
{
   if (primitive == RET_TRIANGLE) glColor3f(1.0, 1.0, 1.0); // Highlight
   else glColor3f(0.8, 0.8, 0.8); // No Highlight
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.3*height, 0.1*width, 0.4*height);

   // Draw a black boundary
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.3*height, 0.1*width, 0.4*height);

   // Draw Rec Triangle icon.
   glColor3fv(selectColor);
   if(!fillShape) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_POLYGON);
        // Left Side
        glVertex3f(0.0225*width, 0.335*height, 0.0);
        glVertex3f(0.0225*width, 0.385*height, 0.0);
        // Down Side
        glVertex3f(0.0225*width, 0.335*height, 0.0);
        glVertex3f(0.0625*width, 0.335*height, 0.0);
        // Hipo Side
        glVertex3f(0.0225*width, 0.385*height, 0.0);
        glVertex3f(0.0625*width, 0.335*height, 0.0);
        
   glEnd();
}

void drawEquTriangleSelectionBox(void)
{
   if (primitive == EQU_TRIANGLE) glColor3f(1.0, 1.0, 1.0); // Highlight
   else glColor3f(0.8, 0.8, 0.8); // No Highlight
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.2*height, 0.1*width, 0.3*height);

   // Draw a black boundary
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.2*height, 0.1*width, 0.3*height);

   // Draw Equ Triangle icon.
   glColor3fv(selectColor);
   if(!fillShape) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_POLYGON);
        // Left Side
        glVertex3f(0.0225*width, 0.235*height, 0.0);
        glVertex3f(0.0525*width, 0.285*height, 0.0);
        // Down Side
        glVertex3f(0.0225*width, 0.235*height, 0.0);
        glVertex3f(0.0825*width, 0.235*height, 0.0);
        //Right Side
        glVertex3f(0.0525*width, 0.285*height, 0.0);
        glVertex3f(0.0825*width, 0.235*height, 0.0);
        
   glEnd();
}

void drawPolygonRegularSelectionBox(void)
{
   if(primitive == POLYGONREGULAR) glColor3f(1.0, 1.0, 1.0); // Highlight
   else glColor3f(0.8, 0.8, 0.8); // No highlight
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.1*height, 0.1*width, 0.2*height);

   // Draw a black bonduary
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.1*height, 0.1*width, 0.2*height);

   // Draw a regular polygon icon 
   float t;
   int i;

   glColor3fv(selectColor);
   if(!fillShape) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_POLYGON);
      
      glVertex3f(0.0225*width, 0.145*height, 0.0);
      glVertex3f(0.05*width, 0.175*height, 0.0);
      
      glVertex3f(0.0725*width, 0.145*height, 0.0);
      glVertex3f(0.05*width, 0.175*height, 0.0);
      
      glVertex3f(0.0225*width, 0.145*height, 0.0);
      glVertex3f(0.05*width, 0.115*height, 0.0);
      
      glVertex3f(0.0725*width, 0.145*height, 0.0);
      glVertex3f(0.05*width, 0.115*height, 0.0);
   glEnd();
}

void drawTextSelectionBox(void)
{
   if(primitive == TEXT) glColor3f(1.0, 1.0, 1.0); // Highlight
   else glColor3f(0.8, 0.8, 0.8); // No highlight
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.0*height, 0.1*width, 0.1*height);

   // Draw a black bonduary
   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.0*height, 0.1*width, 0.1*height);

   // Draw a regular polygon icon 
   float t;
   int i;

   glColor3fv(selectColor);
   glRasterPos3f(0.01 * width, 0.058 *height, 0.0);
   writeBitmapString(GLUT_BITMAP_8_BY_13, "TEXT");
}

// Function to draw unused part of left selection area.
void drawInactiveArea(void)
{
   glColor3f(0.6, 0.6, 0.6);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);

   glColor3f(0.0, 0.0, 0.0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);
}

// Function to draw temporary point.
void drawTempPoint(void)
{
   glColor3f(1.0, 0.0, 0.0);
   glPointSize(pointSize);
   glBegin(GL_POINTS);
      glVertex3f(tempX, tempY, 0.0);
   glEnd(); 
}

// Function to draw a grid.
void drawGrid(void) {
    int i;

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x5555);
    glColor3f(0.75, 0.75, 0.75);

    glBegin(GL_LINES);
    if(grid_up_down == 2){ //  DIVIDE O GRID EM PARTES MAIORES
        for (i = 2; i <= 17; i=i+3) {
            glVertex3f(i * 0.05 * (width), 0.0, 0.0); //HORIZONTAL
            glVertex3f(i * 0.05 * (width), height, 0.0);
        }
        for (i = 0; i <= 20; i=i+3) {
            glVertex3f(0.05 * width, i * 0.05 * height, 0.0); //VERTICAL
            glVertex3f(width, i * 0.05 * height, 0.0);
        }
    }else if (grid_up_down == -2){ //DIVIDE O GRID EM PARTES MENORES
        for (i = 2; i <= 19; i++) {
            glVertex3f(i * 0.05 * (width), 0.0, 0.0);
            glVertex3f(i * 0.05 * (width), height, 0.0);
        }
        for (i = 1; i <= 19; i++) {
            glVertex3f(0.1 * width, i * 0.05 * height, 0.0);
            glVertex3f(width, i * 0.05 * height, 0.0);
        }
    }else if (grid_up_down == 4){ // MAIORES
        for (i = 4; i <= 19; i=i+3) {
            glVertex3f(i * 0.1 * (width), 0.0, 0.0);
            glVertex3f(i * 0.1 * (width), height, 0.0);
        }
        for (i = 0; i <= 19; i=i+2) {
            glVertex3f(0.1 * width, i * 0.1 * height, 0.0);
            glVertex3f(width, i * 0.1 * height, 0.0);
        }
    }else if (grid_up_down == -4) { //MENORES
        for (i = 4; i <= 39; i++) {
            glVertex3f(i * 0.025 * (width), 0.0, 0.0);
            glVertex3f(i * 0.025 * (width), height, 0.0);
        }
        for (i = 1; i <= 39; i++) {
            glVertex3f(0.1 * width, i * 0.025 * height, 0.0);
            glVertex3f(width, i * 0.025 * height, 0.0);
        }
    }else{//ORIGINAL
        for (i = 2; i <= 9; i++) {
            glVertex3f(i * 0.1 * (width), 0.0, 0.0);
            glVertex3f(i * 0.1 * (width), height, 0.0);
        }
        for (i = 1; i <= 9; i++) {
            glVertex3f(0.1 * width, i * 0.1 * height, 0.0);
            glVertex3f(width, i * 0.1 * height, 0.0);
        }
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);


}

// Drawing routine.
void drawScene(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0, 0.0, 0.0);

   drawPointSelectionBox();
   drawLineSelectionBox();
   drawRectangleSelectionBox();
   drawMultilineSelectionBox();
   drawCircleSelectionBox();
   drawIsoTriangleSelectionBox();
   drawRetTriangleSelectionBox();
   drawEquTriangleSelectionBox();
   drawPolygonRegularSelectionBox();
   drawTextSelectionBox();
   drawInactiveArea();

   drawPoints();
   drawLines();
   drawRectangles();
   drawMultilines();
   drawCircles();
   drawIsoTriangles();
   drawRetTriangles();
   drawEquTriangles();
   drawPolygonReg();
   drawTexts();
   if ( (primitive != POINT) &&
	   (pointCount == 1) ) drawTempPoint();
   if (isGrid) drawGrid();

   glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
   if ( y < (1- NUMBERPRIMITIVES*0.1)*height ) primitive = INACTIVE;
   else if ( y < (1 - 9*0.1)*height ) primitive = TEXT;
   else if ( y < (1 - 8*0.1)*height ) primitive = POLYGONREGULAR;
   else if ( y < (1 - 7*0.1)*height ) primitive = EQU_TRIANGLE;
   else if ( y < (1 - 6*0.1)*height ) primitive = RET_TRIANGLE;
   else if ( y < (1 - 5*0.1)*height ) primitive = ISO_TRIANGLE;
   else if ( y < (1 - 4*0.1)*height ) primitive = CIRCLE;
   else if ( y < (1 - 3*0.1)*height ) primitive = MULTILINE;
   else if ( y < (1 - 2*0.1)*height ) primitive = RECTANGLE;
   else if ( y < (1 - 1*0.1)*height ) primitive = LINE;
   else primitive = POINT;
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      y = height - y; // Correct from mouse to OpenGL co-ordinates.
      
	  // Click outside canvas - do nothing.
      if ( x < 0 || x > width || y < 0 || y > height ) ;

	  // Click in left selection area.
      else if ( x < 0.1*width ) 
	  {
	     pickPrimitive(y);
		 pointCount = 0;
	  }

	  // Click in canvas.
      else
	  {
	     if (primitive == POINT) points.push_back( Point(x, y, selectColor) );
         else if (primitive == LINE) 
		 { 
	        if (pointCount == 0)
			{
               tempX = x; tempY = y;
		       pointCount++;
			}
	        else 
			{
             lines.push_back( Line(tempX, tempY, x, y, selectColor) );
		       pointCount = 0;
			}
		 }
         else if (primitive == RECTANGLE) 
		 { 
	        if (pointCount == 0)
			{
               tempX = x; tempY = y;
		       pointCount++;
			}
	        else 
			{
               rectangles.push_back( Rectangle(tempX, tempY, x, y, selectColor) );
		       pointCount = 0;
			}
		 }
         else if (primitive == MULTILINE)
       {
            if (pointCount == 0)
            {
               tempX = x; tempY = y;
               pointCount++;
            }
            else
            {
               multilines.push_back( Multiline(tempX, tempY, x, y, selectColor) );
               tempX = x; tempY = y;
               pointCount++;
            }
       }
         else if (primitive == CIRCLE)
       {
            if (pointCount == 0)
            {
               tempX = x; tempY = y;
               pointCount++;
            }
            else
            {
               circles.push_back( Circle(tempX, tempY, x, y, selectColor) );
               pointCount = 0;
            }
       }
         else if (primitive == ISO_TRIANGLE)
       {
            if (pointCount == 0)
            {
               tempX = x; tempY = y;
               pointCount++;
            }
            else
            {
               iso_triangles.push_back( Iso_Triangle(tempX, tempY, x, y, selectColor) );
               pointCount = 0;
            }
       }
         else if (primitive == RET_TRIANGLE)
       {
            if (pointCount == 0)
            {
               tempX = x; tempY = y;
               pointCount++;
            }
            else
            {
               ret_triangles.push_back( Ret_Triangle(tempX, tempY, x, y, selectColor) );
               pointCount = 0;
            }
       }
         else if (primitive == EQU_TRIANGLE)
      {
         if (pointCount == 0)
         {
            tempX = x; tempY = y;
            pointCount++;
         }
         else
         {
            equ_triangles.push_back( Equ_Triangle(tempX, tempY, x, y, selectColor) );
            pointCount = 0;
         }
      }
         else if (primitive == POLYGONREGULAR)
      {
            if (pointCount == 0)
            {
               tempX = x; tempY = y;
               pointCount++;
            }
            else
            {
               float r;

               r = sqrt(pow(tempX-x, 2) + pow(tempY-y, 2));
               PolygonsRegs.push_back( PolygonReg(tempX, tempY, vert, r, selectColor));
               pointCount = 0;
            }
      }
         else if (primitive == TEXT)
      {
         if (pointCount == 0)
         {
            tempX = x; tempY = y;
            pointCount++;
         }
         else
         {
            texts.push_back( Text(tempX, tempY, selectColor, GLUT_BITMAP_8_BY_13, "TEXT"));
            pointCount = 0;
         }
      }
	  }
   }
   glutPostRedisplay();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Set viewing box dimensions equal to window dimensions.
   glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);
   
   // Pass the size of the OpenGL window to globals.
   width = w; 
   height = h;  

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:
         exit(0);
         break;
      case 101:
         primitive = INACTIVE;
         break;
      case '+':
         if (primitive == POLYGONREGULAR)
            vert++;
         break;
      case '-':
         if (primitive == POLYGONREGULAR)
            if (vert > 3) vert--;
         break;
      default:
         break;
   }
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void)
{
   points.clear();
   lines.clear();
   rectangles.clear();
   multilines.clear();
   circles.clear();
   iso_triangles.clear();
   ret_triangles.clear();
   equ_triangles.clear();
   PolygonsRegs.clear();
   texts.clear();
   primitive = INACTIVE;
   pointCount = 0;
   grid_up_down = 0;
}

// The right button menu callback function.
void rightMenu(int id)
{
   if (id==1) 
   {
	  clearAll();
	  glutPostRedisplay();
   }
   if (id==2) exit(0);
}

// The sub-menu callback function.
void grid_menu(int id)
{
   if (id==4) isGrid = 0;
   if (id==3)
   {
      isGrid = 1;
      grid_up_down = 0;
   }
   if (id == 14)
   {
      isGrid = 1;
      grid_up_down = 2;
   }
   if (id == 15)
   {
      isGrid = 1;
      grid_up_down = 4;
   }
   if (id == 16)
   {
      isGrid = 1;
      grid_up_down = -2;
   }
   if (id == 17)
   {
      isGrid = 1;
      grid_up_down = -4;
   }
   glutPostRedisplay();
}

void color_menu(int id)
{
   if (id == 5) selectColor = currentColor[1]; 
   if (id == 6) selectColor = currentColor[2]; 
   if (id == 7) selectColor = currentColor[3];
   if (id == 8) selectColor = currentColor[4]; 
   if (id == 9) selectColor = currentColor[5]; 
   if (id == 10) selectColor = currentColor[6]; 
   if (id == 11) selectColor = currentColor[7];
   glutPostRedisplay();
}

void fill_menu(int id)
{
   if (id == 12) fillShape = true;
   if (id == 13) fillShape = false;
   glutPostRedisplay();
}

// Function to create menu.
void makeMenu(void)
{
   int sub_menu;
   sub_menu = glutCreateMenu(grid_menu);
   glutAddMenuEntry("Minuscule", 17);
   glutAddMenuEntry("Low", 16);
   glutAddMenuEntry("Normal", 3);
   glutAddMenuEntry("Big", 14);
   glutAddMenuEntry("Gigant", 15);
   glutAddMenuEntry("Off", 4);

   int sub2;
   sub2 = glutCreateMenu(color_menu);
   glutAddMenuEntry("Green", 5);
   glutAddMenuEntry("Blue", 6);
   glutAddMenuEntry("Yellow", 7);
   glutAddMenuEntry("Purple", 8);
   glutAddMenuEntry("Pink", 9);
   glutAddMenuEntry("Black", 10);

   int sub3;
   sub3 = glutCreateMenu(fill_menu);
   glutAddMenuEntry("Fill", 12);
   glutAddMenuEntry("Outline", 13);

   glutCreateMenu(rightMenu);
   glutAddSubMenu("Grid", sub_menu);
   glutAddSubMenu("Color", sub2);
   glutAddSubMenu("Fill/Wire", sub3);
   glutAddMenuEntry("Clear",1);
   glutAddMenuEntry("Quit",2);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Gabriel Faga - 43560" << endl; 
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_DOUBLE); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(150, 100); 
   glutCreateWindow("Gabriel Faga - 43560");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMouseFunc(mouseControl); 

   makeMenu(); // Create menu.


   glutMainLoop(); 

   return 0;  
}