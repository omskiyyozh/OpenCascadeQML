
#include "OCCView.h"

#include <QtQuick/qquickwindow.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QtCore/QRunnable>
#include <BRepPrimAPI_MakeSphere.hxx>
//! [7]
OCCT::OCCT(QQuickItem* parent)
    : QQuickItem(parent)
    , m_renderer(nullptr)
{
    connect(this, &QQuickItem::windowChanged, this, &OCCT::handleWindowChanged);
}
//! [7]

//! [8]
void OCCT::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
}
//! [8]

//! [1]
void OCCT::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &OCCT::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &OCCT::cleanup, Qt::DirectConnection);
//! [1]
//! [3]
        // Ensure we start with cleared to black. The OCCT's blend mode relies on this.
        win->setColor(Qt::black);
    }
}
//! [3]

//! [6]
void OCCT::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
}

class CleanupJob : public QRunnable
{
public:
    CleanupJob(OCCTRenderer *renderer) : m_renderer(renderer) { }
    void run() override { delete m_renderer; }
private:
    OCCTRenderer *m_renderer;
};

void OCCT::releaseResources()
{
    window()->scheduleRenderJob(new CleanupJob(m_renderer), QQuickWindow::BeforeSynchronizingStage);
    m_renderer = nullptr;
}

OCCTRenderer::OCCTRenderer(): m_viewer(nullptr), m_view(nullptr)
{

}

OCCTRenderer::~OCCTRenderer()
{
    m_view.Nullify();
        m_context.Nullify();
        m_viewer.Nullify();
}

void OCCTRenderer::initViewer(Aspect_Drawable drawable)
{
    if (!m_viewer) {
    auto deviceContext = wglGetCurrentDC();
    auto renderContext = wglGetCurrentContext();

        if (deviceContext == nullptr || renderContext == nullptr)
            return;

        // Setup display driver.
        Handle(Aspect_DisplayConnection) display;
        Handle(OpenGl_GraphicDriver) driver = new OpenGl_GraphicDriver(display,Standard_False);
        driver->ChangeOptions().buffersNoSwap = Standard_True;
        driver->ChangeOptions().buffersOpaqueAlpha = Standard_True;
        driver->ChangeOptions().sRGBDisable = Standard_True;
        //driver->ChangeOptions().ffpEnable = Standard_True;

     //   driver->CreateRendersWindow(window,wglGetCurrentContext());


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


        m_view->SetImmediateUpdate(Standard_False);
        m_view->SetWindow(window, reinterpret_cast<Aspect_RenderingContext>(wglGetCurrentContext()));

        //m_view->SetWindow(window);
      //  m_view->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_WHITESMOKE, 0.2, V3d_ZBUFFER);}
}}
//! [6]

//! [9]
void OCCT::sync()
{

    if (!m_renderer) {
        m_renderer = new OCCTRenderer();
        m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
        m_renderer->setWindow(window());
      //  Aspect_Drawable drawable (Aspect_Drawable (window()->winId()));
      //  m_renderer->initViewer(Aspect_Drawable (window()->winId()));
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &OCCTRenderer::init, Qt::DirectConnection);

     //  connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &OCCTRenderer::paint, Qt::DirectConnection);
    }
    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setT(m_t);
    m_renderer->setWindow(window());


}
//! [9]

//! [4]
void OCCTRenderer::init()
{
   // m_window->beginExternalCommands();
    Q_ASSERT(m_viewer.IsNull());

    if (!m_window->winId())
        return;

    // Setup display driver.
        Handle(Aspect_DisplayConnection) display;
        Handle(OpenGl_GraphicDriver) driver = new OpenGl_GraphicDriver(display,Standard_False);
        driver->ChangeOptions().buffersNoSwap = Standard_True;
        driver->ChangeOptions().buffersOpaqueAlpha = Standard_True;
        driver->ChangeOptions().sRGBDisable = Standard_True;
        //driver->ChangeOptions().ffpEnable = Standard_True;




        // Create and setup the viewer.
        m_viewer = new V3d_Viewer(driver);
        m_viewer->SetDefaultBackgroundColor(Quantity_NOC_GRAY50);
        m_viewer->SetDefaultLights();
        m_viewer->SetLightOn();

        // Create and setup interactivity context.
        m_context = new AIS_InteractiveContext(m_viewer);
        m_context->SetDisplayMode(AIS_Shaded, true);

        // Create and setup view.
        Handle(WNT_Window) window = new WNT_Window(Aspect_Drawable(m_window->winId()));
        m_view = m_viewer->CreateView();
   //    driver->CreateRenderWindow(window,wglGetCurrentContext());

        m_view->SetImmediateUpdate(Standard_False);
        m_view->SetWindow(window, reinterpret_cast<Aspect_RenderingContext>(wglGetCurrentContext()));

        //m_view->SetWindow(window);
        m_view->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_WHITESMOKE, 0.2, V3d_ZBUFFER);



    // Create a demo scene.
    this->createDemoScene();

       // m_window->endExternalCommands();
}

void OCCTRenderer::createDemoScene()
{
   m_window->beginExternalCommands();
    // Create a bisque cone at [0, 10, 0].
    gp_Ax2 axis;
    axis.SetLocation(gp_Pnt(0.0, 10.0, 0.0));

    TopoDS_Shape bisqueCone = BRepPrimAPI_MakeCone(axis, 3.0, 1.5, 5.0).Shape();
    Handle(AIS_Shape) bisqueConeShape = new AIS_Shape(bisqueCone);
    bisqueConeShape->SetColor(Quantity_NOC_BISQUE);

    // Create a chocolate cone at [8, 10, 0].
    axis.SetLocation(gp_Pnt(8.0, 10.0, 0.0));
    TopoDS_Shape chocoCone = BRepPrimAPI_MakeCone(axis, 3.0, 0.0, 5.0).Shape();
    Handle(AIS_Shape) chocoConeShape = new AIS_Shape(chocoCone);
    chocoConeShape->SetColor(Quantity_NOC_CHOCOLATE);

    //Create sphere
    Handle_AIS_Shape aSphere = new AIS_Shape(BRepPrimAPI_MakeSphere(gp_Pnt(10,20,10),4));

    m_context->Display(bisqueConeShape, Standard_True);
    m_context->Display(chocoConeShape, Standard_True);
    m_context->Display(aSphere, Standard_True);

    // Fit all into the view.
    m_view->FitAll();
   m_window->endExternalCommands();
}


//! [4] //! [5]
void OCCTRenderer::paint()
{
//    m_view->SetImmediateUpdate(Standard_False);
//           m_view->SetWindow(window, reinterpret_cast<Aspect_RenderingContext>(renderContext));
//           m_view->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_WHITESMOKE, 0.1, V3d_ZBUFFER);
    gp_Ax2 axis;
        axis.SetLocation(gp_Pnt(0.0, 10.0, 0.0));

        TopoDS_Shape bisqueCone = BRepPrimAPI_MakeCone(axis, 3.0, 1.5, 5.0).Shape();
        Handle(AIS_Shape) bisqueConeShape = new AIS_Shape(bisqueCone);
        bisqueConeShape->SetColor(Quantity_NOC_BISQUE);

        // Create a chocolate cone at [8, 10, 0].
        axis.SetLocation(gp_Pnt(8.0, 10.0, 0.0));
        TopoDS_Shape chocoCone = BRepPrimAPI_MakeCone(axis, 3.0, 0.0, 5.0).Shape();
        Handle(AIS_Shape) chocoConeShape = new AIS_Shape(chocoCone);
        chocoConeShape->SetColor(Quantity_NOC_CHOCOLATE);

        m_context->Display(bisqueConeShape, Standard_True);
        m_context->Display(chocoConeShape, Standard_True);

        // Fit all into the view.
        m_view->FitAll();
}
//! [5]
