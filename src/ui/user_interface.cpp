#include "util.hpp"
#include "vectors.hpp"
#include "ui/user_interface.hpp"
#include <iostream>

std::array< std::string, 10> g_ROMAN_NUMS = {"I","II","III","IV","V","VI","VII","IX","X","X"};

userInterface::userInterface()
{

}

selectionReturn userInterface::handleInput(vec2 _pos)
{
	selectionReturn r = {-1, -1};
	button * b = nullptr;

	for(size_t i = 0; i < m_elements.size(); ++i)
	{        
        if(!m_elements[i].click( _pos ))
			continue;

		for(size_t j = 0; j < m_elements[i].getButtons()->size(); ++j)
		{
			b = m_elements[i].getAt(j);
			if(b->on())
			{
				r.m_sel_val = i;
				r.m_button_val = j;
				break;
			}
		}
	}

	//if(m_elements[i].inWorldSpace()) std::cout << "CLICK AT " << _pos.m_x << ", " << _pos.m_y << " world space\n";
	/*if(!m_elements[i].isVisible())
			continue;
		for(size_t j = 0; j < m_elements[i].getButtons()->size(); ++j)
		{
			b = m_elements[i].getAt(j);

			vec2 tempPos = _pos;
			if(m_elements[i].inWorldSpace())
				tempPos = toWorldSpace(_pos);

			if(pointInRect(tempPos, b->getPos(), b->getDim()))
			{
				b->set(true);
				r.m_sel_val = i;
				r.m_button_val = j;

				//Break out of the two nested loops.
				i = m_elements.size();
				break;
			}
		}
	}*/

	/*if(r.m_sel_val != -1 and r.m_button_val != -1)
	{
		for(auto &i : *m_elements[r.m_sel_val].getButtons())
		{
			i.set(false);
		}
		if(b != nullptr) b->set(true);
	}*/
	return r;
}

void userInterface::reset()
{
	for(auto &i : m_elements) i.reset();
}

void userInterface::update(const float _s, const vec2 _mouse)
{
    for(auto &i : m_elements)
        i.update(_s, _mouse);
}
