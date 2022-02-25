#include <windows.h>
#include <WNT_Window.hxx>

#include "OCCView.h"

OCCRenderer::OCCRenderer() : m_view(nullptr), m_viewer(nullptr), m_context(nullptr)
{

}

#include <BRepPrimAPI_MakeCone.hxx>
#include <AIS_Shape.hxx>

OCCView::OCCView(QQuickItem* parent) :
    QQuickItem(parent), renderer (nullptr)
{
    QObject::connect(this, &QQuickItem::windowChanged, this, &OCCView::onWindowChanged, Qt::DirectConnection);
}

void OCCView::onWindowChanged(QQuickWindow* window)
{
    if (!window)
        return;


    QObject::connect(window, &QQuickWindow::beforeSynchronizing, this, &OCCView::onSynchronizing, Qt::DirectConnection);
  //  QObject::connect(window, &QQuickWindow::sceneGraphInvalidated, this, &OCCView::onInvalidating, Qt::DirectConnection);

    //window->setClearBeforeRendering(false);

}

void OCCView::onSynchronizing()
{
    // If the viewer is not yet initialized, initialize it.
    if (!renderer)
    { //засунь в рендер винайди параметром, инциализацию не вызывать в гуишном потоке
        renderer = new OCCRenderer();
        WId a = window()->winId();
        Aspect_Drawable drawable = Aspect_Drawable(&a);
        renderer->initializeViewer(drawable);
        QObject::connect(this->window(), &QQuickWindow::beforeRendering, renderer, &OCCRenderer::onRendering, Qt::DirectConnection);
    }
   //renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
//   renderer->setWindow(window());
//    // Get the control position and size.
    QPoint viewportPos = this->mapToGlobal(QPointF(0, 0)).toPoint();
    QSize viewportSize = this->size().toSize();
    renderer->setViewPortSize(viewportSize, viewportPos);
//    // Check if the viewport needs to be resized.
//    if (viewportPos.x() != m_viewportPos.x() || viewportPos.y() != m_viewportPos.y())
//        m_view->MustBeResized();
//    if (viewportSize.width() != m_viewportSize.width() || viewportSize.height() != m_viewportSize.height())
//    {
//        m_view->MustBeResized();
//        m_view->Invalidate();
//    }

//    // Store the current pos and size.
//    m_viewportPos = viewportPos;
//    m_viewportSize = viewportSize;
}

//void OCCRenderer::onInvalidating()
//{
//    m_view.Nullify();
//    m_context.Nullify();
//    m_viewer.Nullify();
//}

void OCCRenderer::onRendering()
{
    if (m_view.IsNull())
        return;

    m_mutex.lock();
    // TODO: Handle user input.
    m_mutex.unlock();

    // Redraw the view.
    // NOTE: MustBeResized is called here, due to a bug that causes the background to
    //       go white, when hovering a MenuBar (and possibly other top-level QML items).
    m_view->Redraw();
    m_view->MustBeResized();
}

#include <QOpenGLContext>
void OCCRenderer::initializeViewer(const Aspect_Drawable& drawable)
{
    Q_ASSERT(m_viewer.IsNull());

    // Request device and render context.
    auto deviceContext = wglGetCurrentDC();
    auto renderContext = wglGetCurrentContext();

    if (drawable == nullptr || deviceContext == nullptr || renderContext == nullptr)
        return;

    // Setup display driver.
    Handle(Aspect_DisplayConnection) display = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) driver = new OpenGl_GraphicDriver(display, Standard_False);
    driver->ChangeOptions().buffersNoSwap = Standard_True;
    //aDriver->ChangeOptions().glslWarnings  = Standard_True;

    // Create and setup the viewer.
    m_viewer = new V3d_Viewer(driver);
    m_viewer->SetDefaultBackgroundColor(Quantity_NOC_GRAY50);
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();

    // Create and setup interactivity context.
    m_context = new AIS_InteractiveContext(m_viewer);
    m_context->SetDisplayMode(AIS_Shaded, false);

    // Create and setup view.
    Handle(WNT_Window) window = new WNT_Window(drawable);
    m_view = m_viewer->CreateView();

    m_view->SetImmediateUpdate(Standard_False);
    m_view->SetWindow(window, reinterpret_cast<Aspect_RenderingContext>(renderContext));
    m_view->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_WHITESMOKE, 0.1, V3d_ZBUFFER);

    // Create a demo scene.
    this->createDemoScene();
}

void OCCRenderer::setViewPortSize(const QSize &size, const QPoint &pos)
{
        if (pos.x() != m_viewportPos.x() || pos.y() != m_viewportPos.y())
            m_view->MustBeResized();
        if (size.width() != m_viewportSize.width() || size.height() != m_viewportSize.height())
        {
            m_view->MustBeResized();
            m_view->Invalidate();
        }

        // Store the current pos and size.
        m_viewportPos = pos;
        m_viewportSize = size;

}

//void OCCRenderer::setViewPort()
//{
//    // Get the control position and size.
//    QPoint viewportPos = this->mapToGlobal(QPointF(0, 0)).toPoint();
//    QSize viewportSize = this->size().toSize();

//    // Check if the viewport needs to be resized.
//    if (viewportPos.x() != m_viewportPos.x() || viewportPos.y() != m_viewportPos.y())
//        m_view->MustBeResized();
//    if (viewportSize.width() != m_viewportSize.width() || viewportSize.height() != m_viewportSize.height())
//    {
//        m_view->MustBeResized();
//        m_view->Invalidate();
//    }

//    // Store the current pos and size.
//    m_viewportPos = viewportPos;
//    m_viewportSize = viewportSize;
//}

void OCCRenderer::createDemoScene()
{
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

    m_context->Display(bisqueConeShape, Standard_True);
    m_context->Display(chocoConeShape, Standard_True);

    // Fit all into the view.
    m_view->FitAll();
}

