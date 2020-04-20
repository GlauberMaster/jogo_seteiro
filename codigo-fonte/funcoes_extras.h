double retornaAngulo(int playerX, int playerY, int mouseX, int mouseY)
{
	float angulo = 0;
	int deltaX = mouseX - playerX;
	int deltaY = mouseY - playerY;
	angulo = atan2(deltaY, deltaX);
	if(angulo < 0)
	{
		angulo *= -1;
	}
	else if(angulo == 0)
	{
		angulo += 0.1;	
	}
		
	if(angulo <= 1 && angulo >= 0)
		angulo = 1;
		
	return angulo;
}
