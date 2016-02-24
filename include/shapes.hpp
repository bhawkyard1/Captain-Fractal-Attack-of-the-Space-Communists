struct circle
{
	vec2 center;
	float radius;
};

bool pointInCircle(vec2 &pos, circle &c)
{
	if( magns( c.center - pos ) < sqr( c.radius ) ) return true;
	return false;
}

bool circleIntersectCircle(circle &c1, circle &c2)
{
	if( magns( c2.center - c1.center ) < sqr( c1.radius + c2.radius ) ) return true;
	return false;
}
