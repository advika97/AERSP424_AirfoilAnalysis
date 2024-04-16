#include "AirfoilElement.h"


AirfoilElement::AirfoilElement()
{
	InitElement();
}

AirfoilElement::AirfoilElement(int nPoints)
{
	InitElement();
	this->PointCount = nPoints;
}

AirfoilElement::AirfoilElement(const AirfoilElement& theSource)
{
	InitElement();
	PointCount = theSource.PointCount;
	Points = theSource.Points;
	ArcLength = theSource.ArcLength;
	m_CamberLine = theSource.m_CamberLine;
	m_Thickness = theSource.m_Thickness;
	m_Curvature = theSource.m_Curvature;
	Inertia = theSource.Inertia;
	Center = theSource.Center;
	Area = theSource.Area;
	Cl = theSource.Cl;
	Cm = theSource.Cm;
	m_LEIndex = theSource.m_LEIndex;
	ptCamber = theSource.ptCamber;
	ptThickness = theSource.ptThickness;
}

AirfoilElement::AirfoilElement(vector<DoublePoint>& ptControl, int nDegree, int nPoints) : Points(nPoints, DoublePoint())
{
	InitElement();
	PointCount = nPoints;
	//Points = ptControl;
	BSpline(nDegree, ptControl);
	UpdateProps(Center);
	UpdateCamberAndThickness();
}

void AirfoilElement::UpdateProps(DoublePoint& center)
{
	UpdateProps(center.dX, center.dY);
}

void AirfoilElement::UpdateProps(double dXs, double dYs)
{
	double dIYY = 0.0;
	double dIXX = 0.0;
	double dA = 0.0;
	DoublePoint SavedPoint(Points[PointCount - 1]); // last point
	Points[PointCount - 1].dX = 0.5 * (Points[0].dX + Points[PointCount - 1].dX);
	Points[PointCount - 1].dY = 0.5 * (Points[0].dY + Points[PointCount - 1].dY);
	xMin = Points[0].dX;
	xMax = Points[0].dX;
	yMin = Points[0].dY;
	yMax = Points[0].dY;

	for (int nCount = 1; nCount < PointCount; ++nCount) {
		double dIX = (Points[nCount].dX - Points[nCount - 1].dX) / 6.0 * (pow(Points[nCount - 1].dY - Points[nCount].dY, 3.0) / 2.0 + Points[nCount].dY * pow(Points[nCount - 1].dY - Points[nCount].dY, 2.0) + 3.0 * pow(Points[nCount].dY, 2.0) * (Points[nCount - 1].dY - Points[nCount].dY) + 2.0 * pow(Points[nCount].dY, 3.0));
		double dIY = (Points[nCount].dY - Points[nCount - 1].dY) / 6.0 * (pow(Points[nCount - 1].dX - Points[nCount].dX, 3.0) / 2.0 + Points[nCount].dX * pow(Points[nCount - 1].dX - Points[nCount].dX, 2.0) + 3.0 * pow(Points[nCount].dX, 2.0) * (Points[nCount - 1].dX - Points[nCount].dX) + 2.0 * pow(Points[nCount].dX, 3.0));
		dIXX += dIX;
		dIYY += dIY;
		dA += Points[nCount - 1].dX * Points[nCount].dY - Points[nCount].dX * Points[nCount - 1].dY;
		xMin = min(xMin, Points[nCount].dX);
		xMax = max(xMax, Points[nCount].dX);
		yMin = min(yMin, Points[nCount].dY);
		yMax = max(yMax, Points[nCount].dY);
	}

	dA /= 2.0;
	Inertia.dX = abs(dIXX) - dA * dYs * dYs;
	Inertia.dY = abs(dIYY) - dA * dXs * dXs;
	Points[PointCount - 1].dX = SavedPoint.dX;
	Points[PointCount - 1].dY = SavedPoint.dY;
	Area = dA;
	double dRadiusMin = 0.0;
	double xTE = 0.5 * (Points[0].dX + Points[PointCount - 1].dX);
	double yTE = 0.5 * (Points[0].dY + Points[PointCount - 1].dY);
	m_LEIndex = 0;

	int i;
	for (i = 1; i < PointCount; ++i) {
		double dRadiusSquared = pow(Points[i].dX - xTE, 2.0) + pow(Points[i].dY - yTE, 2.0);
		if (dRadiusSquared > dRadiusMin) {
			dRadiusMin = dRadiusSquared;
			m_LEIndex = i;
		}
	}

	if (Area < 0.0 && m_Recursive == 0) {
		ReversePointSequence();
		m_Recursive = 1;
		UpdateProps(dXs, dYs);
	}

	ArcLength.reserve(PointCount);// = new double[PointCount];
	ArcLength.emplace_back(0.0);

	for (i = 1; i < PointCount; ++i) {
		ArcLength.emplace_back(ArcLength[i - 1] + hypot(Points[i].dX - Points[i - 1].dX, Points[i].dY - Points[i - 1].dY));
	}

}

void AirfoilElement::ReversePointSequence()
{
	reverse(Points.begin(), Points.end());
}

double AirfoilElement::getLowerY(double x)
{
	double RetVal = Points[0].dY;
	if (x >= Points[PointCount - 1].dX) {
		RetVal = Points[PointCount - 1].dY;
	}
	else if (x <= Points[m_LEIndex].dX) {
		RetVal = Points[m_LEIndex].dY;
	}
	else {
		for (int i = m_LEIndex + 1; i < PointCount; ++i) {
			if (Points[i].dX >= x) {
				RetVal = Points[i - 1].dY + (Points[i].dY - Points[i - 1].dY) * (x - Points[i - 1].dX) / (Points[i].dX - Points[i - 1].dX);
				break;
			}
		}
	}

	return RetVal;;
}

double AirfoilElement::getUpperY(double x)
{
	double RetVal = Points[0].dY;
	if (x >= Points[0].dX) {
		RetVal = Points[0].dY;
	}
	else if (x <= Points[m_LEIndex].dX) {
		RetVal = Points[m_LEIndex].dY;
	}
	else {
		for (int i = 1; i <= m_LEIndex; ++i) {
			if (x >= Points[i].dX) {
				double dDeltaX = Points[i].dX - Points[i - 1].dX;
				if (dDeltaX > 1.0E-24) {
					RetVal = Points[i - 1].dY + (Points[i].dY - Points[i - 1].dY) * (x - Points[i - 1].dX) / dDeltaX;
				}
				else {
					RetVal = Points[i].dY;
				}
				break;
			}
		}
	}

	return RetVal;

}

void AirfoilElement::setPoints(int nPoints, vector<double>& X, vector<double>& Y)
{
	setPoints(nPoints, X, Y, 0);
}

void AirfoilElement::setPoints(int nPoints, vector<double>& X, vector<double>& Y, int nOffset)
{
	vector<bool> valid(nPoints);
	valid.emplace_back(true);
	int j = 1;

	int i;
	for (i = 1; i < nPoints; ++i) {
		if (!(abs(X[nOffset + i] - X[nOffset + i - 1]) > 1.0E-16) && !(abs(Y[nOffset + i] - Y[nOffset + i - 1]) > 1.0E-16)) {
			valid.emplace_back(false);
		}
		else {
			++j;
			valid.emplace_back(true);
		}
	}

	PointCount = j;
	Points.clear();
	Points.reserve(PointCount);

	Points.emplace_back(DoublePoint(X[nOffset + 0], Y[nOffset + 0]));
	for (i = 1; i < nPoints; ++i) {
		if (valid[i]) {
			Points.emplace_back(DoublePoint(X[nOffset + i], Y[nOffset + i]));
		}
	}

	UpdateProps(Center);
	UpdateCamberAndThickness();

}

void AirfoilElement::setPoints(int nPoints, vector<DoublePoint>& pt)
{
	setPoints(nPoints, pt, 0);
}

void AirfoilElement::setPoints(int nPoints, vector<DoublePoint>& pt, int nOffset)
{
	int j = nPoints;

	do {
		PointCount = j;
		Points.clear();
		Points.reserve(PointCount);
		j = 0;

		for (int i = 0; i < nPoints; ++i) {
			Points.emplace_back(pt[nOffset + i]);
			if (j > 0) {
				if (abs(Points[j].dX - Points[j - 1].dX) > 1.0E-16 || abs(Points[j].dY - Points[j - 1].dY) > 1.0E-16) {
					++j;
				}
			}
			else {
				++j;
			}
		}
	} while (j != PointCount);

	UpdateProps(Center);
	UpdateCamberAndThickness();

}

bool AirfoilElement::getPoints(vector<double>& X, vector<double>& Y)
{
	return getPoints(X, Y, 0);
}

bool AirfoilElement::getPoints(vector<double>& X, vector<double>& Y, int nOffset)
{
	bool bRetVal = false;
	if (X.size() >= Points.size() + nOffset) {
		for (int i = 0; i < Points.size(); ++i) {
			X[i + nOffset] = Points[i].dX;
			Y[i + nOffset] = Points[i].dY;
		}

		bRetVal = true;
	}

	return bRetVal;

}

bool AirfoilElement::getPoints(vector<DoublePoint>& pts)
{
	return getPoints(pts, 0);
}

bool AirfoilElement::getPoints(vector<DoublePoint>& pts, int nOffset)
{
	bool bRetVal = false;
	if (pts.size() >= Points.size() + nOffset) {
		for (int i = 0; i < Points.size(); ++i) {
			pts[i + nOffset].dX = Points[i].dX;
			pts[i + nOffset].dY = Points[i].dY;
		}

		bRetVal = true;
	}

	return bRetVal;

}

double AirfoilElement::getTEThickness()
{
	double dX = Points[0].dX - Points[PointCount - 1].dX;
	double dY = Points[0].dY - Points[PointCount - 1].dY;
	double dSign = dY < 0.0 ? -1.0 : 1.0;
	return dSign * hypot(dX, dY);
}

void AirfoilElement::setTEThickness(double newTEThickness)
{
	int nLE = m_LEIndex;

	for (int j = 0; j < 5; ++j) {
		double dCurrentTEThickness = getTEThickness();
		double dDeltaArc = 0.5 * (newTEThickness - dCurrentTEThickness);
		double dX = Points[0].dX - Points[nLE].dX;
		double dY = Points[0].dY - Points[nLE].dY;
		double dDeltaTheta = dDeltaArc / hypot(dX, dY);

		double dRadius;
		double dTheta;
		int i;
		for (i = 0; i < nLE - 1; ++i) {
			dX = Points[i].dX - Points[nLE].dX;
			dY = Points[i].dY - Points[nLE].dY;
			dRadius = hypot(dX, dY);
			dTheta = atan2(Points[i].dY - Points[nLE].dY, Points[i].dX - Points[nLE].dX);
			dTheta += dDeltaTheta;
			Points[i].dX = Points[nLE].dX + dRadius * cos(dTheta);
			Points[i].dY = Points[nLE].dY + dRadius * sin(dTheta);
		}

		for (i = nLE + 1; i < PointCount; ++i) {
			dX = Points[i].dX - Points[nLE].dX;
			dY = Points[i].dY - Points[nLE].dY;
			dRadius = hypot(dX, dY);
			dTheta = atan2(Points[i].dY - Points[nLE].dY, Points[i].dX - Points[nLE].dX);
			dTheta -= dDeltaTheta;
			Points[i].dX = Points[nLE].dX + dRadius * cos(dTheta);
			Points[i].dY = Points[nLE].dY + dRadius * sin(dTheta);
		}
	}

	UpdateProps(Center);
	UpdateCamberAndThickness();

}

void AirfoilElement::ReSpline(int NewPointCount)
{
	ReSpline(NewPointCount, 0.1);
}

void AirfoilElement::ReSpline(int NewPointCount, double weight)
{
	ArcLength.clear();
	ArcLength.reserve(PointCount);
	vector<double> X(PointCount);
	vector<double> Y(PointCount);
	NewPointCount /= 2;
	NewPointCount = 1 + 2 * NewPointCount;
	int iLE = 0;
	double xLE = Points[0].dX;
	double xTE = Points[0].dX;

	int i;
	for (i = 0; i < PointCount; ++i) {
		X[i] = Points[i].dX;
		Y[i] = Points[i].dY;
		if (X[i] < xLE) {
			xLE = X[i];
			iLE = i;
		}
	}

	ArcLength.emplace_back(0.0);

	for (i = 1; i < PointCount; ++i) {
		ArcLength.emplace_back(ArcLength[i - 1] + hypot(X[i] - X[i - 1], Y[i] - Y[i - 1]));
	}

	RationalSpline theXSpline(ArcLength, X, weight);
	RationalSpline theYSpline(ArcLength, Y, weight);
	double sLE = ArcLength[iLE];
	double sTE = ArcLength[PointCount - 1];
	double ds;
	double Radius;
	double x;
	if (iLE + 1 < PointCount) {
		ds = (ArcLength[iLE + 1] - ArcLength[iLE - 1]) / 250.0;

		for (Radius = ArcLength[iLE - 1]; Radius <= ArcLength[iLE + 1]; Radius += ds) {
			x = theXSpline.EvalPoint(Radius);
			if (x < xLE) {
				sLE = Radius;
				xLE = x;
			}
		}
	}

	Points.clear();
	Points.reserve(NewPointCount);
	ds = 0.1;
	Radius = sqrt(1.0 + ds * ds);
	x = acos(1.0 / Radius);
	double Phi_1 = M_PI - x;
	int iNewLE = (NewPointCount - 1) / 2;
	double DeltaPhi = (Phi_1 - x) / (double)iNewLE;
	double Phi = Phi_1;

	double s;
	//int i;
	for (i = 0; i < iNewLE; ++i) {
		s = sLE * 0.5 * (1.0 + Radius * cos(Phi));
		Points.emplace_back(DoublePoint(theXSpline.EvalPoint(s), theYSpline.EvalPoint(s)));
		Phi -= DeltaPhi;
	}

	Phi = Phi_1;

	for (i = iNewLE; i < NewPointCount; ++i) {
		s = sLE + (sTE - sLE) * 0.5 * (1.0 + Radius * cos(Phi));
		Points.emplace_back(DoublePoint(theXSpline.EvalPoint(s), theYSpline.EvalPoint(s)));
		Phi -= DeltaPhi;
	}

	PointCount = NewPointCount;
	UpdateProps(Center);
	UpdateCamberAndThickness();

}

DoublePoint AirfoilElement::getBounds(int boundsType)
{
	return boundsType == 0 ? DoublePoint(xMin, yMin) : DoublePoint(xMax, yMax);
}

void AirfoilElement::FlipY(DoublePoint Pivot)
{
	for (int i = 0; i < PointCount; ++i) {
		Points[i].dY = 2.0 * Pivot.dY - Points[i].dY;
	}

	ReversePointSequence();
	UpdateProps(Center);
	UpdateCamberAndThickness();
}

void AirfoilElement::InitElement()
{
	PointCount = 0;
	Cl = 0.0;
	Cm = 0.0;
	Center.dX = 0.25;
	Center.dY = 0.0;
	Area = 0.0;
}

void AirfoilElement::BSpline(int nPolynomDegree, vector<DoublePoint>& ptControl)
{
	int n = (int)(ptControl.size() - 1);
	int t = nPolynomDegree + 1;
	vector<int> u(n + t + 1);
	//int[] u = new int[n + t + 1];
	ComputeIntervals(u, n, t);
	double dUStep = (double)(n - t + 2) / (double)(PointCount - 1);
	double dU = 0.0;
	int i = 0;
	Points[i].dX = ptControl[i].dX;
	Points[i].dY = ptControl[i].dY;
	++i;

	while (i < PointCount - 1) {
		dU += dUStep;
		DoublePoint calcxyz = compute_point(u, n, t, dU, ptControl);
		Points[i].dX = calcxyz.dX;
		Points[i].dY = calcxyz.dY;
		++i;
	}

	Points[i].dX = ptControl[n].dX;
	Points[i].dY = ptControl[n].dY;

}

void AirfoilElement::ComputeIntervals(vector<int>& u, int n, int t)
{
	for (int j = 0; j <= n + t; ++j) {
		if (j < t) {
			u[j] = 0;
		}
		else if (j >= t && j <= n) {
			u[j] = j - t + 1;
		}
		else if (j > n) {
			u[j] = n - t + 2;
		}
	}
}

DoublePoint AirfoilElement::compute_point(vector<int>& u, int n, int t, double v, vector<DoublePoint>& control)
{
	DoublePoint ptOutput;// = new DoublePoint(0.0, 0.0);

	for (int k = 0; k <= n; ++k) {
		double temp = blend(k, t, u, v);
		ptOutput.dX += control[k].dX * temp;
		ptOutput.dY += control[k].dY * temp;
	}

	return ptOutput;
}

void AirfoilElement::RotatePoints(vector<DoublePoint>& Points, DoublePoint Pivot, double DeltaAngle) {
	for (int i = 0; i < Points.size(); ++i) {
		DoublePoint Radius(Points[i].dX - Pivot.dX, Points[i].dY - Pivot.dY);
		double Angle = atan2(Radius.dY, Radius.dX);
		Angle -= DeltaAngle;
		Points[i].dX = Pivot.dX + Radius.getLength() * cos(Angle);
		Points[i].dY = Pivot.dY + Radius.getLength() * sin(Angle);
	}

}

void AirfoilElement::UpdateCamberAndThickness()
{
	AirfoilElement theCopy(*this);
	int nLE = theCopy.m_LEIndex;
	double dX = theCopy.Points[0].dX - theCopy.Points[nLE].dX;
	double dY = theCopy.Points[0].dY - theCopy.Points[nLE].dY;
	double dAngle = atan2(dY, dX);
	RotatePoints(theCopy.Points, theCopy.Points[nLE], dAngle);
	m_CamberLine.reserve(nLE + 1);
	m_Thickness.reserve(nLE + 1);

	for (int i = 0; i <= nLE; ++i) {
		double YLower = theCopy.getLowerY(theCopy.Points[i].dX);
		double YUpper = theCopy.getUpperY(theCopy.Points[i].dX);
		m_CamberLine[i] = DoublePoint(theCopy.Points[i].dX, 0.5 * (YUpper + YLower));
		m_Thickness[i] = DoublePoint(theCopy.Points[i].dX, YUpper - YLower);
	}

	DoublePoint ptCamberMax = m_CamberLine[0];
	DoublePoint ptCamberMin = m_CamberLine[0];
	ptThickness = m_Thickness[0];

	int i;
	for (i = 0; i < m_CamberLine.size(); ++i) {
		if (m_CamberLine[i].dY > ptCamberMax.dY) {
			ptCamberMax.dX = m_CamberLine[i].dX;
			ptCamberMax.dY = m_CamberLine[i].dY;
		}

		if (m_CamberLine[i].dY < ptCamberMin.dY) {
			ptCamberMin.dX = m_CamberLine[i].dX;
			ptCamberMin.dY = m_CamberLine[i].dY;
		}

		if (m_Thickness[i].dY > ptThickness.dY) {
			ptThickness.dX = m_Thickness[i].dX;
			ptThickness.dY = m_Thickness[i].dY;
		}
	}

	if (abs(ptCamberMax.dY) > abs(ptCamberMin.dY)) {
		ptCamber = ptCamberMax;
	}
	else {
		ptCamber = ptCamberMin;
	}

	DoublePoint var10000 = ptCamber;
	var10000.dY -= m_CamberLine[0].dY;
	if (abs(ptCamber.dY) < 1.0E-5) {
		ptCamber.dX = 0.0;
		ptCamber.dY = 0.0;
	}

	if (abs(ptThickness.dY) < 1.0E-5) {
		ptThickness.dX = 0.0;
		ptThickness.dY = 0.0;
	}

	RotatePoints(m_CamberLine, m_CamberLine[nLE], -dAngle);
	m_Curvature.reserve(PointCount);

	for (i = 1; i < PointCount - 1; ++i) {
		DoublePoint pt1 = Points[i - 1];
		DoublePoint pt2 = Points[i];
		DoublePoint pt3 = Points[i + 1];
		Circle c(pt1, pt2, pt3);
		m_Curvature[i] = 1.0 / c.radius;
	}

	m_Curvature[0] = m_Curvature[1];
	m_Curvature[PointCount - 1] = m_Curvature[PointCount - 2];

}

double AirfoilElement::blend(int k, int t, vector<int>& u, double v)
{
	double value;
	if (t == 1) {
		if ((double)u[k] <= v && v < (double)u[k + 1]) {
			value = 1.0;
		}
		else {
			value = 0.0;
		}
	}
	else if (u[k + t - 1] == u[k] && u[k + t] == u[k + 1]) {
		value = 0.0;
	}
	else if (u[k + t - 1] == u[k]) {
		value = ((double)u[k + t] - v) / (double)(u[k + t] - u[k + 1]) * blend(k + 1, t - 1, u, v);
	}
	else if (u[k + t] == u[k + 1]) {
		value = (v - (double)u[k]) / (double)(u[k + t - 1] - u[k]) * blend(k, t - 1, u, v);
	}
	else {
		value = (v - (double)u[k]) / (double)(u[k + t - 1] - u[k]) * blend(k, t - 1, u, v) + ((double)u[k + t] - v) / (double)(u[k + t] - u[k + 1]) * blend(k + 1, t - 1, u, v);
	}

	return value;
}

