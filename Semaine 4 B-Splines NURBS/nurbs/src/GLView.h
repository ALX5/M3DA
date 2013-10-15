/**
*
*  @author F. Aubert
*  @file GLView
*
*/



#ifndef GLVIEW_H
#define GLVIEW_H

#include "glsupport.h"

#include "UtilGL.h"
#include <QtEvents>

#include "nurbs.h"


/**
  @class GLView

  The widget to draw with OpenGL.
  */
class GLView : public QGLWidget {
  Q_OBJECT
public:
  //! Ctor
  explicit GLView(QWidget *parent = 0);
  //! Dtor
  virtual ~GLView();

  //! initialize data application
  void initData();

  // Degré.
  int _p;

  // Nombre de points de contrôles.
  int _m;

  Nurbs* _nurbs;

  int _choice;


protected:
  // OpenGL drawing :
  //! initialize the OpenGL context
  void initializeGL();
  //! OpenGL drawing (called when updateGL() )
  void paintGL();
  //! should update projection (called when the widget is resized)
  void resizeGL(int width,int height);

  // events :
  //! called when a mouse button is pressed in the widget
  void mousePressEvent(QMouseEvent *event);
  //! called when the mouse is moved in the widget
  void mouseMoveEvent(QMouseEvent *event);
  //! called when a mouse button is released in the widget
  void mouseReleaseEvent(QMouseEvent *event);
  //! called when a key is pressed in the widget
  void keyPressEvent( QKeyEvent *event );
  //! called when a key is released in the widget
  void keyReleaseEvent( QKeyEvent *event );
  //! called when the mouse wheel is turned
  void wheelEvent(QWheelEvent *event);


private:

signals:

public slots:
  //! called every 20ms by the MainWindow
  void updateData();

  void choice1();
  void choice2();
  void choice3();
  void choice4();
  void choice5();
};

#endif // GLVIEW_H
