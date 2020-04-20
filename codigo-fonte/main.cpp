#include <math.h>
#include <stdio.h>
#include <locale.h>

//allegro5 includes
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

//bibliotecas personalizadas
#include "objetos.h"
#include "funcoes_extras.h"

//Variáveis globais
const int WIDTH = 800;
const int HEIGHT = 600;
const int FPS = 60;
const int NUM_SETAS = 5;
const int NUM_NUVENS = 4;
enum{MAIN, FASE1, FASE2, FASE3, FASE4, FASE5, GANHOU, FASE6, SOBRE};
int telaAtual = MAIN;
bool passouFase = false;
bool gameOver = false;
bool permissaoParaAtirar = true;


//variaveis allegro
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;

//pré-definição de funções

//Nuvem
void inicializaNuvens(Nuvem nuvens[], int size);
void desenhaNuvens(Nuvem nuvens[], int size, ALLEGRO_BITMAP *img_nuvens);
void moverNuvens(Nuvem nuvens[], int size);

//setas
void atualizaSeta(Seta seta[], int size, bool delayGravidade, Alvo &alvo);

int main(void)
{
	//variáveis locais
	bool done = false;
	bool mouse = false;
	double valorAngulo = 0;
	int frameAtual = 0;
	int frameAtualForca = 0;
	int forca = 0;
	
	setlocale(LC_ALL, "portuguese");
	//variaveis de objetos
	Nuvem nuvens[NUM_NUVENS];
	Seta setas[NUM_SETAS];
	Player player1, player2;
	Alvo alvo;
	
	//inicializar o programa
	if(!al_init())
	{
		printf("Erro ao inicializar o programa");
		return -1;
	}
	display = al_create_display(WIDTH, HEIGHT);
	if(!display)
	{
		printf("Erro ao iniciar didsplay");
		return -1;
	}
	
	
	//iniciando addons
	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	
	//carregando imagens
	//tela principal
	ALLEGRO_BITMAP *background = al_load_bitmap("imagens/ceu.png");
	ALLEGRO_BITMAP *logotipo = al_load_bitmap("imagens/logotipo.png");
	ALLEGRO_BITMAP *btn_play = al_load_bitmap("imagens/icone_play.png");
	ALLEGRO_BITMAP *btn_sobre = al_load_bitmap("imagens/icone_sobre.png");
	ALLEGRO_BITMAP *img_nuvem = al_load_bitmap("imagens/nuvem.png");
	//fases
	ALLEGRO_BITMAP *background_stage1 = al_load_bitmap("imagens/bg01.png");
	ALLEGRO_BITMAP *background_stage2 = al_load_bitmap("imagens/bg02.png");
	ALLEGRO_BITMAP *background_stage3 = al_load_bitmap("imagens/bg03.png");
	ALLEGRO_BITMAP *background_stage4 = al_load_bitmap("imagens/bg04.png");
	ALLEGRO_BITMAP *background_stage5 = al_load_bitmap("imagens/bg05.png");
	ALLEGRO_BITMAP *playerImg = al_load_bitmap("imagens/player.png");
	ALLEGRO_BITMAP *alvoImg = al_load_bitmap("imagens/alvo_terrestre.png");
	ALLEGRO_BITMAP *flecha = al_load_bitmap("imagens/flecha.png");
	ALLEGRO_BITMAP *barraCarregamento = al_load_bitmap("imagens/barraCarregamento.png");
	ALLEGRO_BITMAP *bgSobre = al_load_bitmap("imagens/bgsobre.png");
	ALLEGRO_BITMAP *logo_ifba = al_load_bitmap("imagens/logo_ifba.png");
	ALLEGRO_BITMAP *icone = al_load_bitmap("imagens/icone.png");
	
	//animção player 2 imagens
	ALLEGRO_BITMAP *playerFrames[2];
	playerFrames[0] = al_load_bitmap("imagens/player1.png");
	playerFrames[1] = al_load_bitmap("imagens/player2.png");
	int playerFrameAtual = 0;
	
	//carregando fontes
	ALLEGRO_FONT *adventureFont = al_load_ttf_font("fontes/3Dventure.ttf", 40, 0);
	ALLEGRO_FONT *pressStart2P40 = al_load_ttf_font("fontes/PressStart2P.ttf", 40, 0);
	ALLEGRO_FONT *pressStart2P15 = al_load_ttf_font("fontes/PressStart2P.ttf", 10, 0);
	ALLEGRO_FONT *pressStart2P10 = al_load_ttf_font("fontes/PressStart2P.ttf", 10, 0);
	
	//criando temporizador e fila de eventos
	timer = al_create_timer(1.0 / FPS);
	event_queue = al_create_event_queue();
	
	//definindo icone do jogo
	al_set_display_icon(display, icone);
	
	
	//adcionando itens à fila de eventos e iniciando o temporizador
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	
	al_start_timer(timer);
	
	//inicializando objetos
	inicializaNuvens(nuvens, NUM_NUVENS);
	player1.inicializa(0, 380);
	for (int i = 0; i < NUM_SETAS; i++)
		setas[i].inicializa(player1);
	alvo.inicializa(700, 300, 60, 60);
	
	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
			if(telaAtual != MAIN && telaAtual != SOBRE && telaAtual != GANHOU)
				if(!passouFase && !gameOver)
				{
					if(permissaoParaAtirar)
						mouse = true;
				}
			}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
			//eventos do mouse para tela main
			if(telaAtual == MAIN)
			{
				if(ev.mouse.x > (WIDTH / 2 - 50) &&
		   		   ev.mouse.x < (WIDTH / 2 + 50) &&
		   		   ev.mouse.y > (HEIGHT / 2 - 50) &&
		  		   ev.mouse.y < (HEIGHT / 2 + 50))
				{
				   	telaAtual = FASE1;
				}
				else if(ev.mouse.x > (WIDTH - 100) &&
			   		   ev.mouse.x < (WIDTH - 100 + 64) &&
		      		   ev.mouse.y > (HEIGHT - 100) &&
			  		   ev.mouse.y < (HEIGHT - 100 + 64))
				{
				   	telaAtual = SOBRE;
				}
			}
			
			else if(telaAtual == SOBRE){}
			else if(telaAtual == GANHOU){}
			else
			{
				if(!passouFase && !gameOver)
					{
						if(permissaoParaAtirar && mouse)
						{
							mouse = false;
							valorAngulo = retornaAngulo(player1.x + 50, player1.y + 50, ev.mouse.x, ev.mouse.y);
							
							//Esquema para o disparo da flecha
							for(int i = 0; i < NUM_SETAS; i++)
							{
								if(i == 0 || setas[i - 1].colidiu)
								{
									if(!setas[i].live)
									{
										setas[i].dispara(valorAngulo, forca);
										break;	
									}
								}
							}
							forca = 0;
							playerFrameAtual = 0;
							permissaoParaAtirar = false;	
						}
					}
			}							
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
				//aqui ele volta para tela inicial, independente da tela atual
				case ALLEGRO_KEY_ESCAPE:
					telaAtual = MAIN;
					passouFase = false;
					gameOver = false;
					permissaoParaAtirar = true;
					for(int i = 0; i < NUM_SETAS; i++)
						setas[i].inicializa(player1);
					break;
				//Aqui ele alterna entre as fases
				case ALLEGRO_KEY_ENTER:
					if(passouFase)
					{
						telaAtual++;
						for(int i = 0; i < NUM_SETAS; i++)
							setas[i].inicializa(player1);
						passouFase = false;
					}
			}
		}
		else if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			switch(telaAtual)
			{
				case MAIN:
					moverNuvens(nuvens, NUM_NUVENS);
					break;
						
				case FASE1:
					//velocidade da gravidade
					frameAtual++;
					if(frameAtual >= 4){
						atualizaSeta(setas, NUM_SETAS, true, alvo);
						frameAtual = 0;
					}
					else
					{
						atualizaSeta(setas, NUM_SETAS, false, alvo);
					}
					
					
					//forca da flecha
					if(!passouFase && !gameOver)
					if(mouse)
					{
						playerFrameAtual = 1;
						frameAtualForca++;
						if(frameAtualForca >= 5)
						{
							frameAtualForca = 0;
							forca++;
							if(forca > 15)
							{
								forca = 15;
							}
						}
					}
					
					
					break;
				case FASE2:
					//velocidade da gravidade
					frameAtual++;
					if(frameAtual >= 3){
						atualizaSeta(setas, NUM_SETAS, true, alvo);
						frameAtual = 0;
					}
					else
					{
						atualizaSeta(setas, NUM_SETAS, false, alvo);
					}
					
					
					//forca da flecha
					if(!passouFase && !gameOver)
					if(mouse)
					{
						playerFrameAtual = 1;
						frameAtualForca++;
						if(frameAtualForca >= 5)
						{
							frameAtualForca = 0;
							forca++;
							if(forca > 20)
							{
								forca = 20;
							}
						}
					}
					break;
				case FASE3:
					//velocidade da gravidade
					frameAtual++;
					if(frameAtual >= 2){
						atualizaSeta(setas, NUM_SETAS, true, alvo);
						frameAtual = 0;
					}
					else
					{
						atualizaSeta(setas, NUM_SETAS, false, alvo);
					}
					
					
					//forca da flecha
					if(!passouFase && !gameOver)
					if(mouse)
					{
						playerFrameAtual = 1;
						frameAtualForca++;
						if(frameAtualForca >= 5)
						{
							frameAtualForca = 0;
							forca++;
							if(forca > 25)
							{
								forca = 25;
							}
						}
					}
					break;
				case FASE4:
					//velocidade da gravidade
					frameAtual++;
					if(frameAtual >= 1){
						atualizaSeta(setas, NUM_SETAS, true, alvo);
						frameAtual = 0;
					}
					else
					{
						atualizaSeta(setas, NUM_SETAS, false, alvo);
					}
					
					
					//forca da flecha
					if(!passouFase && !gameOver)
					if(mouse)
					{
						playerFrameAtual = 1;
						frameAtualForca++;
						if(frameAtualForca >= 5)
						{
							frameAtualForca = 0;
							forca++;
							if(forca > 30)
							{
								forca = 30;
							}
						}
					}
					break;
				case FASE5:
					//velocidade da gravidade
					frameAtual++;
					if(frameAtual >= 0){
						atualizaSeta(setas, NUM_SETAS, true, alvo);
						frameAtual = 0;
					}
					else
					{
						atualizaSeta(setas, NUM_SETAS, false, alvo);
					}
					
					
					//forca da flecha
					if(!passouFase && !gameOver)
					if(mouse)
					{
						playerFrameAtual = 1;
						frameAtualForca++;
						if(frameAtualForca >= 5)
						{
							frameAtualForca = 0;
							forca++;
							if(forca > 35)
							{
								forca = 35;
							}
						}
					}
					break;
				case GANHOU:
					break;
				case SOBRE:
					break;
			}
		}
		
		//DESENHANDO ELEMENTOS DA TELA
		switch(telaAtual)
		{
			case MAIN:
				al_draw_scaled_bitmap(background, 0, 0, 80, 60, 0, 0, 800, 600, 0);
				desenhaNuvens(nuvens, NUM_NUVENS, img_nuvem);
				al_draw_bitmap(logotipo, 100, HEIGHT / 2 - 200, 0);
				al_draw_scaled_bitmap(btn_play, 0, 0, 32, 32, WIDTH / 2 - 50, HEIGHT / 2 - 50, 100, 100, 0);
				al_draw_scaled_bitmap(btn_sobre, 0, 0, 32, 32, WIDTH - 100, HEIGHT - 100, 64, 64, 0);
				break;
			case FASE1:
				al_draw_scaled_bitmap(background_stage1, 0, 0, 64, 64, 0, 0, 800, 600, 0);
				break;	
			case FASE2:
				al_draw_scaled_bitmap(background_stage2, 0, 0, 64, 64, 0, 0, 800, 600, 0);
				break;
			case FASE3:
				al_draw_scaled_bitmap(background_stage3, 0, 0, 64, 64, 0, 0, 800, 600, 0);
				break;
			case FASE4:
				al_draw_scaled_bitmap(background_stage4, 0, 0, 64, 64, 0, 0, 800, 600, 0);
				break;
			case FASE5:
				al_draw_scaled_bitmap(background_stage5, 0, 0, 64, 64, 0, 0, 800, 600, 0);
				break;
				
			case GANHOU:
				al_draw_scaled_bitmap(bgSobre, 0, 0, 64, 64, 0, 0, 800, 600, 0);
				al_draw_text(pressStart2P40, al_map_rgb(0, 0, 0), WIDTH / 2, 100, ALLEGRO_ALIGN_CENTRE,  "Parabens");
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 100, 180, 0,  "Atualmente o jogo so possui 5 fases");
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 100, 200, 0,  "Esperamos que tenha se divertido jogando");
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 100, 240, 0,  "Tente zerar novamente! nao conte com a sorte desta vez. haha");
			
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 490, 500, 0,  "ESC - Voltar");
				break;
				
				
			case SOBRE:
				al_draw_scaled_bitmap(bgSobre, 0, 0, 64, 64, 0, 0, 800, 600, 0);
				al_draw_scaled_bitmap(logo_ifba, 0, 0, 160, 240, 100, 100, 80, 120, 0);
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 240, 100, 0,  "Este jogo foi feito como requisito da materia");
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 240, 120, 0,  "Programacao Estruturada. Tendo como unico");
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 240, 140, 0,  "objetivo o aprendizado");
				
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 100, 260, 0,  "Autores");
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 100, 280, 0,  "Glauber Castro da Silva");
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 100, 300, 0,  "Oseias Gomes Guerra");
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 100, 320, 0,  "Paulo Eduardo Gomes Santos");
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 100, 340, 0,  "Wanderson Guilherme Costa Goncalves");
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 100, 380, 0,  "Orientadora");
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 100, 400, 0,  "Marlecia Ferreira Sanders");
				
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 100, 440, 0,  "Creditos");
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 100, 460, 0,  "Sprite do arqueiro: Shutterstock");
				
				al_draw_text(pressStart2P15, al_map_rgb(0, 0, 0), 490, 500, 0,  "ESC - Voltar");
				break;
		}
		
		if(telaAtual >= 1 && telaAtual < 6)
		{
			alvo.desenha(alvoImg, 700, 410);
			al_draw_scaled_bitmap(barraCarregamento, 0, 0, 10, 10, player1.x + 20, player1.y - 20, 5 * forca, 10, 0);
			player1.desenhaPlayer(playerFrames[playerFrameAtual]);
			for(int j = 0; j < NUM_SETAS; j++)
				setas[j].desenha(flecha);
				
			//desenhando quandidade de flechas no HUD
			for(int i = 0; i < NUM_SETAS; i++)
			{
				if(!setas[i].live)
					al_draw_rotated_bitmap(flecha, 16, 4 , 17*(i+1), 25, -1.5708, 0);
			}	
			//desenhando nome da fase no HUD
			al_draw_textf(adventureFont, al_map_rgb(0, 0, 0), 600, 10, 0, "FASE %i", telaAtual);
		}
		
		if(passouFase)
		{
			al_draw_text(pressStart2P40, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "NIVEL COMPLETO");
			al_draw_text(pressStart2P15, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2 + 50, ALLEGRO_ALIGN_CENTRE, "Tecle ENTER para ir ao proximo nivel");
		}

		if(gameOver && !passouFase)
		{
			al_draw_text(pressStart2P40, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
			al_draw_text(pressStart2P15, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2 + 50, ALLEGRO_ALIGN_CENTRE, "Tecle ESC para voltar a tela inicial");
		}
		al_flip_display();
		al_clear_to_color(al_map_rgb(255, 255, 255));
	}
	//encerrando programa
	al_destroy_display(display);
	
	return 0;
}

//Funções

//nuvens da tela inicial
void inicializaNuvens(Nuvem nuvens[], int size)
{
	for(int i = 0; i < size; i++)
	{
		nuvens[i].x = i * 250;
		nuvens[i].y = (i + 1) * 50;
		nuvens[i].live = true;
		nuvens[i].speed = 1;
	}
}
void desenhaNuvens(Nuvem nuvens[], int size, ALLEGRO_BITMAP *img_nuvem)
{
	for(int i = 0; i < size; i++)
	{
		if(nuvens[i].live)
		{
			al_draw_scaled_bitmap(img_nuvem, 0, 0, 32, 32, nuvens[i].x, nuvens[i].y, 100, 100, 0);
		}
	}
}
void moverNuvens(Nuvem nuvens[], int size)
{
	for(int i = 0; i < size; i++)
	{
		if(nuvens[i].live)
		{
			nuvens[i].x += nuvens[i].speed;
			
			if(nuvens[i].x >= WIDTH + 50)
			{
				nuvens[i].x = 0;
			}
		}
	}
}

//Flecha
void atualizaSeta(Seta seta[], int size, bool delayGravidade, Alvo &alvo)
{
	for(int i = 0; i < size; i++)
	{
		if(seta[i].live && !seta[i].colidiu)
		{
			if(delayGravidade)
			{
					seta[i].speedY++;
			}
			seta[i].x += seta[i].speedX;
			seta[i].y += seta[i].speedY;
			seta[i].angulo = atan2(seta[i].speedY, seta[i].speedX);
			//colisao com o chao
			if(seta[i].y >= 700)
			{
				seta[i].colidiu = true;
			}
			
			//colisão com o alvo
			if(seta[i].x > (alvo.x + alvo.centro - alvo.raioX*0.5) &&
		   	   seta[i].x < (alvo.x + alvo.centro + alvo.raioX) &&
		       seta[i].y > (alvo.y + alvo.centro - alvo.raioY) &&
			   seta[i].y < (alvo.y + alvo.centro + alvo.raioY))
					{
					   	seta[i].colidiu = true;
					   	passouFase = true;
					}
			
			if(seta[i].colidiu)
			{
				seta[i].speedX = 0;
				seta[i].speedY = 0;
				permissaoParaAtirar = true;
			}
			
			//teste Game Over
			bool restamFlechas = false;
			for(int j = 0; j < size; j++)
			{
				if(!seta[j].colidiu)
				{
					restamFlechas = true;
				}
			}
			if(!restamFlechas && !passouFase)
			{
				gameOver = true;
			}
		
		}
	}
}
