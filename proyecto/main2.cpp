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
    int pos_x;
    int pos_y;
    int direccion;
    bool en_el_aire;
    int y_velocidad;
} p;

struct Enemigo
{
    int poss_x, poss_y;
    int direccion;
} e1, e2, e3;

BITMAP *fondo;
BITMAP *megacorriendo;
BITMAP *megacorriendo2;
BITMAP *bloque;
BITMAP *vacio;
BITMAP *enemigo_bitmap;

char mapita[MAXANCHO / ANCHOB][MAXLARGO / ELARGO];

void dibujo(personaje& p, Enemigo *e1, Enemigo *e2, Enemigo *e3);
void mover_personaje(personaje &p);
void mover_enemigo(Enemigo *e);
void dibujamapa();
void mapa(int level, Enemigo *e1, Enemigo *e2, Enemigo *e3);

int main()
{
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, MAXANCHO, MAXLARGO, 0, 0);
    fondo = create_bitmap(MAXANCHO, MAXLARGO);
    clear_to_color(fondo, 0xFFFFFF);
    megacorriendo = load_bitmap("megacorriendo.bmp", NULL);
    megacorriendo2 = load_bitmap("megacorriendo2.bmp", NULL);
    fondo = load_bitmap("fondo.bmp", NULL);
    bloque = load_bitmap("bloque3.bmp", NULL);
    vacio = load_bitmap("f.bmp", NULL);
    enemigo_bitmap = load_bitmap("enemigo.bmp", NULL);

    // Inicializamos el personaje
    p.pos_x = 40;
    p.pos_y = 640;
    p.direccion = 1;
    p.en_el_aire = false;
    p.y_velocidad = 0;

    mapa(1, &e1, &e2, &e3);
    dibujamapa();

    while (!key[KEY_ESC])
    {
        dibujo(p, &e1, &e2, &e3);
        mover_personaje(p);
        mover_enemigo(&e1);
        mover_enemigo(&e2);
        mover_enemigo(&e3);
    }

    destroy_bitmap(megacorriendo);
    destroy_bitmap(megacorriendo2);
    destroy_bitmap(fondo);
    destroy_bitmap(bloque);
    destroy_bitmap(vacio);
    destroy_bitmap(enemigo_bitmap);

    return 0;
}
END_OF_MAIN()

void dibujo(personaje &p, Enemigo *e1, Enemigo *e2, Enemigo *e3)
{
    masked_blit(fondo, screen, 0, 0, 0, 0, MAXANCHO, MAXLARGO);
    if (p.direccion == 1)
    {
        masked_blit(megacorriendo, screen, 0, 0, p.pos_x, p.pos_y, ANCHOB, ELARGO);
    }
    else if (p.direccion == 2)
    {
        masked_blit(megacorriendo2, screen, 0, 0, p.pos_x, p.pos_y, ANCHOB, ELARGO);
    }
    masked_blit(enemigo_bitmap, screen, 0, 0, e1->poss_y, e1->poss_x, ANCHOB, ELARGO);
    masked_blit(enemigo_bitmap, screen, 0, 0, e2->poss_y, e2->poss_x, ANCHOB, ELARGO);
    masked_blit(enemigo_bitmap, screen, 0, 0, e3->poss_y, e3->poss_x, ANCHOB, ELARGO);
}

void mover_enemigo(Enemigo *e)
{
    int velocidad_enemigo_y = 1; // Ajusta la velocidad del enemigo
    int next_pos_y;

    if (e->direccion == 0)
    {
        next_pos_y = e->poss_y - velocidad_enemigo_y;

        if (next_pos_y >= 0 && mapita[e->poss_x / ANCHOB][(e->poss_y / ANCHOB)] != 'x' && mapita[(e->poss_x / ANCHOB) + 1][(e->poss_y / ANCHOB)] != ' ')
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

        if (next_pos_y < MAXANCHO && mapita[e->poss_x / ANCHOB][(e->poss_y / ANCHOB) + 1] != 'x' && mapita[(e->poss_x + ANCHOB) / ANCHOB][(e->poss_y + ANCHOB) / ANCHOB] != ' ')
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
    int velocidad = 5; // Ajusta la velocidad
    int fuerza_salto = -20; // Fuerza inicial del salto (ajustable)

    // Calcular la columna y fila actual del personaje
    int columna_actual = p.pos_x / ANCHOB;
    int fila_actual = p.pos_y / ELARGO;
    int fila_siguiente = (p.pos_y + ELARGO) / ELARGO;

    // Movimiento eje x
    if (key[KEY_D] && p.pos_x + ANCHOB < MAXANCHO)
    {
        if (mapita[fila_actual][columna_actual + 1] != 'x' && mapita[fila_siguiente][columna_actual + 1] != 'x')
        {
            p.pos_x += velocidad;
            p.direccion = 1;
        }
    }
    else if (key[KEY_A] && p.pos_x >= velocidad)
    {   
        if (mapita[fila_actual][columna_actual - 1] != 'x' && mapita[fila_siguiente][columna_actual - 1] != 'x')
        {
            p.pos_x -= velocidad;
            p.direccion = 2;
        }
    }

    // Salto
    if (key[KEY_SPACE] && !p.en_el_aire)
    {
        p.en_el_aire = true;
        p.y_velocidad = fuerza_salto; // Aplicamos la fuerza inicial del salto
    }

    // Movimiento vertical (eje y)
    if (p.en_el_aire)
    {
        p.pos_y += p.y_velocidad;
        p.y_velocidad += 1; // Agregamos una gravedad simulada (ajustable)

        // Limitar el salto a dos bloques de altura
        if (p.y_velocidad > 0 && p.pos_y >= 640) // Altura máxima del salto (ajustable)
        {
            p.en_el_aire = false;
            p.y_velocidad = 0;
            p.pos_y = 640;
        }

        // Verificar colisión con el suelo
        if (mapita[fila_siguiente][columna_actual] == 'x' || mapita[fila_siguiente][columna_actual + 1] == 'x')
        {
            p.en_el_aire = false; // El personaje aterrizó en el suelo
            p.y_velocidad = 0;
            p.pos_y = (fila_siguiente * ELARGO) - ELARGO;
        }
    }
    else
    {
        // Verificar colisión con el suelo
        if (mapita[fila_siguiente][columna_actual] != 'x' || mapita[fila_siguiente][columna_actual + 1] != 'x')
        {
            p.en_el_aire = false;
        }
    }
}

void dibujamapa()
{
    int i, j;
    for (i = 0; i < MAXANCHO / ANCHOB; i++)
    {
        for (j = 0; j < MAXLARGO / ELARGO; j++)
        {
            if (mapita[i][j] == 'x')
            {
                draw_sprite(fondo, bloque, ANCHOB * j, ELARGO * i);
            }
            else if (mapita[i][j] == 'c')
            {
                draw_sprite(fondo, vacio, ANCHOB * j, ELARGO * i);
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


	destroy_bitmap(megacorriendo);
	destroy_bitmap(megacorriendo2);

	destroy_bitmap(bloque);
	destroy_bitmap(vacio);
	destroy_bitmap(enemigo_bitmap);

}

