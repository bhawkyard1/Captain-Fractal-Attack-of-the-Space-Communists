#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

//Input handler
void handleKeys( unsigned char key, int x, int y );

void handleKeys( unsigned char key, int x, int y, renderer &r )
{
    //Toggle quad
    if( key == 'q' )
    {
        r.DELETE_rflip();
    }
}
#endif

