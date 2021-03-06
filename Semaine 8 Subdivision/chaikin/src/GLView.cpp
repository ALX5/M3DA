/**

  @author F. Aubert
  **/


#include "GLView.h"
#include <iostream>
#include <QPushButton>

using namespace std;
using namespace prog3d;


/// ctor
GLView::GLView(QWidget *parent) : QGLWidget(parent) {

  setFocusPolicy(Qt::StrongFocus); // this widget can now catch the keyboard events

  _choice=1;
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
  double x = event->x() / (double)(width()/2) -1;
  double y = -(event->y() / (double)(height()/2) -1);
  if (event->button()==Qt::LeftButton) {
    cout << "left mouse : " << event->x() << "," << event->y() << endl;
    bool b = false;
    _chaikin.addPoint(x,y,b);
  }
  if (event->button()==Qt::RightButton) {
    cout << "right mouse : " << event->x() << "," << event->y() << endl;
    bool b = true;
    _chaikin.addPoint(x,y,b);
  }
}

void GLView::mouseMoveEvent(QMouseEvent *event) {
  cout << "motion : " << event->x() << "," << event->y() << endl;
}

void GLView::mouseReleaseEvent(QMouseEvent *event) {
  switch(event->button()){
  case Qt::LeftButton:
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
    _chaikin = Chaikin();
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


void GLView::drawChoice0() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    _chaikin.drawCurves();
    glPopMatrix();
}

void GLView::drawChoice1() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    _chaikin.drawClosedCurves();
    glPopMatrix();
}

void GLView::drawChoice2() {
    if(_lastChoice==0) _chaikin.approximate();
    else if(_lastChoice==1) _chaikin.interpolate();
    _choice=_lastChoice;
}
void GLView::drawChoice3() {
    _chaikin.initializeCurves();
    _choice=_lastChoice;
}

void GLView::paintGL() {
  /// clears the window
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  /// choice example
  switch(_choice) {
    case 0:
      drawChoice0();
      break;
    case 1:
      drawChoice1();
      break;
    case 2:
      drawChoice2();
      break;
    case 3:
      drawChoice3();
      break;
  }

}

/** ********************************************************************** **/

void GLView::choice(int i,const string &s) {
  // i = button number, s = button text
  cout << "choice " << i << " " << s << endl;
  _lastChoice=_choice;
  _choice=i;
  _choiceText=s;
}
