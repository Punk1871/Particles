#include "Engine.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

Engine::Engine()
{
	VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	m_widow.create(desktopMode, "Particle Generator")
}


void Engine::run()
{
	Clock clock; //create clock object
	bool unit_tests_done = false; // keep track of unit tests.
	

	while (m_Window.isOpen())
	{
		
			if (!unit_tests_done)  //unit test
			{
				unit_tests_done = true;
				cout << "Starting Particle unit tests..." << endl;
				Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
				p.unitTests();
				cout << "Unit tests complete.  Starting engine..." << endl;
			}


			Time deltaTime = clock.restart(); //restart clock

			float dt = deltaTime.asSeconds(); // convert the clock time to seconds.

			input(); //call input
			update(dt); //call update
			draw(); // call draw
		
	}
}

void Engine::input()
{
	Event event;

    while (m_Window.pollEvent(event)) //poll the windows event queue
    {
        switch (event.type) 
        {
        case Event::Closed:
            m_Window.close();
            break;

        case Event::KeyPressed:
            if (event.key.code == Keyboard::Escape)//escpa
                m_Window.close();
            break;
        case Event::MouseButtonPressed:
            if (event.mouseButton.button == Mouse::Left) 
            {

                // Create 5 particles

                for (int i = 0; i < 5; ++i) 
                {
                    //random num between 25 and 50
                    int numPoints = rand() % 26 + 25;

                    //construct particle based on random points and mouse location
                    Particle particle(m_Window, numPoints, { event.mouseButton.x, event.mouseButton.y });

                    //adding the particle to the vector
                    m_particles.push_back(particle);
                }
            }

            break;

        default:
            break;
        }
    }
}

void Engine::update(float dtAsSeconds)
{
    for (auto it = m_particles.begin(); it != m_particles.end();
    {
        if (it->getTTL() > 0.0)
        {
            it -> update(dtAsSeconds); //updating particle
            it++;
        }
        else
        {
            it = m_particles.erase(it); //erasing old particles trying not to bog down the cpu
        }
    }
}

void Engine::draw()
{
    this->m_Window.clear(); //clear the window

    for (const auto& particle : this->m_particles) //loops through each particle in M_Particles
    {
        m_Window.draw(particle);
    }

    this->m_Window.display();
}
