#ifndef UI_CLASSES
#define UI_CLASSES

class button
{
	bool selected = false;
	size_t col[8];
	size_t tcol[8];
	SDL_Texture * texture;
	SDL_Rect mask;
	int cost;
	bool dark;
public:
	button(string,int*,int*,vec2,vec2);
	button(string,int*,int*,vec2,vec2,int);
	void select();
	void updateText(string);
	void set(bool s) {selected = s;}
	bool on() {return selected;}
	void draw();
	void del();
	SDL_Rect * getRekt() {return &mask;}
	
	int getCost() {return cost;}
	void setCost(int pcost) {cost = pcost;}
	
	void setDark(bool b) {dark = b;}
	bool isDark() {return dark;}
};

button::button(string txt, int b_col[], int t_col[], vec2 pos, vec2 dim)
{
	dark = false;
	selected = false;
	
	TTF_Font * Font_Pixelade = TTF_OpenFont("pix.TTF", 18);
	texture = renderText(txt, Font_Pixelade, {255,255,255,255}, renderer, I_MAX);
	TTF_CloseFont(Font_Pixelade);
	
	for(size_t i = 0; i < 8; i++)
	{
		col[i] = b_col[i];
	}
	for(size_t i = 0; i < 8; i++)
	{
		tcol[i] = t_col[i];
	}
	
	mask.x = pos.x;
	mask.y = pos.y;
	mask.w = dim.x;
	mask.h = dim.y;
	
	cost = 0;
}

button::button(string txt, int b_col[], int t_col[], vec2 pos, vec2 dim, int pcost)
{
	dark = true;
	selected = false;
	
	TTF_Font * Font_Pixelade = TTF_OpenFont("pix.TTF", 18);
	texture = renderText(txt, Font_Pixelade, {255,255,255,255}, renderer, I_MAX);
	TTF_CloseFont(Font_Pixelade);
	
	for(size_t i = 0; i < 8; i++)
	{
		col[i] = b_col[i];
	}
	for(size_t i = 0; i < 8; i++)
	{
		tcol[i] = t_col[i];
	}
	
	mask.x = pos.x;
	mask.y = pos.y;
	mask.w = dim.x;
	mask.h = dim.y;
	
	cost = pcost;
}

void button::updateText(string text)
{
	TTF_Font * Font_Pixelade = TTF_OpenFont("pix.TTF", 18);
	texture = renderText(text, Font_Pixelade, {255,255,255,255}, renderer, I_MAX);
	TTF_CloseFont(Font_Pixelade);
}

void button::select()
{
	if(selected)
	{
		selected = false;
		SDL_SetTextureColorMod(texture,tcol[0],tcol[1],tcol[2]);
		SDL_SetTextureAlphaMod(texture,tcol[3]);
	}
	else 
	{
		selected = true;
		SDL_SetTextureColorMod(texture,tcol[4],tcol[5],tcol[6]);
		SDL_SetTextureAlphaMod(texture,tcol[7]);
	}
}

void button::draw()
{
	if(!selected)
	{
		SDL_SetRenderDrawColor(renderer,col[0],col[1],col[2],col[3]);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer,col[4],col[5],col[6],col[7]);
	}
	SDL_RenderFillRect(renderer,&mask);
	SDL_RenderCopy(renderer,texture,NULL,&mask);
	
	if(dark)
	{
		SDL_SetRenderDrawColor(renderer,0,0,0,200);
		SDL_RenderFillRect(renderer,&mask);
	}
}

void button::del()
{
	SDL_DestroyTexture(texture);
}

class selection
{
	vector<button> buttons;
	int selected;
public:
	vector<button> * getButtons() {return &buttons;}
	int getSelected() {return selected;}
	void add(button b) {buttons.push_back(b);}
	bool click(vec2);
	void draw();
	button * getAt(size_t i) {return &buttons.at(i);}
};

bool selection::click(vec2 p)
{
	bool any = false;
	for(size_t i = 0; i < buttons.size(); i++)
	{
		if(buttons.at(i).isDark()) continue;
		if(pointInRect(p, buttons.at(i).getRekt()))
		{
			any = true;
		}
	}
	
	if(!any) return false;
	
	for(size_t i = 0; i < buttons.size(); i++)
	{
		buttons.at(i).set(false);
		if(buttons.at(i).isDark()) continue;
		if(pointInRect(p, buttons.at(i).getRekt()))
		{
			buttons.at(i).select();
			selected = i;
		}
	}
	return true;
}

void selection::draw()
{
	for(size_t i = 0; i < buttons.size(); i++)
	{
		buttons.at(i).draw();
	}
}

#endif
