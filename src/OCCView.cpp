#include "OCCView.h"

OCCView::OCCView()
{
  Handle(Aspect_DisplayConnection) display;
  Handle(OpenGL_GraphicsDriver) driver = new OpenGL_GraphicDriver(display, Standard_False);

  m_viewer = new V3d_Viewer(driver);
  m_viewer->SetDefaultLights();
  m_viewer->SetLightsOn();
  m_context = new AIS_InteractiveContext(m_viewer);
  m_context->SetDisplayMode(AIS_Shaded, false);
  m_view = m_viewer->CreateView();
  Aspect_Handle window_handle = (Aspect_Handle)winId();
  Handle(WNT_Window) window = new WNT_Window(window_handle);
  m_view->SetWindow(window);
  m_context->SetDisplayMode(AIS_Shaded,true);
}
