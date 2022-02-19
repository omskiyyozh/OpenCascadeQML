#pragma once
#include <QtQuick/QQuickItem>
#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Graphic3d_Vec2.hxx>

#include <Aspect_Drawable.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <V3d_View.hxx>

#include <Aspect_Handle.hxx>

class OCCView: public QQuickItem {
public:
    explicit OCCView();
    ~OCCView ();

public slots:
    void resize();
    void zoom();
    void rotate();
    void pan();
private:
Handle(V3d_Viewer) m_viewer;
Handle(V3d_View) m_view;
Handle(AIS_InteractiveContext) m_context;

};
