#include "Particle.h"
#include <cstdlib>
#include <ctime>
#include <random>

using namespace sf;
using namespace std;



bool Particle::almostEqual(double a, double b, double eps)
{
	return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    
    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
    {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0,j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition) : m_A(2, numPoints), m_ttl(TTL), m_numPoints(numPoints)
{
    random_device rd; 
    mt19937 gen(rd());
    uniform_real_distribution<float> thetaDis(0.0f, static_cast<float>(M_PI / 2));
    uniform_real_distribution<float> velocityDis(100.0f, 500.0f);
    uniform_real_distribution<float> radiusDis(20.0f, 80.0f);

    this->m_ttl = TTL;
    this->m_numPoints = numPoints;

    m_radiansPerSec = static_cast<float>(rand()) / RAND_MAX * static_cast<float>(M_PI);

    this->m_cartesianPlane.setCenter(0, 0);
    this->m_cartesianPlane.setSize(target.getSize().x, (-1.0)* target.getSize().y);

    this->m_centerCoordinate = Vector2f(round(target.mapPixelToCoords(mouseClickPosition, this->m_cartesianPlane).x), round(target.mapPixelToCoords(mouseClickPosition, this->m_cartesianPlane).y));

    this->m_vx = velocityDis(gen); //random x velocity
    this->m_vy = velocityDis(gen); //random y velocity

    m_color1 = Color::Black; //set color 1 to black;
    m_color2 = Color(rand() % 256, rand() % 256, rand() % 256); //random colors!

    float theta = thetaDis(gen); 
    float dTheta = 2 * M_PI / (numPoints - 1); 

    for (int j = 0; j < numPoints; j++)
    {
        float r = radiusDis(gen);

        float dx = r * cos(theta);
        float dy = r * sin(theta);

        this->m_A(0, j) = m_centerCoordinate.x + dx;
        this->m_A(1, j) = m_centerCoordinate.y + dy;

        theta += dTheta;
    }
}

void Particle::draw(RenderTarget& target, RenderStates states) const
{
    VertexArray lines(TriangleFan, m_numPoints + 1);

    Vector2f pixelCenter = Vector2f(target.mapCoordsToPixel(this->m_centerCoordinate, this->m_cartesianPlane));

    lines[0].position = pixelCenter;
    lines[0].color = this->m_color1;

    for (unsigned short j = 1; j <= m_numPoints; j++)
    {
        lines[j].position = Vector2f(target.mapCoordsToPixel(Vector2f(m_A(0, j - 1), m_A(1, j - 1)), this->m_cartesianPlane));
        lines[j].color = this->m_color2;
    }

    target.draw(lines);
}

void Particle::update(float dt)
{
    this->m_ttl -= dt; //Subtract dt from m_ttl;

    rotate(dt * this->m_radiansPerSec); //rotate the particle

    float shrinkFactor = 0.6f;
    scale(1.0f - shrinkFactor * dt); //scale the particle

    float dx = this->m_vx * dt;
    this->m_vy -= G * dt;
    float dy = this->m_vy * dt; // calculate translation distance

    translate(dx, dy); //actually translating.
}

void Particle::translate(double xShift, double yShift)
{
    TranslationMatrix T(xShift, yShift, this->m_A.getCols()); 

    this->m_A = T + m_A;
    
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift; //updating particles new center coord

}

void Particle::rotate(double theta)
{

    Vector2f temp = m_centerCoordinate; 

    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    RotationMatrix R(theta);

    this->m_A = R * m_A;

    translate(temp.x, temp.y);
}

void Particle::scale(double c)
{
    Vector2f temp = m_centerCoordinate;

    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    ScalingMatrix S(c);

    this->m_A = S * m_A;

    translate(temp.x, temp.y);
}
