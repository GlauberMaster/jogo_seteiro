void desenhaTela()
{
	switch (telaAtual)
	{
		case MAIN:
			al_draw_scaled_bitmap(ceu, 0, 0, 1600, 900, 0, 0, 800, 600, 0);
			al_draw_scaled_bitmap(chao, 0, 0, 1610, 258, 0, 500, 800, 100, 0);
			al_draw_scaled_bitmap(nuvem, 0, 0, 1280, 715, 400, 20, 50, 25);
			al_draw_bitmap(logotipo, WIDTH/2, HEIGHT/2, 0);
			break;
		case FASE1:
			
			break;
		case FASE2:
		
			break;
		case FASE3:
		
			break;
		case FASE4:
		
			break;
		case FASE5:
		
			break;
		case GANHOU:
			
			break;
		case SOBRE:
		
			break;
		case GAME_OVER:
		
			break;
		}
}
