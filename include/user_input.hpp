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

