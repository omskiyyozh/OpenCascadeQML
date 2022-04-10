
#include "OCCView.h"

#include <BRepPrimAPI_MakeSphere.hxx>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QtCore/QRunnable>
#include <QtQuick/qquickwindow.h>

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

void OCCT::releaseResources() {
  m_renderer = nullptr;
}

OCCTRenderer::OCCTRenderer() : m_viewer(nullptr), m_view(nullptr), myCurrentMode(CurAction3d_Nothing) {}

OCCTRenderer::~OCCTRenderer() {
  m_view.Nullify();
  m_context.Nullify();
  m_viewer.Nullify();
}

void OCCTRenderer::initViewer(Aspect_Drawable drawable) {
    Q_ASSERT(m_viewer.IsNull());

    if (drawable == nullptr)
        return;

    Handle(Aspect_DisplayConnection) display;
    Handle(OpenGl_GraphicDriver) driver = new OpenGl_GraphicDriver(display,Standard_False);
    driver->ChangeOptions().buffersNoSwap = Standard_True;
    driver->ChangeOptions().buffersOpaqueAlpha = Standard_True;
    driver->ChangeOptions().sRGBDisable = Standard_False;
    driver->ChangeOptions().ffpEnable = Standard_True;

    m_viewer = new V3d_Viewer(driver);

    m_viewer->SetDefaultBackgroundColor(Quantity_NOC_GRAY50);
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();
    m_viewer->SetComputedMode(Standard_True);
    m_viewer->SetDefaultComputedMode(Standard_True);

    m_context = new AIS_InteractiveContext(m_viewer);
    m_context->SetDisplayMode(AIS_Shaded, true);

    Handle(WNT_Window) window = new WNT_Window(drawable);
    m_view = m_viewer->CreateView();


    m_view->SetImmediateUpdate(Standard_True);
    m_view->SetWindow(window, reinterpret_cast<Aspect_RenderingContext>(wglGetCurrentContext()));

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

    objects.push_back(bisqueConeShape);
    objects.push_back(chocoConeShape);
    objects.push_back (aSphere);
}

void OCCT::sync() {

  if (!m_renderer) {
    m_renderer = new OCCTRenderer();
      m_renderer->setWindow(window());
    connect(m_renderer, &OCCTRenderer::occViewChanged, this,
            &OCCT::emitChanged);

    m_renderer->initViewer(Aspect_Drawable(window()->winId()));
    connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer,
            &OCCTRenderer::init, Qt::DirectConnection);
  }
  m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
  m_renderer->setWindow(window());
}

void OCCTRenderer::init() {
    m_view->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_WHITESMOKE, 0.2, V3d_ZBUFFER);
    this->createDemoScene();
}

void OCCTRenderer::createDemoScene() {
 m_window->beginExternalCommands();

    if (myCurrentMode == 0) {
 for (auto object: objects) {
     m_context->Display(object, Standard_True);
 }
 m_view->FitAll();}
    else {
        m_view->RedrawImmediate();
        m_view->Redraw();}
 m_window->endExternalCommands();
}

Handle(V3d_Viewer) OCCTRenderer::getViewer()
{
    return m_viewer;
}

void OCCTRenderer::paint() {
    m_view->FitAll();
  m_view->Redraw();
}

void OCCTRenderer::rotationTheScene() {
  myCurrentMode = CurAction3d_DynamicRotation;
}

void OCCTRenderer::movementTheScene() {
  myCurrentMode = CurAction3d_DynamicPanning;
}

void OCCTRenderer::leftButtonDown(const int &x, const int &y) {
  myXmin = x;
  myYmin = y;
  myXmax = x;
  myYmax = y;
}

void OCCTRenderer::middleButtonDown(const int &x, const int &y) {
  myXmin = x;
  myYmin = y;
  myXmax = x;
  myYmax = y;

  if (myCurrentMode == CurAction3d_DynamicRotation) {
    m_view->StartRotation(x, y);
  }
}

void OCCTRenderer::wheelMove(const int &x, const int &y, const int &delta) {
    myCurrentMode = CurAction3d_DynamicZooming;
    Standard_Integer aFactor = 5;

    Standard_Integer aX = x;
    Standard_Integer aY = y;

    if (delta > 0) {
        aX += aFactor;
        aY += aFactor;
    } else {
        aX -= aFactor;
        aY -= aFactor;
    }

    m_view->Zoom(x, y, aX, aY);
    emit occViewChanged();
}

void OCCTRenderer::mMove(const int &x, const int &y) {
    myCurrentMode = CurAction3d_DynamicRotation;
    m_view->Rotation(x, y);
    emit occViewChanged();
}

void OCCT::mousePress(int x, int y, int buttonsFlag) {
  if (buttonsFlag == Qt::LeftButton) {
    m_renderer->leftButtonDown(x, y);
  } else if (buttonsFlag == Qt::MiddleButton) {
    m_renderer->middleButtonDown(x, y);
  } else if (buttonsFlag == Qt::RightButton) {
    m_renderer->rightButtonDown();
  }
}

void OCCT::mouseRelease(int x, int y, int buttonsFlag) {

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
  }
}

void OCCT::mouseMove(const int x, const int y, const int buttonsFlag) {
  if (buttonsFlag == Qt::MiddleButton) {
    m_renderer->mMove(x, y);
  }
}
