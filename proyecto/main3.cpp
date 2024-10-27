#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <allegro.h>

#define MAXANCHO 1280
#define MAXLARGO 720
#define MAXMANCHO 32
#define MAXALTO 18
#define PANCHO 40
#define PLARGO 40
#define ANCHOB 40
#define EANCHO 40
#define ELARGO 40

struct personaje
{
	int direccion;
    int pos_x, pos_y;
} p;

struct Enemigo
{
    int poss_x, poss_y;
    int direccion;
} e1, e2, e3;


BITMAP *fondo;
BITMAP *megaparado;
BITMAP *bloque;
BITMAP *vacio;
BITMAP *enemigo_img;
BITMAP *megacorriendo2;
BITMAP *megacorriendo;
BITMAP *enemigo_bitmap;
BITMAP *bala;

void dibujo(personaje& p, Enemigo *e1, Enemigo *e2, Enemigo *e3);
void mapa(int level,Enemigo *e1, Enemigo *e2, Enemigo *e3);
void mover_personaje(personaje& p);
void mover_enemigo(Enemigo *e);
void dibujamapa();
void limpiar();
char mapita[MAXANCHO][MAXLARGO];

int main()
{
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, MAXANCHO, MAXLARGO, 0, 0);
    fondo = create_bitmap(MAXANCHO, MAXLARGO);
    clear_to_color(fondo, 0xFFFFFF);
    megaparado = load_bitmap("megaparado.bmp", NULL);
    megacorriendo = load_bitmap("megaparado.bmp", NULL);
    megacorriendo2 = load_bitmap("megaparado.bmp", NULL);
    fondo = load_bitmap("fondo.bmp", NULL);
    bloque = load_bitmap("bloque3.bmp", NULL);
    vacio = load_bitmap("f.bmp", NULL);
    enemigo_bitmap = load_bitmap("enemigo.bmp", NULL);
    bala = load_bitmap("bala.bmp", NULL);

    bool salida = false;

    mapa(1,&e1, &e2, &e3);
    dibujamapa();

    while (!key[KEY_ESC])
    {
    	dibujo(p,&e1,&e2,&e3);
        mover_personaje(p);
        mover_enemigo(&e1);
        mover_enemigo(&e2);
        mover_enemigo(&e3);

    }

    salida = true;

    return 0;
}
END_OF_MAIN()

void dibujo(personaje& p, Enemigo *e1, Enemigo *e2, Enemigo *e3)
{
	masked_blit(fondo, screen, 0, 0, 0, 0, MAXANCHO, MAXLARGO);
	if (p.direccion == 1)
	{
	masked_blit(megacorriendo, screen, 0, 0, p.pos_x, p.pos_y, MAXANCHO, MAXLARGO);
	}
	if (p.direccion == 2)
	{
	masked_blit(megacorriendo2, screen, 0, 0, p.pos_x, p.pos_y, MAXANCHO, MAXLARGO);
	}
	masked_blit(enemigo_bitmap, screen, 0, 0, e1->poss_y, e1->poss_x, MAXANCHO, MAXLARGO);
	masked_blit(enemigo_bitmap, screen, 0, 0, e2->poss_y, e2->poss_x, MAXANCHO, MAXLARGO);
	masked_blit(enemigo_bitmap, screen, 0, 0, e3->poss_y, e3->poss_x, MAXANCHO, MAXLARGO);
	//printf("(%d,%d||%d)",e->poss_x,e->poss_y,e->direccion);

}
void mover_enemigo(Enemigo *e)
{
    int velocidad_enemigo_y = 5; // Ajusta la velocidad como desees
    int next_pos_y;

    if (e->direccion == 0)
    {
        next_pos_y = e->poss_y - velocidad_enemigo_y;


        if (next_pos_y >= 0 && mapita[e->poss_x / ANCHOB][(e->poss_y / ANCHOB)-1]!= 'x')
        {
            e->poss_y = next_pos_y;
        }
        else
        {
            e->direccion = 1;
        }
    }
    else if (e->direccion == 1)
    {
        next_pos_y = e->poss_y + velocidad_enemigo_y;

        if (next_pos_y < MAXALTO &&  mapita[e->poss_x / ANCHOB][(e->poss_y / ANCHOB)+1] != 'x')
        {
            e->poss_y = next_pos_y;
        }
        else
        {
            e->direccion = 0;
        }
    }
}

void mover_personaje(personaje& p)
{
    bool disparo_realizado = false;
    p.pos_x = 40;
    p.pos_y = 620;
    int bala_x = -1;
    int bala_y = -1;
    int velocidad = 5; // Ajusta la velocidad 
    int y_velocidad = 0;
    int salto = 0;
    bool bajar = false;
    bool en_el_aire = true;
    int pos_actual;

    // Calcular la columna y fila actual del personaje
    int columna_actual = p.pos_x / ANCHOB;
    int fila_actual = p.pos_y / ANCHOB;
    int fila_siguiente = (p.pos_y + PLARGO) / ANCHOB;

        
        // Movimiento eje x
        if (key[KEY_D])
        {
            p.pos_x + PANCHO < MAXANCHO;
            if (mapita[p.pos_y / PLARGO][(p.pos_x + velocidad) / PANCHO] != 'x' && mapita[(p.pos_y + PLARGO - 1) / PLARGO][(p.pos_x + PANCHO) / PANCHO] != 'x')
            {
                p.pos_x += velocidad;
                p.direccion = 1;
            }
        }

        if (key[KEY_A])
        {
            p.pos_x >= velocidad;
            if (mapita[p.pos_y / PLARGO][(p.pos_x - velocidad) / PANCHO] != 'x' && mapita[(p.pos_y + PLARGO - 1) / PLARGO][(p.pos_x - velocidad) / PANCHO] != 'x')
            {
                p.pos_x -= velocidad;
                p.direccion = 2;
            }
        }

        // Movimiento eje y
        if (en_el_aire)
        {
            y_velocidad += 1;
            p.pos_y += y_velocidad;

            // Verificar colisión con el suelo
            if (mapita[fila_siguiente][columna_actual] == 'x' || mapita[fila_siguiente][(p.pos_x + PLARGO - 1) / ANCHOB] == 'x')
            {
                en_el_aire = false;
                p.pos_y = fila_actual * ANCHOB;
                y_velocidad = 0;
            }
        }

        if (p.pos_y < fila_actual * ANCHOB)
        {
            en_el_aire = true;
        }

        // Salto
        if (key[KEY_SPACE] && !en_el_aire)
        {
        	{
            	// Verificar que el personaje puede saltar
            	if (mapita[fila_actual - 1][columna_actual] != 'x' && mapita[fila_actual - 1][columna_actual + 1] != 'x')
            	{
            	    en_el_aire = true;
            	    y_velocidad = -20;
               		pos_actual = p.pos_y;
            	}
        	}
        
        	// Movimiento eje y
        	if (en_el_aire)
        	{
            	p.pos_y = p.pos_y + y_velocidad;

            	// Verificar colisiones en el eje y
            	if (p.pos_y < pos_actual - 90 || (mapita[p.pos_y/PANCHO][p.pos_x/PANCHO] == 'x' || (mapita[p.pos_y/PANCHO][(p.pos_x+PANCHO-5)/PANCHO] == 'x')))
            	{		 
            		en_el_aire = false;
            		y_velocidad = +10;
            		bajar = true;
				}
        	}
        	else
        	{
            	// Verificar colisión con el suelo
            	if (mapita[fila_siguiente][columna_actual] != 'x' && mapita[fila_siguiente][(p.pos_x+PLARGO)/PANCHO] != 'x')
            	{
           			en_el_aire = false;
                	bajar = true;
            	}
        	}
        
        	if(bajar || (mapita[(p.pos_y+PLARGO)/PANCHO][p.pos_x/PANCHO] != 'x' && mapita[(p.pos_y+PLARGO)/PANCHO][(p.pos_x+PANCHO-5)/PANCHO] != 'x'))
			{
        		p.pos_y = p.pos_y + y_velocidad;
        		if (mapita[(p.pos_y+PLARGO)/PANCHO][p.pos_x/PANCHO] == 'x' || mapita[(p.pos_y+PLARGO)/PANCHO][(p.pos_x+PANCHO-5)/PANCHO] == 'x')
            	{
                	bajar = false;
            	}
			}
		}
        en_el_aire = true;
        y_velocidad = -15; // Ajusta la altura del salto como desees
        // Disparo
        if (key[KEY_X] && !en_el_aire)
        {
           {
    			bala_x = p.pos_x + 50;
    			bala_y = p.pos_y + 24;
			}
			if (bala_x != -1)
			{	
    			bala_x += 20;
    			// Verificar colision de la bala con los bloques
    			int bala_celda_x = bala_x / ANCHOB;
    			int bala_celda_y = bala_y / ANCHOB;
    			if (bala_celda_x >= 0 && bala_celda_x < MAXMANCHO && bala_celda_y >= 0 && bala_celda_y < MAXALTO)
    			{
        			if (mapita[bala_y / ANCHOB][bala_x / ANCHOB] == 'x')
        			{
            			bala_x = -1;
            			bala_y = -1;
        			}
    			}
    			if (bala_x > 1280)
    			{
        			bala_x = -1;
    			}
			}
        	if (bala_x != -1)
        	{
            	masked_blit(bala, screen, 0, 0, bala_x, bala_y, 20, 10);
        	}

	    }

	}	


void dibujamapa()
{
    int i, j;
    for (i = 0; i < MAXALTO; i++)
    {
        for (j = 0; j < MAXMANCHO; j++)
        {
            if (mapita[i][j] == 'x')
            {
                draw_sprite(fondo, bloque, ANCHOB * j, ANCHOB * i);
            }
            if (mapita[i][j] == 'c')
            {
                draw_sprite(fondo, vacio, ANCHOB * j, ANCHOB * i);
            }
        }
    }
}

void mapa(int level,Enemigo *e1, Enemigo *e2, Enemigo *e3)
{
    FILE *mapFile;
    int i, j, cont = 1;
    char aux;
    switch (level)
    {
        case 1:
            if ((mapFile = fopen("mapa.txt", "r")) == NULL)
            {
				printf("Error al abrir archivo");
	            break;
	        }
	        break;
	}
	
	for (i = 0; i < MAXALTO; i++)
	{
	    for (j = 0; j < MAXMANCHO; j++)
    	{
		    fscanf(mapFile, "%c", &mapita[i][j]);
		    if (mapita[i][j] == 'e')
		    {
		    	if(cont == 1){
		    		e1->direccion;
		    		e1->poss_x =i*EANCHO;
		    		printf("e1 %d\n", i*EANCHO);
		    		e1->poss_y =j*ELARGO;
		    		mapita[i][j]=' ';
					cont++;	
				}
				else if (cont == 2){
					e2->direccion;
		    		e2->poss_x =i*EANCHO;
		    		printf("e2 %d\n", i*EANCHO);
		    		e2->poss_y =j*ELARGO;
		    		mapita[i][j]=' ';
		    		cont++;
				}
				else if (cont == 3){
					e3->direccion;
		    		e3->poss_x =i*EANCHO;
		    		printf("e3 %d\n", i*EANCHO);
		    		e3->poss_y =j*ELARGO;
		    		mapita[i][j]=' ';
		    		cont++;
				}
			}
	    }
	    fscanf(mapFile, "%c", &aux);
	}    
	fclose(mapFile);
}

void limpiar()
{
    destroy_bitmap(fondo);
    destroy_bitmap(megaparado);
}

