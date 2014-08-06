/**
 * File:   Line.h
 *
 * Created on 30. September 2010, 09:40
 *
 * @section LICENSE
 * This file is part of JuPedSim.
 *
 * JuPedSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * JuPedSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with JuPedSim. If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 *
 *
 */

#ifndef _LINE_H
#define	_LINE_H

#include "Point.h"
#include "../IO/OutputHandler.h"

#include <string>


// external variables
extern OutputHandler* Log;


class Line {
private:
    Point _point1;
    Point _point2;
    Point _centre;

    //unique identifier for all line elements
    static int _static_UID;
    int _uid;

public:

    Line();
    Line(const Point& p1, const Point& p2);
    Line(const Line& orig);
    virtual ~Line();

    /**
     * All Line elements (also derived class) have a unique ID
     * @return the unique ID of this line element.
     */
    int GetUniqueID() const;

    /**
     * Set/Get the first end point of the line
     */
    void SetPoint1(const Point& p);

    /**
     * Set/Get the second end point of the line
     */
    void SetPoint2(const Point& p);

    /**
     * Set/Get the first end point of the line
     */
    const Point& GetPoint1(void) const;

    /**
     * Set/Get the second end point of the line
     */
    const Point& GetPoint2(void) const;

    /**
     * Return the center of the line
     */
    const Point& GetCentre(void) const;

    /**
     * @return a normal vector to this line
     */
    Point NormalVec() const; // Normalen_Vector zu Line

    /**
     *TODO: FIXME
     */
    double NormalComp(const Point& v) const; // Normale Komponente von v auf l

    /**
     * Note that that result must not lie on the segment
     * @return the orthogonal projection of p on the line defined by the segment points.
     */
    Point LotPoint(const Point& p) const;

    /**
     * @return the point on the segment with the minimum distance to p
     */
    Point ShortestPoint(const Point& p) const;

    /**
     * @return true if the point p lies on the line defined by the first and the second point
     */
    bool IsInLine(const Point& p) const;

    /**
     * @see IsInLine
     * @return true if the point p is within the line segment defined the line end points
     */
    bool IsInLineSegment(const Point& p) const;

    /**
     * @return the distance from the line to the point p
     */
    double DistTo(const Point& p) const;

    /**
     * @return the distance square from the line to the point p
     */
    double DistToSquare(const Point& p) const;

    /**
     * @return the length (Norm) of the line
     */
    double Length() const;

    /**
     * @return the lenght square of  the segment
     */
    double LengthSquare() const;

    /**
     * @return true if both segments are equal. The end points must be in the range of J_EPS.
     * @see Macro.h
     */
    bool operator==(const Line& l) const;

    /**
     * @return true if both segments are not equal. The end points must be in the range of J_EPS.
     * @see Macro.h
     */
    bool operator!=(const Line& l) const;

    /**
     * @see http://alienryderflex.com/intersect/
     * @see http://social.msdn.microsoft.com/Forums/en-US/csharpgeneral/thread/e5993847-c7a9-46ec-8edc-bfb86bd689e3/
     * @return true if both segments intersect
     */
    bool IntersectionWith(const Line& l) const; // check two segments for intersections

    /**
     * @return true if the segment intersects with the circle of radius r
     */
    bool IntersectionWithCircle(const Point& centre, double radius=0.30 /*m for pedestrians*/);


    /**
     * @return true if both segments share at least one common point
     */
    bool ShareCommonPointWith(const Line& line) const;

    /**
     * @return true if the given point is one end point of the segment
     */
    bool HasEndPoint(const Point& point) const;

    /**
     * return the same value if the checked points are all situated on the same side.
     * @return 0 or 1 depending on which side of the line the point is located.
     */
    int WichSide (const Point& pt);

    /**
     * @return true if the point is located in the left hand side of the line.
     * For horizontal lines return true if the point is above the line.
     */
    bool IsLeft (const Point& pt);

    /**
     * @return true for horizontal lines
     */
    bool IsHorizontal();

    /**
     * @return true for vertical lines
     */
    bool IsVertical();

    /**
     * @return left point wrt. the point pt
    */
    const Point& GetLeft(const Point& pt);

    /**
    	 * @return left point wrt. the point pt
    	*/
    const Point& GetRight(const Point& pt);

    /**
     * @return a nice formated string describing the line
     */
    virtual std::string Write() const;

    /**
     * @return a nice formated string describing the line
     */
    std::string toString() const;


};

#endif	/* _LINE_H */

