#ifndef OCCView_H
#define OCCView_H

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

class OCCRenderer : public QObject
{
Q_OBJECT;
public:
OCCRenderer();
~OCCRenderer() {}

public slots:


    void onRendering();

public:
    void initializeViewer(const Aspect_Drawable& drawable);
    void setViewPortSize(const QSize &size, const QPoint &pos);
    // Private interface.
private:
    void createDemoScene();

private:
    Handle(V3d_Viewer)				m_viewer;
    Handle(V3d_View)				m_view;
    Handle(AIS_InteractiveContext)	m_context;

    QSize							m_viewportSize;
    QPoint							m_viewportPos;
    QMutex							m_mutex;
};

//Renderer class
class OCCView : public QQuickItem
{
    Q_OBJECT

    // Member fields.
private:

OCCRenderer *renderer;
    // Properties.
public:

    // Constructor/Destructor.
public:
    explicit OCCView(QQuickItem* parent = nullptr);

    // Signals.
signals:

    // Slots.
private slots:
    void onSynchronizing();
    void onWindowChanged(QQuickWindow* window);
 //    void onInvalidating();

    // Static interface.
public:
};

#endif // OCCView_H
