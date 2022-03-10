
#include "OCCView.h"

#include <BRepPrimAPI_MakeSphere.hxx>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QtCore/QRunnable>
#include <QtQuick/qquickwindow.h>
//! [7]
OCCT::OCCT(QQuickItem *parent) : QQuickItem(parent), m_renderer(nullptr) {
  connect(this, &QQuickItem::windowChanged, this, &OCCT::handleWindowChanged);
}

void OCCT::handleWindowChanged(QQuickWindow *win) {
  if (win) {
    connect(win, &QQuickWindow::beforeSynchronizing, this, &OCCT::sync,
            Qt::DirectConnection);
    connect(win, &QQuickWindow::sceneGraphInvalidated, this, &OCCT::cleanup,
            Qt::DirectConnection);
    win->setColor(Qt::black);
  }
}

void OCCT::emitChanged() { emit occViewChanged(); }

void OCCT::cleanup() {
  delete m_renderer;
  m_renderer = nullptr;
}

//class CleanupJob : public QRunnable {
//public:
//  CleanupJob(OCCTRenderer *renderer) : m_renderer(renderer) {}
//  void run() override { delete m_renderer; }

//private:
//  OCCTRenderer *m_renderer;
//};

void OCCT::releaseResources() {
//  window()->scheduleRenderJob(new CleanupJob(m_renderer),
//                              QQuickWindow::BeforeSynchronizingStage);
  m_renderer = nullptr;
}

OCCTRenderer::OCCTRenderer() : m_viewer(nullptr), m_view(nullptr) {}

OCCTRenderer::~OCCTRenderer() {
  m_view.Nullify();
  m_context.Nullify();
  m_viewer.Nullify();
}

void OCCTRenderer::initViewer(Aspect_Drawable drawable) {
    Q_ASSERT(m_viewer.IsNull());

    if (drawable == nullptr)
        return;

    // Setup display driver.
    Handle(Aspect_DisplayConnection) display;
    Handle(OpenGl_GraphicDriver) driver = new OpenGl_GraphicDriver(display,Standard_False);
    //driver->EnableVBO(Standard_True);
    driver->ChangeOptions().buffersNoSwap = Standard_False;
    driver->ChangeOptions().buffersOpaqueAlpha = Standard_True;
    driver->ChangeOptions().sRGBDisable = Standard_False;
    driver->ChangeOptions().ffpEnable = Standard_True;


    // Create and setup the viewer.
    m_viewer = new V3d_Viewer(driver);
    m_viewer->SetDefaultBackgroundColor(Quantity_NOC_GRAY50);
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();

    // Create and setup interactivity context.
    m_context = new AIS_InteractiveContext(m_viewer);
    m_context->SetDisplayMode(AIS_Shaded, true);

    // Create and setup view.
    Handle(WNT_Window) window = new WNT_Window(drawable);
    m_view = m_viewer->CreateView();

    //driver->CreateRenderWindow(window,wglGetCurrentContext());
    m_view->SetImmediateUpdate(Standard_False);
    m_view->SetWindow(window, reinterpret_cast<Aspect_RenderingContext>(wglGetCurrentContext()));

//    //m_view->SetWindow(window);
//    m_view->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_WHITESMOKE, 0.2, V3d_ZBUFFER);


//    // Create a demo scene.
    this->createDemoScene();
}

void OCCT::sync() {

  if (!m_renderer) {
    m_renderer = new OCCTRenderer();
      m_renderer->setWindow(window());
    connect(m_renderer, &OCCTRenderer::occViewChanged, this,
            &OCCT::emitChanged);


  //  m_renderer->setViewportSize(window()->size() *
   //                             window()->devicePixelRatio());
   // m_renderer->setWindow(window());

    m_renderer->initViewer(Aspect_Drawable(window()->winId()));
   //  connect(window(), &QQuickWindow::beforeRendering, m_renderer,
   //        &OCCTRenderer::init, Qt::DirectConnection);
    connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer,
            &OCCTRenderer::paint, Qt::DirectConnection);
    connect(window(), &QQuickWindow::afterRendering, m_renderer,
           &OCCTRenderer::init, Qt::DirectConnection);
  }
      //  connect(window(), &QQuickWindow::afterRenderPassRecording, m_renderer,
      //          &OCCTRenderer::init, Qt::DirectConnection);

  m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
  m_renderer->setWindow(window());
}

void OCCTRenderer::init() {
    //if (m_context.) {
    //m_view->SetWindow(window);
    m_view->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_WHITESMOKE, 0.2, V3d_ZBUFFER);
//    Handle_AIS_Shape aSphere =
//        new AIS_Shape(BRepPrimAPI_MakeSphere(gp_Pnt(10, 20, 10), 4));
//    m_context->DisplayAll(Standard_True);
    // Create a demo scene.
    this->createDemoScene(); //}
    //else
 //  m_view->Redraw();
}

void OCCTRenderer::createDemoScene() {
 m_window->beginExternalCommands();

  gp_Ax2 axis;
  axis.SetLocation(gp_Pnt(0.0, 10.0, 0.0));

  TopoDS_Shape bisqueCone = BRepPrimAPI_MakeCone(axis, 3.0, 1.5, 5.0).Shape();
  Handle(AIS_Shape) bisqueConeShape = new AIS_Shape(bisqueCone);
  bisqueConeShape->SetColor(Quantity_NOC_BISQUE);

  axis.SetLocation(gp_Pnt(8.0, 10.0, 0.0));
  TopoDS_Shape chocoCone = BRepPrimAPI_MakeCone(axis, 3.0, 0.0, 5.0).Shape();
  Handle(AIS_Shape) chocoConeShape = new AIS_Shape(chocoCone);
  chocoConeShape->SetColor(Quantity_NOC_CHOCOLATE);

  Handle_AIS_Shape aSphere =
      new AIS_Shape(BRepPrimAPI_MakeSphere(gp_Pnt(10, 20, 10), 4));

  m_context->Display(bisqueConeShape, Standard_True);
  m_context->Display(chocoConeShape, Standard_True);
  m_context->Display(aSphere, Standard_True);

  m_view->FitAll();
 m_window->endExternalCommands();
}

Handle(V3d_Viewer) OCCTRenderer::getViewer()
{
    return m_viewer;
}

void OCCTRenderer::paint() {
 // m_view->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_WHITESMOKE, 0.2,
 //                         V3d_ZBUFFER);
 // this->createDemoScene();
    //   m_view->FitAll();
      m_view->Redraw();
}

void OCCTRenderer::rotationTheScene() {
  myCurrentMode = CurAction3d_DynamicRotation;
}

void OCCTRenderer::movementTheScene() {
  myCurrentMode = CurAction3d_DynamicPanning;
}

void OCCTRenderer::leftButtonDown(const int &x, const int &y) {
  // Save the current mouse coordinate in min.
  myXmin = x;
  myYmin = y;
  myXmax = x;
  myYmax = y;
}

void OCCTRenderer::middleButtonDown(const int &x, const int &y) {
  // Save the current mouse coordinate in min.
  myXmin = x;
  myYmin = y;
  myXmax = x;
  myYmax = y;

  if (myCurrentMode == CurAction3d_DynamicRotation) {
    m_view->StartRotation(x, y);
  }
}

void OCCTRenderer::wheelMove(const int &x, const int &y, const int &delta) {
    Standard_Integer aFactor = 5;

    Standard_Integer aX = x;
    Standard_Integer aY = y;


    if (delta > 0)
    {
        aX += aFactor;
        aY += aFactor;
    }
    else
    {
        aX -= aFactor;
        aY -= aFactor;
    }

    m_view->Zoom(x, y, aX, aY);

    emit occViewChanged();
}

void OCCTRenderer::mMove(const int &x, const int &y) {
  switch (myCurrentMode) {
  case CurAction3d_DynamicRotation:
    m_view->Rotation(x, y);
    emit occViewChanged();
    break;

  case CurAction3d_DynamicZooming:
    m_view->Zoom(myXmin, myYmin, x, y);
    emit occViewChanged();
    break;

  case CurAction3d_DynamicPanning:
    m_view->Pan(x - myXmax, myYmax - y);
    myXmax = x;
    myYmax = y;
    emit occViewChanged();
    break;

  default:
    break;
  }
}

void OCCT::mousePress(int x, int y, int buttonsFlag) {
  if (buttonsFlag == Qt::LeftButton) {
    m_renderer->leftButtonDown(x, y);
    // emit occViewChanged();
  } else if (buttonsFlag == Qt::MiddleButton) {
    m_renderer->middleButtonDown(x, y);
    // emit occViewChanged();
  } else if (buttonsFlag == Qt::RightButton) {
    m_renderer->rightButtonDown();
    // emit occViewChanged();
  }
}

void OCCT::mouseRelease(int x, int y, int buttonsFlag) {
  // Q_UNUSED(x)
  // Q_UNUSED(y)
  if (buttonsFlag == Qt::LeftButton) {
    m_renderer->leftButtonUp();
  } else if (buttonsFlag == Qt::MiddleButton) {
    m_renderer->middleButtonDown(x, y);
  } else if (buttonsFlag == Qt::RightButton) {
    m_renderer->rightButtonUp();
  }
}

void OCCT::mouseWheel(const int x, const int y, const int delta,
                      const int buttonsFlag) {
   if (buttonsFlag == Qt::NoButton)
  {
    m_renderer->wheelMove(x, y, delta);
    emit occViewChanged();
  }
}

void OCCT::mouseMove(const int x, const int y, const int buttonsFlag) {
  if (buttonsFlag == Qt::MiddleButton) {
    m_renderer->mMove(x, y);
    // emit occViewChanged();
  }
}
