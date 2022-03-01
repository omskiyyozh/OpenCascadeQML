#ifndef OCCView_H
#define OCCView_H
#include <windows.h>
#include <WNT_Window.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <AIS_Shape.hxx>
#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickWindow.h>
#include <QMutex.h>

#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Graphic3d_Vec2.hxx>

#include <Aspect_Drawable.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
//! [1]
class OCCTRenderer : public QObject
{
    Q_OBJECT
public:
    OCCTRenderer();
    ~OCCTRenderer();

    void initViewer (Aspect_Drawable drawable);
    void setT(qreal t) { m_t = t; }
    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }
    void paint();
    void createDemoScene();
public slots:
    void init();


private:
    QSize m_viewportSize;
    qreal m_t;
    QQuickWindow *m_window;
     QMutex                         m_mutex;
    Handle(V3d_Viewer)				m_viewer { nullptr };
    Handle(V3d_View)				m_view { nullptr };
    Handle(AIS_InteractiveContext)	m_context { nullptr };
};
//! [1]

//! [2]
class OCCT : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    QML_ELEMENT

public:
    OCCT(QQuickItem* parent = nullptr);

    qreal t() const { return m_t; }
    void setT(qreal t);

signals:
    void tChanged();

public slots:
    void sync();
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    void releaseResources() override;

    qreal m_t;
    OCCTRenderer *m_renderer;
};

#endif // OCCView_H
