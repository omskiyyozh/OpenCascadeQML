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

//Renderer class
class OCCView : public QQuickItem
{
    Q_OBJECT

    // Member fields.
private:
    Handle(V3d_Viewer)				m_viewer;
    Handle(V3d_View)				m_view;
    Handle(AIS_InteractiveContext)	m_context;

    QSize							m_viewportSize;
    QPoint							m_viewportPos;
    QMutex							m_mutex;

    // Properties.
public:

    // Constructor/Destructor.
public:
    explicit OCCView(QQuickItem* parent = nullptr);

    // Signals.
signals:

    // Slots.
private slots:
    void onWindowChanged(QQuickWindow* window);

public slots:
    void onSynchronizing();
    void onInvalidating();
    void onRendering();

    // Public interface.
public:

    // Protected interface.
protected:
    void initializeViewer(const Aspect_Drawable& drawable);

    // Private interface.
private:
    void createDemoScene();

    // Static interface.
public:
};

#endif // OCCView_H
