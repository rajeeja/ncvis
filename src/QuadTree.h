///////////////////////////////////////////////////////////////////////////////
///
///	\file    QuadTree.h
///	\author  Paul Ullrich
///	\version June 27, 2022
///

#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "Exception.h"

///////////////////////////////////////////////////////////////////////////////

class QuadTreeNode {

public:
	///	<summary>
	///		Maximum number of levels in QuadTree.
	///	</summary>
	static const int QUADTREE_MAX_LEVELS = 32;

public:
	///	<summary>
	///		Constructor.
	///	</summary>
	QuadTreeNode(
		double dX0 = 0.0,
		double dX1 = 360.0,
		double dY0 = -180.0,
		double dY1 = 180.0,
		int iLevel = 0
	) :
		m_dX0(dX0),
		m_dY0(dY0),
		m_iLevel(iLevel)
	{
		m_dXdelta = 0.5 * (dX1 - dX0);
		m_dYdelta = 0.5 * (dY1 - dY0);

		for (size_t i = 0; i < 4; i++) {
			m_dXcoord[i] = 0.0;
			m_dYcoord[i] = 0.0;
			m_sIndex[i] = static_cast<size_t>(-1);
			m_pChildren[i] = NULL;
		}
	}

	///	<summary>
	///		Insert a point into the quadtree.
	///	</summary>
	bool insert(
		double dX,
		double dY,
		size_t sI
	) {
		size_t sXi = static_cast<size_t>((dX - m_dX0) / m_dXdelta);
		size_t sYi = static_cast<size_t>((dY - m_dY0) / m_dYdelta);

		_ASSERT(sXi < 2);
		_ASSERT(sYi < 2);

		const size_t sIx = 2 * sYi + sXi;

		if (m_pChildren[sIx] != NULL) {
			m_pChildren[sIx]->insert(dX, dY, sI);

		} else if (m_sIndex[sIx] == static_cast<size_t>(-1)) {
			m_dXcoord[sIx] = dX;
			m_dYcoord[sIx] = dY;
			m_sIndex[sIx] = sI;

		} else if (m_iLevel != QUADTREE_MAX_LEVELS) {
			m_pChildren[sIx] =
				new QuadTreeNode(
					m_dX0 + static_cast<double>(sXi) * m_dXdelta,
					m_dX0 + static_cast<double>(sXi+1) * m_dXdelta,
					m_dY0 + static_cast<double>(sYi) * m_dYdelta,
					m_dY0 + static_cast<double>(sYi+1) * m_dYdelta,
					m_iLevel + 1);

			m_pChildren[sIx]->insert(m_dXcoord[sIx], m_dYcoord[sIx], m_sIndex[sIx]);
			m_pChildren[sIx]->insert(dX, dY, sI);
		}

		return true;
	}

	///	<summary>
	///		Inquire about closest point in quadtree, using inexact search.
	///	</summary>
	size_t find_inexact(
		double dX,
		double dY
	) const {
		static const int s_iLookup[4][3] = {{2,1,3}, {0,3,2}, {3,0,1}, {1,2,0}};

		int iX = static_cast<int>((dX - m_dX0) / m_dXdelta);
		int iY = static_cast<int>((dY - m_dY0) / m_dYdelta);

		if (iX < 0) {
			iX = 0;
		} else if (iX > 1) {
			iX = 1;
		}
		if (iY < 0) {
			iY = 0;
		} else if (iY > 1) {
			iY = 1;
		}

		const size_t sIx = static_cast<size_t>(2 * iY + iX);

		_ASSERT(sIx < 4);

		if (m_pChildren[sIx] != NULL) {
			return m_pChildren[sIx]->find_inexact(dX, dY);

		} else if (m_sIndex[sIx] != static_cast<size_t>(-1)) {
			return m_sIndex[sIx];

		} else {
			for (int k = 0; k < 3; k++) {
				int ixNeighbor = s_iLookup[sIx][k];
				if (m_pChildren[ixNeighbor] != NULL) {
					return m_pChildren[ixNeighbor]->find_inexact(dX, dY);

				} else if (m_sIndex[ixNeighbor] != static_cast<size_t>(-1)) {
					return m_sIndex[ixNeighbor];
				}
			}
		}

		_EXCEPTION();
	}

protected:
	///	<summary>
	///		Lower coordinate bound (X direction).
	///	</summary>
	double m_dX0;

	///	<summary>
	///		Coordinate spacing (X direction).
	///	</summary>
	double m_dXdelta;

	///	<summary>
	///		Lower coordinate bound (Y direction).
	///	</summary>
	double m_dY0;

	///	<summary>
	///		Coordinate spacing (Y direction).
	///	</summary>
	double m_dYdelta;

	///	<summary>
	///		Level of this node.
	///	</summary>
	int m_iLevel;

	///	<summary>
	///		X coordinate in each child node.
	///	</summary>
	double m_dXcoord[4];

	///	<summary>
	///		Y coordinate in each child node.
	///	</summary>
	double m_dYcoord[4];

	///	<summary>
	///		Index in each child node.
	///	</summary>
	size_t m_sIndex[4];

	///	<summary>
	///		Pointers to child nodes.
	///	</summary>
	QuadTreeNode * m_pChildren[4];
};

///////////////////////////////////////////////////////////////////////////////

#endif // _QUADTREE_H_

