#include "Region.h"

double rotation_angle = 0.0;

Region::Region(int _height, int _width, double _simHeight)
{
    height = _height;
    width = _width;
    simHeight = _simHeight;
    cScale = height / simHeight;
    simWidth = width / cScale;
    geometry = CYLINDER;

#if (DEBUG)
    cout << "------------------" << endl;
    cout << "Region constructor" << endl;
    cout << "------------------" << endl;
    cout << "height = " << height << endl;
    cout << "width = " << width << endl;
    cout << "simHeight = " << simHeight << endl;
    cout << "cScale = " << cScale << endl;
    cout << "simWidth = " << simWidth << endl;
#endif
}

void Region::setupRegion(int _RegionNr, double _overRelaxation, int _resolution, double _density, int _numThreads)
{
    this->RegionNr = _RegionNr;
    this->characteristic_length = 0.15;
    this->overRelaxation = _overRelaxation;
    this->numThreads = _numThreads;

    this->dt = 1.0 / 60.0;
    this->numIters = 40;

    this->geometryX = 0;
    this->geometryY = 0;
    this->resolution = _resolution;

    int res = this->resolution;

    if (RegionNr == 1)
        res = 2 * this->resolution;

    double domainHeight = 1.0;
    double domainWidth = domainHeight / simHeight * simWidth;
    double h = domainHeight / res;

    int numX = floor(domainWidth / h);
    int numY = floor(domainHeight / h);

    double density = _density;

    this->fluid = make_shared<Fluid>(density, numX, numY, h, overRelaxation, numThreads);
    shared_ptr<Fluid> f = this->fluid;

    int n = f->numY;
    Point pos = { 0.4, 0.5 };

    if (RegionNr == 2)
    { // tank
#pragma omp parallel for schedule(static) num_threads(numThreads)
        for (int i = 0; i < f->numX; i++)
        {
            for (int j = 0; j < f->numY; j++)
            {
                double s = 1.0; // fluid
                if (i == 0 || i == f->numX - 1 || j == 0)
                    s = 0.0; // solid
                f->s[i * n + j] = s;
            }
        }
        setGeometry(pos.x, pos.y, true);
        // CALL_SET_GEOMETRY(this,setGeometry)(0.4, 0.5, true);
        this->gravity = -9.81;
        this->showPressure = true;
        this->showSmoke = false;
        this->showStreamlines = false;
        this->showVelocity = false;
        this->showXVelocity = false;
        this->showYVelocity = false;
        this->showVelocityVectors = false;
        this->showGeometry = true;
        this->showGeometryPosition = false;
    }
    else if (RegionNr == 0 || RegionNr == 1)
    { // vortex shedding

        double inVel = 2.0;
#pragma omp parallel for schedule(static) num_threads(numThreads)
        for (int i = 0; i < f->numX; i++)
        {
            for (int j = 0; j < f->numY; j++)
            {
                double s = 1.0; // fluid
                if (i == 0 || j == 0 || j == f->numY - 1)
                    s = 0.0; // solid

                f->s[i * n + j] = s;

                if (i == 1)
                {
                    f->u[i * n + j] = inVel;
                }
            }
        }

        double pipeH = 0.1 * f->numY;
        int minJ = floor(0.5 * f->numY - 0.5 * pipeH);
        int maxJ = floor(0.5 * f->numY + 0.5 * pipeH);

        for (int j = minJ; j < maxJ; j++)
            f->m[j] = 0.0;

        setGeometry(pos.x, pos.y, true);

        this->gravity = 0.0;
        this->showPressure = false;
        this->showSmoke = true;
        this->showStreamlines = false;
        this->showVelocity = false;
        this->showXVelocity = false;
        this->showYVelocity = false;
        this->showVelocityVectors = false;

        if (RegionNr == 1)
        {
            this->dt = 1.0 / 60.0; // 1/120
            this->numIters = 40;   // 100
            this->showPressure = true;
        }
    }
    else if (RegionNr == 3)
    { // paint
        this->gravity = 0.0;
        this->overRelaxation = 1.0;
        this->showPressure = false;
        this->showSmoke = true;
        this->showStreamlines = false;
        this->showVelocity = false;
        this->showXVelocity = false;
        this->showYVelocity = false;
        this->showVelocityVectors = false;
        this->characteristic_length = 0.075;
        this->showGeometry = true;
        this->showGeometryPosition = false;
        setGeometry(pos.x, pos.y, true);
    }

    //    document.getElementById("streamButton").checked = this->showStreamlines;
    //    document.getElementById("velocityButton").checked = this->showVelocity;
    //    document.getElementById("pressureButton").checked = this->showPressure;
    //    document.getElementById("smokeButton").checked = this->showSmoke;
    //    document.getElementById("overrelaxButton").checked = this->overRelaxation > 1.0;
}

void Region::setGeometry(double x, double y, bool reset)
{
    switch (geometry)
    {
    case CYLINDER:
        setGeometryCylinder(x, y, reset);
        break;
    case NACA:
        setGeometryNaca(x, y, reset);
        break;
    default:
        setGeometryCylinder(x, y, reset);
    }
}

void Region::setGeometryCylinder(double x, double y, bool reset)
{

    double vx = 0.0;
    double vy = 0.0;

    if (!reset)
    {
        vx = (x - this->geometryX) / this->dt;
        vy = (y - this->geometryY) / this->dt;
    }

    this->geometryX = x;
    this->geometryY = y;
    double r = this->characteristic_length;
    shared_ptr<Fluid> f = this->fluid;
    int n = f->numY;
    //    double cd = sqrt(2) * f->h;
#pragma omp parallel for schedule(static) num_threads(fluid->numThreads)
    for (int i = 1; i < f->numX - 2; i++)
    {
        for (int j = 1; j < f->numY - 2; j++)
        {

            f->s[i * n + j] = 1.0;

            double dx = (i + 0.5) * f->h - x;
            double dy = (j + 0.5) * f->h - y;

            if (dx * dx + dy * dy < r * r)
            {
                f->s[i * n + j] = 0.0;
                if (this->RegionNr == 3)
                    f->m[i * n + j] = 0.5 + 0.5 * sin(0.1 * this->frameNr);
                else
                {
                    f->m[i * n + j] = 1.0;
                    this->frameNr = 0;
                }

                f->u[i * n + j] = vx;
                f->u[(i + 1) * n + j] = vx;
                f->v[i * n + j] = vy;
                f->v[i * n + j + 1] = vy;
            }
        }
    }

    this->showGeometry = true;
    //    this->showGeometryPosition=true;
}



void Region::setGeometryNaca(double x, double y, bool reset)
{
    double vx = 0.0;
    double vy = 0.0;

    if (!reset)
    {
        vx = (x - this->geometryX) / this->dt;
        vy = (y - this->geometryY) / this->dt;
    }

    this->geometryX = x;
    this->geometryY = y;
    double r = this->characteristic_length;
    shared_ptr<Fluid> f = this->fluid;
    int n = f->numY;
    Point center = { x, y };
    vector<Point> P = getNacaPoints(center, r);
    //    double cd = sqrt(2) * f->h;
#pragma omp parallel for schedule(static) num_threads(f->numThreads)
    for (int i = 1; i < f->numX - 2; i++)
    {
        for (int j = 1; j < f->numY - 2; j++)
        {

            f->s[i * n + j] = 1.0;

            // double dx = (i + 0.5) * f->h - x;
            // double dy = (j + 0.5) * f->h - y;

            // //! axis change by a rotation of theta=pi/4
            // double dxb = sqrt(2) / 2 * (dx + dy);
            // double dyb = sqrt(2) / 2 * (-dx + dy);

            // if (fabs(dxb) < r && fabs(dyb) < r)
            // Point M = {(i + 0.5) * f->h ,(j + 0.5) * f->h };

            if (isInsidePolygon(P, Point({ (i + 0.5) * f->h, (j + 0.5) * f->h })))
            {
                f->s[i * n + j] = 0.0;
                if (this->RegionNr == 3)
                    f->m[i * n + j] = 0.5 + 0.5 * sin(0.1 * this->frameNr);
                else
                {
                    f->m[i * n + j] = 1.0;
                    this->frameNr = 0;
                }

                f->u[i * n + j] = vx;
                f->u[(i + 1) * n + j] = vx;
                f->v[i * n + j] = vy;
                f->v[i * n + j + 1] = vy;
            }
        }
    }

    this->showGeometry = true;
}


void Region::updateRegionSize(int _height, int _width)
{

    height = _height;
    width = _width;
    cScale = height / simHeight;
    simWidth = width / cScale;

#if (DEBUG)
    cout << "----------------" << endl;
    cout << "updateRegionSize" << endl;
    cout << "----------------" << endl;
    cout << "height = " << height << endl;
    cout << "width = " << width << endl;
    cout << "simHeight = " << simHeight << endl;
    cout << "cScale = " << cScale << endl;
    cout << "simWidth = " << simWidth << endl;
#endif
}

void Region::update()
{
    if (!paused)
    {
        rotation_angle = (rotation_angle > 360.0) ? 0.0 : rotation_angle + 0.025;
        fluid->simulate(dt, gravity, numIters);
    }
}

OBJ indexToOBJ(int index)
{
    switch (index)
    {
    case 0:
        return CYLINDER;
    case 1:
        return NACA;
    default:
        return CYLINDER;
    }
}