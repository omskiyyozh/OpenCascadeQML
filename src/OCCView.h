#ifndef OCCView_H
#define OCCView_H
#include <windows.h>
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <QMutex.h>
#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickWindow.h>
#include <TopoDS_Shape.hxx>
#include <WNT_Window.hxx>


#include <AIS_InteractiveContext.hxx>
#include <Graphic3d_Vec2.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

#include <Aspect_DisplayConnection.hxx>
#include <Aspect_Drawable.hxx>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
//! [1]
class OCCTRenderer : public QObject {
  Q_OBJECT
public:
  OCCTRenderer();
  ~OCCTRenderer();

  void initViewer(Aspect_Drawable drawable);
  void setViewportSize(const QSize &size) { m_viewportSize = size; }
  void setWindow(QQuickWindow *window) { m_window = window; }
  void paint();
  void createDemoScene();
  Handle(V3d_Viewer) getViewer();
public slots:
  void init();

private:
  QSize m_viewportSize;
  QQuickWindow *m_window;
  QMutex m_mutex;
  Handle(V3d_Viewer) m_viewer{nullptr};
  Handle(V3d_View) m_view{nullptr};
  Handle(AIS_InteractiveContext) m_context{nullptr};

private:
  // save the mouse position.
  Standard_Integer myXmin;
  Standard_Integer myYmin;
  Standard_Integer myXmax;
  Standard_Integer myYmax;

  // mouse actions.
  enum CurrentAction3d {
    CurAction3d_Nothing,
    CurAction3d_DynamicZooming,
    CurAction3d_WindowZooming,
    CurAction3d_DynamicPanning,
    CurAction3d_GlobalPanning,
    CurAction3d_DynamicRotation
  };

  // the mouse current mode.
  CurrentAction3d myCurrentMode{CurAction3d_DynamicRotation};

  // CurrentAction3d myCurrentMode {CurAction3d_DynamicPanning};
public:
  // Mouse
  void leftButtonDown(const int &x, const int &y);
  void middleButtonDown(const int &x, const int &y);
  void rightButtonDown(){};
  void leftButtonUp(){};
  void middleButtonUp(){};
  void rightButtonUp(){};
  void wheelMove(const int &x, const int &y, const int &delta);
  void mMove(const int &x, const int &y);

signals:
  void occViewChanged();

public slots:
  void rotationTheScene();
  void movementTheScene();
};
//! [1]

//! [2]
class OCCT : public QQuickItem {
  Q_OBJECT
  QML_ELEMENT

public:
  OCCT(QQuickItem *parent = nullptr);

public slots:
  void sync();
  void cleanup();

private slots:
  void handleWindowChanged(QQuickWindow *win);
  void emitChanged();
signals:
  void occViewChanged();

public slots:
  // slots for mouse events.
  void mousePress(const int x, const int y, const int buttonsFlag);
  void mouseRelease(const int x, const int y, const int buttonsFlag);
  void mouseWheel(const int x, const int y, const int delta,
                  const int buttonsFlag);
  void mouseMove(const int x, const int y, const int buttonsFlag);

private:
  void releaseResources() override;

  OCCTRenderer *m_renderer;
};

#endif // OCCView_H
