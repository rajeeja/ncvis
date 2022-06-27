///////////////////////////////////////////////////////////////////////////////
///
///	\file    wxImagePanel.h
///	\author  Paul Ullrich
///	\version June 27, 2022
///

#ifndef _WXIMAGEPANEL_H_
#define _WXIMAGEPANEL_H_

#include <wx/wx.h>
#include <wx/sizer.h>

#include "DataArray1D.h"
#include "CoordTransforms.h"
#include "GridDataSampler.h"
#include "ColorMap.h"

////////////////////////////////////////////////////////////////////////////////

///	<summary>
///		A wxWidget that manages display of the data.
///	</summary>
class wxImagePanel : public wxPanel {
public:
	///	<summary>
	///		Constructor.
	///	</summary>
	wxImagePanel(
		wxFrame * parent,
		const GridDataSampler * pgds,
		const DataArray1D<float> * pdata
	);

public:
	///	<summary>
	///		Callback for when a paint event is triggered.
	///	</summary>
	void OnPaint(wxPaintEvent & evt);

	///	<summary>
	///		Callback for when a size event is triggered.
	///	</summary>
	void OnSize(wxSizeEvent & evt);

	///	<summary>
	///		Callback for when an idle event is triggered.
	///	</summary>
	void OnIdle(wxIdleEvent & evt);

	///	<summary>
	///		Callback for when the mouse is moved.
	///	</summary>
	void OnMouseMotion(wxMouseEvent & evt);

	///	<summary>
	///		Callback for when the mouse leaves the window.
	///	</summary>
	void OnMouseLeaveWindow(wxMouseEvent & evt);

	///	<summary>
	///		Callback for when the mouse is left-double-clicked.
	///	</summary>
	void OnMouseLeftDoubleClick(wxMouseEvent & evt);

public:
	///	<summary>
	///		Generate the image from the image map.
	///	</summary>
	void GenerateImageFromImageMap(
		bool fRedraw = false
	);

	///	<summary>
	///		Set the color map.
	///	</summary>
	void SetColorMap(
		const std::string & strColorMap,
		bool fRedraw = false
	);

	///	<summary>
	///		Resample the coordinate range.
	///	</summary>
	void SetCoordinateRange(
		double dX0,
		double dX1,
		double dY0,
		double dY1,
		bool fRedraw = false
	);

	///	<summary>
	///		Set the data range.
	///	</summary>
	void SetDataRange(
		float dDataMin,
		float dDataMax,
		bool fRedraw = false
	);

public:
	///	<summary>
	///		Repaint the panel now.
	///	</summary>
	void PaintNow();

	///	<summary>
	///		Render the device context.
	///	</summary>
	void Render(wxDC & dc);

private:
	///	<summary>
	///		Colormap.
	///	</summary>
	ColorMap m_colormap;

	///	<summary>
	///		Longitude region displayed in plot.
	///	</summary>
	double m_dXrange[2];

	///	<summary>
	///		Latitude region displayed in plot.
	///	</summary>
	double m_dYrange[2];

	///	<summary>
	///		Data range displayed in plot.
	///	</summary>
	float m_dDataRange[2];

	///	<summary>
	///		Pointer to the grid data sampler.
	///	</summary>
	const GridDataSampler * m_pgds;

	///	<summary>
	///		Pointer to the data.
	///	</summary>
	const DataArray1D<float> * m_pdata;

	///	<summary>
	///		Array of sample longitudes.
	///	</summary>
	DataArray1D<double> m_dSampleLon;

	///	<summary>
	///		Array of sample latitudes.
	///	</summary>
	DataArray1D<double> m_dSampleLat;

	///	<summary>
	///		Image map.
	///	</summary>
	DataArray1D<int> m_imagemap;

	///	<summary>
	///		Image bitmap data.
	///	</summary>
	wxImage m_image;

	///	<summary>
	///		A flag indicating the window has been resized.
	///	</summary>
	bool m_fResize;

	DECLARE_EVENT_TABLE()
};

////////////////////////////////////////////////////////////////////////////////

#endif // _WXIMAGEPANEL_H_

