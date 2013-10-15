/**

  @author F. Aubert
  **/


#include "GLView.h"
#include <iostream>

#include "nurbs.h";

using namespace std;
using namespace prog3d;


/// ctor
GLView::GLView(QWidget *parent) : QGLWidget(parent) {
    _nurbs=new Nurbs();
  setFocusPolicy(Qt::StrongFocus); // this widget can now catch the keyboard events
}


/// dtor
GLView::~GLView() {
}

/**
 initialize GL context (with glew ou without glew) + common set up
 **/
void GLView::initializeGL() {

#ifdef WITH_GLEW
  // initialization of all available OpenGL functionalities
  if (glewInit()!=GLEW_OK) {
    throw ErrorD("cant initialize glew");
  }
#endif

  cout << "GL version :" << glGetString(GL_VERSION) << endl;
  cout << "Shading Language  : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

  // common OpenGL set up
  glEnable(GL_DEPTH_TEST);  // hidden parts removal with depth buffer algorithm
  glClearColor(1,1,1,1); // color value when glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) is done
  glClearDepth(1.0);       // depth value when glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) is done

  glDisable(GL_STENCIL_TEST);
  glClearStencil(0);

  // polygon drawings set up
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glPixelStorei(GL_PACK_ALIGNMENT,4);
  glPixelStorei(GL_UNPACK_ALIGNMENT,4);

  ugl::initUtilGL(this);

  initData();

}


/** ***************************************************************************
resize
  **/


/// when the graphics window is sized/resized (including the first time the window is shown)
void GLView::resizeGL(int width, int height) {
  glViewport(0,0,width,height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1,1,-1,1,-1,1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}



/** **********************************************************************
  Events
  **/
void GLView::mousePressEvent(QMouseEvent *event) {
  if (event->button()==Qt::LeftButton) {
    cout << "left mouse : " << event->x() << "," << event->y() << endl;
  }
  if (event->button()==Qt::RightButton) {
    cout << "right mouse : " << event->x() << "," << event->y() << endl;
  }
}

void GLView::mouseMoveEvent(QMouseEvent *event) {
  cout << "motion : " << event->x() << "," << event->y() << endl;
}

void GLView::mouseReleaseEvent(QMouseEvent *event) {
  switch(event->button()){
  case Qt::LeftButton:
      cout << "left mouse released" << endl;
    break;
  case Qt::RightButton:
    break;
  default:break;
  }
}

void GLView::wheelEvent(QWheelEvent *event) {
  cout << "mouse wheel :" << event->delta() << endl;
}


void GLView::keyPressEvent(QKeyEvent *event) {
  if (event->isAutoRepeat())
    QGLWidget::keyPressEvent(event);
  else
    switch(event->key()){
    case Qt::Key_Z:
        cout << "Z pressed" << endl;
      break;
    case Qt::Key_S:
        cout << "S pressed" << endl;
      break;
    case Qt::Key_Space:
        cout << "space pressed" << endl;
      break;
    default:
      QGLWidget::keyPressEvent(event); // dispatch the event to the parent
    }
}

void GLView::keyReleaseEvent(QKeyEvent *event) {
  if (event->isAutoRepeat())
    QGLWidget::keyReleaseEvent(event);
  else
    switch(event->key()){
      case Qt::Key_Space:
        cout << "space released" << endl;
      case Qt::Key_Z:
        break;
      case Qt::Key_S:
        break;
      default:
        QGLWidget::keyReleaseEvent(event); // dispatch the event to the parent
    }
}



/** ***************************************************************************
  init/update data
  **/
void GLView::initData() {
    _p=2;
    _m=4;
    _choice=0;
}

void GLView::updateData() {
  // update data :


  // update view :
  updateGL();
}


/** ******************************************************************* **/
/**
  Drawings
**/



void GLView::paintGL() {    
    _nurbs->initialize(_m,_p);
    if(_choice==1) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        _nurbs->drawAllN(_p);

        // Question 5
        /*
         * Nk,p nuls pour t = 3.5 :
         * p=1 : N(0,1), N(3,1), N(4,1)
         * p=2 : N(3,2)
         * p=3 : /
         */
        glPopMatrix();
        _choice=0;
    }
    if(_choice==2) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        vector<Vector2> P = {Vector2(1,0),Vector2(2,0.7),Vector2(3,0.8),Vector2(5,0.5),Vector2(4,0.3)};
        _nurbs->drawAllN(_p);
        _nurbs->drawControlPolygone(P);
        _nurbs->drawBSpline(P,_p);
        glPopMatrix();
        _choice=0;
    }
    if(_choice==3) {}
    if(_choice==4) {}
    if(_choice==5) {}
}

/** ********************************************************************** **/

void GLView::choice1() { _choice=1; }
void GLView::choice2() { _choice=2; }
void GLView::choice3() { _choice=3; }
void GLView::choice4() { _choice=4; }
void GLView::choice5() { _choice=5; }



