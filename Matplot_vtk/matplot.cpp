#ifdef LEMMAUSEVTK

#include "matplot.h"

#include "vtkActor.h"
#include "vtkAxesActor.h"
#include "vtkCamera.h"
#include "vtkCaptionActor2D.h"
#include "vtkContourFilter.h"
#include "vtkDataSetMapper.h"
#include "vtkGlyph3D.h"
#include "vtkGlyphSource2D.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPNGWriter.h"
#include "vtkProperty.h"
#include "vtkProperty2D.h"
#include "vtkRectilinearGridGeometryFilter.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkStructuredGridGeometryFilter.h"
#include "vtkScalarBarActor.h"
#include "vtkTextProperty.h"
#include "vtkTubeFilter.h"
#include "vtkWarpScalar.h"
#include "vtkWindowToImageFilter.h"
#include "vtkRenderLargeImage.h"

#include "vtkTextActor.h"
#include "vtkVectorText.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTransform.h"
#include "vtkFollower.h"

#include "vtkAxisActor2D.h"
//#include "vtkCubeAxesActor.h"
#include "vtkCubeAxesActor2D.h"

using namespace matplot;

/// Default constructor
Plot2D_VTK::Plot2D_VTK(std::string x_label, std::string y_label,
		       int xpix, int ypix, bool semilogX)
{
    semilogx = semilogX;
    xPix = xpix;
    yPix = ypix;

    plot_no = 0;

    // set up the renderer
    rend = vtkRenderer::New();
    rend->SetBackground(1., 1., 1.);

    // set up the xy plot
    xyplot = vtkXYPlotActor::New();

    xyplot->GetProperty()->SetColor(0.0, 0.0, 0.0);

    xyplot->SetBorder(10);
    xyplot->GetPositionCoordinate()->SetValue(0.0, 0.0, 0);
    xyplot->GetPosition2Coordinate()->SetValue(1.0, 1.0, 0);

    xyplot->GetProperty()->SetLineWidth(1);
    xyplot->GetProperty()->SetPointSize(5);

    xyplot->PlotPointsOff();
    xyplot->PlotLinesOff();
    xyplot->PlotCurvePointsOn();
    xyplot->PlotCurveLinesOn();

    xyplot->SetXValuesToArcLength();

    xyplot->SetLabelFormat("%2.1f");
    xyplot->SetTitle("");
    xyplot->SetXTitle(x_label.c_str());
    xyplot->SetYTitle(y_label.c_str());

    if (semilogx) {
        xyplot->LogxOn();
    }

    vtkTextProperty* text_prop = xyplot->GetTitleTextProperty();
    text_prop->SetColor(0.0, 0.0, 0.0);
    text_prop->SetFontFamilyToArial();
    xyplot->SetAxisTitleTextProperty(text_prop);
    xyplot->SetAxisLabelTextProperty(text_prop);
    xyplot->SetTitleTextProperty(text_prop);
}

/// Destructor
Plot2D_VTK::~Plot2D_VTK()
{
    xyplot->Delete();
    rend->Delete();
}

/// Render current figure to screen
void Plot2D_VTK::show()
{
    rend->AddActor(xyplot);
    render_interactive(rend,xPix,yPix);
}

/// Render current figure to file
void Plot2D_VTK::draw_to_png(std::string filename)
{
    rend->AddActor(xyplot);
    render_to_png(rend,xPix,yPix,filename);
}
//==============================================================================

Surf_VTK::Surf_VTK(int px, int py)
{
    has_data = false;
    Lxy = -1;
    Lz = -1;
    xPix = px;
    yPix = py;

    gridfunc = vtkStructuredGrid::New();
    rend = vtkRenderer::New();
}

/// Destructor
Surf_VTK::~Surf_VTK()
{
    gridfunc->Delete();
    rend->Delete();
}

/// Clear plot data before reuse
void Surf_VTK::purge()
{
    assert(has_data);
    gridfunc->Delete();
    rend->Delete();
    gridfunc = vtkStructuredGrid::New();
    rend = vtkRenderer::New();
    has_data = false;
    Lxy = -1;
    Lz = -1;
}

void Surf_VTK::renderer(bool draw_axes, bool draw_colorbar, bool draw_box,
			bool do_warp)
{
    assert(has_data);

    // filter to geometry primitive
    vtkStructuredGridGeometryFilter *geometry =
	vtkStructuredGridGeometryFilter::New();
    geometry->SetInputData(gridfunc);

    // warp to fit in box
    vtkWarpScalar *warp = vtkWarpScalar::New();
    if (do_warp)
    {
	double scale = Lxy/Lz;
	warp->SetInputConnection(geometry->GetOutputPort());
	warp->XYPlaneOn();
	warp->SetScaleFactor(scale);
    }

    // map gridfunction
    vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
    if (do_warp)
	mapper->SetInputConnection(warp->GetOutputPort());
    else
	mapper->SetInputConnection(geometry->GetOutputPort());

    double tmp[2];
    gridfunc->GetScalarRange(tmp);
    mapper->SetScalarRange(tmp[0], tmp[1]);

    // create plot surface actor
    vtkActor *surfplot = vtkActor::New();
    surfplot->SetMapper(mapper);

    // create outline
    vtkOutlineFilter *outlinefilter = vtkOutlineFilter::New();
    if (do_warp)
	outlinefilter->SetInputConnection(warp->GetOutputPort());
    else
	outlinefilter->SetInputConnection(geometry->GetOutputPort());
    vtkPolyDataMapper *outlineMapper = vtkPolyDataMapper::New();
    outlineMapper->SetInputConnection(outlinefilter->GetOutputPort());
    vtkActor *outline = vtkActor::New();
    outline->SetMapper(outlineMapper);
    outline->GetProperty()->SetColor(0, 0, 0);

    // create axes
    vtkAxesActor* axes = vtkAxesActor::New();
    axes->SetShaftTypeToCylinder();
    axes->SetNormalizedShaftLength( 0.85, 0.85, 0.85);
    axes->SetNormalizedTipLength( 0.15, 0.15, 0.15);
    axes->SetCylinderRadius( 0.500 * axes->GetCylinderRadius() );
    axes->SetConeRadius( 1.025 * axes->GetConeRadius() );
    axes->SetSphereRadius( 1.500 * axes->GetSphereRadius() );
    vtkTextProperty* text_prop_ax = axes->GetXAxisCaptionActor2D()->
	GetCaptionTextProperty();
    text_prop_ax->SetColor(0.0, 0.0, 0.0);
    text_prop_ax->SetFontFamilyToArial();
    text_prop_ax->SetFontSize(8);
    axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->
	ShallowCopy(text_prop_ax);
    axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->
	ShallowCopy(text_prop_ax);

    // create colorbar
    vtkScalarBarActor *colorbar = vtkScalarBarActor::New();
    colorbar->SetLookupTable(mapper->GetLookupTable());
    colorbar->SetWidth(0.085);
    colorbar->SetHeight(0.9);
    colorbar->SetPosition(0.9, 0.1);
    vtkTextProperty* text_prop_cb = colorbar->GetLabelTextProperty();
    text_prop_cb->SetColor(1.0, 1.0, 1.0);
    colorbar->SetLabelTextProperty(text_prop_cb);

    // renderer
    rend->AddActor(surfplot);
    if (draw_box)
	rend->AddActor(outline);
    if (draw_axes)
	rend->AddActor(axes);
    if (draw_colorbar)
	    rend->AddActor(colorbar);

    rend->SetBackground(0.25, 0.25, 0.25);

    // renderer is now set up!

    // clean up
    colorbar->Delete();
    warp->Delete();
    axes->Delete();
    outline->Delete();
    outlinefilter->Delete();
    outlineMapper->Delete();
    surfplot->Delete();
    mapper->Delete();
    geometry->Delete();
}
//==============================================================================

/// Default constructor
Contour_VTK::Contour_VTK(int px, int py)
{
	XScale = LINEAR;
	YScale = LINEAR;

    has_data = false;
    xPix = px;
    yPix = py;

    gridfunc = vtkRectilinearGrid::New();
    rend = vtkRenderer::New();
}

/// Default constructor
Contour_VTK::Contour_VTK(int px, int py, SCALE xsc, SCALE ysc)
{
	XScale = xsc;
	YScale = ysc;

	xlabel  = "x";
	ylabel  = "y";

    has_data = false;
    xPix = px;
    yPix = py;

    gridfunc = vtkRectilinearGrid::New();
    rend = vtkRenderer::New();
}

/// Destructor
Contour_VTK::~Contour_VTK()
{
    gridfunc->Delete();
    rend->Delete();
}

/// Clear plot data before reuse
void Contour_VTK::purge()
{
    assert(has_data);
    gridfunc->Delete();
    rend->Delete();
    gridfunc = vtkRectilinearGrid::New();
    rend = vtkRenderer::New();
    has_data = false;
}

/// Set labels for contour plots
void Contour_VTK::SetXLabel(const std::string &xlab) {
	xlabel = xlab;
}

void Contour_VTK::SetYLabel(const std::string &ylab ) {
	ylabel = ylab;
}

void Contour_VTK::renderer(bool draw_colorbar, bool draw_surf, int lines)
{
    assert(has_data);

    // filter to geometry primitive
    vtkRectilinearGridGeometryFilter *geometry =
	vtkRectilinearGridGeometryFilter::New();
    geometry->SetInputData(gridfunc);

    // map gridfunction
    vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(geometry->GetOutputPort());

    double tmp[2];
    gridfunc->GetScalarRange(tmp);
    mapper->SetScalarRange(tmp[0], tmp[1]);

    // create plot surface actor
    vtkActor *surfplot = vtkActor::New();
    surfplot->SetMapper(mapper);

    // create contour lines (10 lines)
    vtkContourFilter *contlines = vtkContourFilter::New();
    contlines->SetInputConnection(geometry->GetOutputPort());
    double tempdiff = (tmp[1]-tmp[0])/(10*lines);
    contlines->GenerateValues(lines, tmp[0]+tempdiff, tmp[1]-tempdiff);
    vtkPolyDataMapper *contourMapper = vtkPolyDataMapper::New();
    contourMapper->SetInputConnection(contlines->GetOutputPort());
    if (draw_surf)
	contourMapper->ScalarVisibilityOff();
    else
	contourMapper->SetScalarRange(tmp[0], tmp[1]);
    vtkActor *contours = vtkActor::New();
    contours->SetMapper(contourMapper);
	contours->GetProperty()->SetOpacity(.25);
	contours->GetProperty()->SetColor(0.5,0.5,0.5);

    // create outline
    vtkOutlineFilter *outlinefilter = vtkOutlineFilter::New();
    outlinefilter->SetInputConnection(geometry->GetOutputPort());
    vtkPolyDataMapper *outlineMapper = vtkPolyDataMapper::New();
    outlineMapper->SetInputConnection(outlinefilter->GetOutputPort());
    vtkActor *outline = vtkActor::New();
    outline->SetMapper(outlineMapper);
    outline->GetProperty()->SetColor(0, 0, 0);

    // create colorbar
    vtkScalarBarActor *colorbar = vtkScalarBarActor::New();
    if (draw_surf)
	colorbar->SetLookupTable(mapper->GetLookupTable());
    else
	colorbar->SetLookupTable(contourMapper->GetLookupTable());
//
    colorbar->SetWidth(0.085);
    colorbar->SetHeight(0.9);
    colorbar->SetPosition(0.9, 0.1);
    vtkTextProperty* text_prop_cb = colorbar->GetLabelTextProperty();
    text_prop_cb->SetColor(1.0, 1.0, 1.0);
    colorbar->SetLabelTextProperty(text_prop_cb);

	double xrange[2];
 	double yrange[2];
 	gridfunc->GetXCoordinates()->GetRange(xrange);
 	gridfunc->GetYCoordinates()->GetRange(yrange);
 	vtkCubeAxesActor2D  *caxis = vtkCubeAxesActor2D::New();

	caxis->ZAxisVisibilityOff();
	caxis->SetXLabel(xlabel.c_str());
	caxis->SetYLabel(ylabel.c_str());

	vtkCamera* mycam = rend->MakeCamera(); // vtkCamera::New();
	//rend->SetActiveCamera(mycam);
  	caxis->SetBounds( xrange[0], xrange[1], yrange[0], yrange[1], 0, 0);
  	caxis->SetRanges( xrange[0], xrange[1], ymin, ymax, 0, 0);
	caxis->SetUseRanges(1);

  	caxis->SetCamera( mycam );
	caxis->SetNumberOfLabels(6);
	// doesn't work, rotate y label 90 degrees
	caxis->GetYAxisActor2D()->GetTitleTextProperty()->SetOrientation(90.);
   	rend->AddActor(caxis);

	mycam->Delete();
	caxis->Delete();

	// handdrwan label
// 	// X label
// 	vtkVectorText                     *xlabel = vtkVectorText::New();
// 	vtkTransformPolyDataFilter  *transxlabel  = vtkTransformPolyDataFilter::New();
// 	vtkTransform                      *trans  = vtkTransform::New();
// 	vtkPolyDataMapper               *xlabmap  = vtkPolyDataMapper::New();
// 	vtkFollower                     *xlabact  = vtkFollower::New();
	// locate and rotate as needed
 	//double yranget = std::abs(yrange[1]-yrange[0]);
	//double xranget = std::abs(xrange[1]-xrange[0]);
// 	trans->Identity();
// 	trans->Scale(5,5,5);  // TODO, is 5 always OK???
// 	transxlabel->SetTransform(trans);
// 	transxlabel->SetInputConnection(xlabel->GetOutputPort());
// 	// TODO input name
// 	xlabel->SetText("Frequency [Hz]");
// 	transxlabel->SetInputConnection(xlabel->GetOutputPort());
// 	xlabmap->SetInputConnection(transxlabel->GetOutputPort());
// 	xlabact->SetMapper(xlabmap);
// 	// centre between axis
// 	xlabact->SetPosition( (xrange[0]+xrange[1])/2 - xlabact->GetCenter()[0],
// 					       yrange[0]-.2*yranget, 0 );
// 	//rend->AddActor(xlabact);
// 	// Y label
// 	vtkVectorText                     *ylabel = vtkVectorText::New();
// 	vtkTransformPolyDataFilter  *transylabel  = vtkTransformPolyDataFilter::New();
// 	vtkTransform                     *ytrans  = vtkTransform::New();
// 	vtkPolyDataMapper               *ylabmap  = vtkPolyDataMapper::New();
// 	vtkFollower                     *ylabact  = vtkFollower::New();
// 	// locate and rotate as needed
// 	ytrans->Identity();
// 	ytrans->Scale(5,5,5); // TODO don't hard code, calc from window size maybe??
// 	ytrans->RotateZ(90);
// 	transylabel->SetTransform(ytrans);
// 	transylabel->SetInputConnection(ylabel->GetOutputPort());
//
// 	ylabel->SetText("Pulse moment [A sec]");
// 	transylabel->SetInputConnection(ylabel->GetOutputPort());
// 	ylabmap->SetInputConnection(transylabel->GetOutputPort());
// 	ylabact->SetMapper(ylabmap);
// 	ylabact->SetPosition( xrange[0]-.2*xranget,
// 					      (yrange[0]+yrange[1])/2 - ylabact->GetCenter()[1],
// 						  0 );
// 	//rend->AddActor(ylabact);

    if (draw_surf)
 	rend->AddActor(surfplot);

	rend->AddActor(contours);
	rend->AddActor(outline);
	if (draw_colorbar) {
		rend->AddActor(colorbar);
 	}

	rend->SetBackground(0.25, 0.25, 0.25);

// 	double xrange[2];
// 	double yrange[2];
// 	gridfunc->GetXCoordinates()->GetRange(xrange);
// 	gridfunc->GetYCoordinates()->GetRange(yrange);
// 	vtkCubeAxesActor	 *caxis = vtkCubeAxesActor::New();
// 	caxis->ZAxisVisibilityOff();
// 	caxis->SetCamera(rend->GetActiveCamera());
// 	caxis->SetBounds( xrange[0], xrange[1], yrange[0], yrange[1], 0, 1 );
//  	rend->AddActor(caxis);

 	//rend->AddActor(yaxis);
//     // clean up
// 	xlabel->Delete();
// 	transxlabel->Delete();
// 	trans->Delete();
// 	xlabmap->Delete();
// 	xlabact->Delete();
//
// 	ylabel->Delete();
// 	transylabel->Delete();
// 	ytrans->Delete();
// 	ylabmap->Delete();
// 	ylabact->Delete();

	// renderer is now set up!
	//caxis->Delete();
	//yaxis->Delete();

    contours->Delete();
    contlines->Delete();
    contourMapper->Delete();
    outline->Delete();
    outlinefilter->Delete();
    outlineMapper->Delete();
    colorbar->Delete();
    surfplot->Delete();
    mapper->Delete();
    geometry->Delete();

}
//==============================================================================

/// Default constructor
Quiver_VTK::Quiver_VTK(int px, int py)
{
    has_data = false;
    xPix = px;
    yPix = py;

    gridfunc = vtkRectilinearGrid::New();
    rend = vtkRenderer::New();
}

/// Destructor
Quiver_VTK::~Quiver_VTK()
{
    gridfunc->Delete();
    rend->Delete();
}

/// Clear plot data before reuse
void Quiver_VTK::purge()
{
    assert(has_data);
    gridfunc->Delete();
    rend->Delete();
    gridfunc = vtkRectilinearGrid::New();
    rend = vtkRenderer::New();
    has_data = false;
}

void Quiver_VTK::renderer(double s)
{
    assert(has_data);

    // filter to geometry primitive
    vtkRectilinearGridGeometryFilter *geometry =
	vtkRectilinearGridGeometryFilter::New();
    geometry->SetInputData(gridfunc);

    // make a vector glyph
    vtkGlyphSource2D* vec = vtkGlyphSource2D::New();
    vec->SetGlyphTypeToArrow();
    vec->SetScale(s);
    vec->FilledOff();

    vtkGlyph3D* glyph = vtkGlyph3D::New();
    glyph->SetInputConnection(geometry->GetOutputPort());
    glyph->SetSourceConnection(vec->GetOutputPort());
    glyph->SetColorModeToColorByScalar();

    // map gridfunction
    vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(glyph->GetOutputPort());

    double tmp[2];
    gridfunc->GetScalarRange(tmp);
    mapper->SetScalarRange(tmp[0], tmp[1]);

    // create plot quiver actor
    vtkActor *quiver_actor = vtkActor::New();
    quiver_actor->SetMapper(mapper);

    // create colorbar
    vtkScalarBarActor *colorbar = vtkScalarBarActor::New();
    colorbar->SetLookupTable(mapper->GetLookupTable());
    colorbar->SetWidth(0.085);
    colorbar->SetHeight(0.9);
    colorbar->SetPosition(0.9, 0.1);
    vtkTextProperty* text_prop_cb = colorbar->GetLabelTextProperty();
    text_prop_cb->SetColor(1.0, 1.0, 1.0);
    colorbar->SetLabelTextProperty(text_prop_cb);

    // create outline
    vtkOutlineFilter *outlinefilter = vtkOutlineFilter::New();
    outlinefilter->SetInputConnection(geometry->GetOutputPort());
    vtkPolyDataMapper *outlineMapper = vtkPolyDataMapper::New();
    outlineMapper->SetInputConnection(outlinefilter->GetOutputPort());
    vtkActor *outline = vtkActor::New();
    outline->SetMapper(outlineMapper);
    outline->GetProperty()->SetColor(0, 0, 0);

    // add actors to renderer
    rend->AddActor(quiver_actor);
    rend->AddActor(colorbar);
    rend->AddActor(outline);
    rend->SetBackground(0.25, 0.25, 0.25);

    // renderer is now set up!

    // clean up
    outline->Delete();
    outlinefilter->Delete();
    outlineMapper->Delete();
    vec->Delete();
    glyph->Delete();
    colorbar->Delete();
    quiver_actor->Delete();
    mapper->Delete();

}

//==============================================================================

/** Start interactive rendereing (default camera).
 * Sets resolution to (xPix,yPix)
 */
void matplot::render_interactive(vtkRenderer *rend, int xPix, int yPix)
{
	vtkRenderWindow *renWin = vtkRenderWindow::New();
    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
    renWin->AddRenderer(rend);
    iren->SetRenderWindow(renWin);
    renWin->SetSize(xPix, yPix);

	// makes first frame look reasonable interactive
	renWin->Render();
	renWin->Start();

    // Start interactive rendering
    iren->Initialize();
    iren->Start();
    iren->Disable();
	iren->Delete();
    renWin->Delete();
}

/** Start interactive rendereing (manual camera placement).
 * Sets resolution to (xPix,yPix)
 */
void matplot::render_interactive_cam(vtkRenderer *rend, int xPix, int yPix,
				     double cam[3], double focal[3])
{
    rend->GetActiveCamera()->SetViewUp(0, 0, 1);
    rend->GetActiveCamera()->SetPosition(cam);
    rend->GetActiveCamera()->SetFocalPoint(focal);

    vtkRenderWindow *renWin = vtkRenderWindow::New();
    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
    renWin->AddRenderer(rend);
    iren->SetRenderWindow(renWin);
    renWin->SetSize(xPix, yPix);

    // Start interactive rendering
    iren->Initialize();
    iren->Start();

    iren->Delete();
    renWin->Delete();
}

/// Renders scene to PNG (default camera)
void matplot::render_to_png(vtkRenderer *rend, int xPix, int yPix,
			    std::string fname)
{
    vtkRenderWindow *renWin = vtkRenderWindow::New();
    renWin->AddRenderer(rend);
    renWin->SetSize(xPix, yPix);
    renWin->Render();
    rend->Render();

    vtkRenderLargeImage* renderLarge = vtkRenderLargeImage::New();
        renderLarge->SetInput(rend);
        renderLarge->SetMagnification(1);

    vtkPNGWriter *pngfile = vtkPNGWriter::New();
        pngfile->SetFileName(fname.c_str());
        pngfile->SetInputConnection(renderLarge->GetOutputPort());
        pngfile->Update();
        pngfile->Update();

    pngfile->Update();
    pngfile->Write();

    renderLarge->Delete();
    pngfile->Delete();
    renWin->Delete();
}

/// Renders scene to PNG (manual camera placement)
void matplot::render_to_png_cam(vtkRenderer *rend, int xPix, int yPix,
				std::string fname, double cam[3],
				double focal[3])
{
    rend->GetActiveCamera()->SetViewUp(0, 0, 1);
    rend->GetActiveCamera()->SetPosition(cam);
    rend->GetActiveCamera()->SetFocalPoint(focal);

    vtkRenderWindow *renWin = vtkRenderWindow::New();
    renWin->AddRenderer(rend);
    renWin->SetSize(xPix, yPix);

    vtkRenderLargeImage* renderLarge = vtkRenderLargeImage::New();
        renderLarge->SetInput(rend);
        renderLarge->SetMagnification(1);

    //vtkWindowToImageFilter *w2i = vtkWindowToImageFilter::New();
    vtkPNGWriter *pngfile = vtkPNGWriter::New();
        //pngfile->SetInputConnection(w2i->GetOutputPort());
        pngfile->SetInputConnection(renderLarge->GetOutputPort());
    pngfile->SetFileName(fname.c_str());

    //w2i->SetInput(renWin);
    //w2i->Update();

    renWin->Render();
    pngfile->Write();

    //w2i->Delete();
    renderLarge->Delete();
    pngfile->Delete();
    renWin->Delete();
}

#endif
