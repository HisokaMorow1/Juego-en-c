#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <allegro.h>
#include <stdbool.h> 
#include <math.h>
#include <allegro.h>
#include <ctime>

#define GRAVITY 1.2       // Gravedad para la caída
#define JUMP_FORCE 16.0   // Fuerza del salto
#define MAX_JUMP_HEIGHT 10 // Altura máxima del salto
#define GROUND_Y 680      // Posición vertical del suelo
#define MAXANCHO 1280
#define MAXLARGO 720
#define MAXMANCHO 32
#define MAXALTO 18
#define PANCHO 40
#define PLARGO 40
#define ANCHOB 40
#define EANCHO 40
#define ELARGO 40
#define BOSSL 40
#define BOSSA 40
#define MAX_ENEMIGOS_INVOCADOS 2
#define N 5
#define NUMERO_DE_NIVELES 4
#define MISIL_COOLDOWN 10000 
#define NUM_MISILES_TO_SHOOT 3 // Cambiar según tus necesidades

struct personaje
{
    int pos_x = 40;
    int pos_y = 620;
    int direccion = 1;
    int jugador_herido = 0;
    int num_vidas = 3;
    bool invulnerable = false;
    int invulnerable_timer = 0;
    double y_velocity;
    bool is_jumping;
    int num_balas_disparadas;
} p;

struct Enemigo
{
    int poss_x, poss_y;
    int direccion;
} e1, e2, e3, e4, e5;

struct Bala
{
    int x;
    int y;
    int x_velocity;
    bool disparada = false;
    int bala_x = -1; 
    int bala_y = -1;
} b;

struct Misil
{
    int x;
    int y;
    int x_velocity;
    int y_velocity;
    double angle;
    double speed; 
    bool disparado = false;
    clock_t last_misil_time = 0;
} m;


struct Boss {
    int direccion;
    int health = 10000;
    int x;
    int y;
    int x_velocity;
    int y_velocity;
    bool alive = true;
    struct Misil misil;
    clock_t last_misil_time = 0; 
} boss;


struct rank {
    char nick[50];
    int puntos;
	int tiempo; 
};


// Globales //

int misil_en_movimiento = false; 
const int OPTION_START = 1;
const int OPTION_EXIT = 2;
const int OPTION_RANKING = 3;
const int MAX_BALAS = 999; // Define el máximo número de balas permitidas
Bala balas[MAX_BALAS]; // Arreglo para almacenar las balas
clock_t ultimo_disparo = 0;
int game_over = 0;
int boss_hit_count = 0;

// Fin Globales //

BITMAP *fondo;
BITMAP *bala;
BITMAP *megacorriendo;
BITMAP *megacorriendo2;
BITMAP *bloque;
BITMAP *vacio;
BITMAP *enemigo_bitmap;
BITMAP *vida;
BITMAP *bos;
BITMAP *misil;
BITMAP *mega;
BITMAP *megall;
BITMAP *menu;

int main_menu(BITMAP* menu);
void cambiar_nivel(int nuevo_nivel, Enemigo *e1, Enemigo *e2, Enemigo *e3, Enemigo *e4, Enemigo *e5,Boss *boss,personaje &p, Bala &b); 
void dibujo(personaje &p, struct Enemigo &e1, struct Enemigo &e2, struct Enemigo &e3, struct Enemigo &e4, struct Enemigo &e5, int bala_x, int bala_y, struct Boss &boss, int current_level, struct Misil &m);
void mapa(int level, Enemigo *e1, Enemigo *e2, Enemigo *e3, Enemigo *e4, Enemigo *e5,Boss *boss);
void dibujar_vidas();
void mover_personaje(personaje &p, Enemigo *e1, Enemigo *e2, Enemigo *e3,Enemigo *e4,Enemigo *e5, int &bala_x, int &bala_y, struct Misil *m,struct Boss *boss);
void disparar_bala(personaje &p);
void actualizar_balas(struct Bala *b);
void dibujamapa();
void limpiar();
void mover_enemigo(Enemigo *e);
char mapita[MAXANCHO][MAXLARGO];
bool verificar_colision_con_enemigos(Bala *bala);
void muestrarank(rank ranking[], int Nreal);
void ingresar_nombre(char nombre[], int longitud_maxima);
void mover_boss(personaje &p, Boss &boss);

int main()
{
	int num_misiles = 10;
	boss.x_velocity = 2; // Velocidad de movimiento
	boss.y_velocity = 0;
	boss.alive = true; 


    int Nreal = 5;
	struct rank ranking[N];
    int bala_x = -1;
    int bala_y = -1;
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, MAXANCHO, MAXLARGO, 0, 0);
    fondo = create_bitmap(MAXANCHO, MAXLARGO);
    megacorriendo2 = load_bitmap("megacorriendo2.bmp", NULL);
    bala = load_bitmap("bala.bmp", NULL);
    megacorriendo = load_bitmap("megacorriendo.bmp", NULL);
    fondo = load_bitmap("fondo.bmp", NULL);
    bloque = load_bitmap("bloque3.bmp", NULL);
    vacio = load_bitmap("f.bmp", NULL);
    enemigo_bitmap = load_bitmap("enemigo.bmp", NULL);
    vida = load_bitmap("vida.bmp", NULL);
	bos = load_bitmap("boss.bmp",NULL);
	misil = load_bitmap("misil.bmp",NULL);
	mega = load_bitmap("mega.bmp",NULL);
	megall = load_bitmap("megall.bmp",NULL);
	menu = load_bitmap("menu.bmp",NULL);

    // Cargar mapa
    mapa(1, &e1, &e2, &e3, &e4, &e5, &boss);
    dibujamapa();
    int exit_game = 0;
    int current_level = 0;
    int menu_choice = 0;
    int boss_hit_count = 0;
  	
	while (!game_over)
    {
    	if (current_level == 0)
        {
            
            menu_choice = main_menu(menu);
            
            switch (menu_choice)
            {
                case OPTION_START:
                    current_level = 1; // Iniciar el juego
                    break;
                case OPTION_EXIT:
                    exit_game = 1; // Salir del juego
                    break;
                case OPTION_RANKING:
                    muestrarank(ranking, N);
                    break;
            }
        }
        else
        {
        dibujo(p, e1, e2, e3, e4, e5, b.bala_x, b.bala_y, boss, current_level, m);

        if (b.disparada)
        {
            b.x += b.x_velocity;
            if (b.x < 0 || b.x > 1250)
            {
                b.disparada = false;
                b.disparada = false;
            }
        }
        
		if (current_level == 1 && e1.poss_x == -100 && e2.poss_x == -100 && e3.poss_x == -100 && e4.poss_x == -100 && e5.poss_x == -100)
		{
    		cambiar_nivel(2, &e1, &e2, &e3, &e4, &e5,&boss,p, b);
    		current_level = 2;
		}
		else if (current_level == 2 && e1.poss_x == -100 && e2.poss_x == -100 && e3.poss_x == -100 && e4.poss_x == -100 && e5.poss_x == -100)
		{
    		cambiar_nivel(3, &e1, &e2, &e3, &e4, &e5,&boss,p, b);
    		current_level = 3;
		}
		else if (current_level == 3 && e1.poss_x == -100 && e2.poss_x == -100 && e3.poss_x == -100 && e4.poss_x == -100 && e5.poss_x == -100)
		{
    		cambiar_nivel(4, &e1, &e2, &e3, &e4, &e5,&boss,p, b);
    		current_level = 4;
    		
		}
        if (current_level == 4 && e1.poss_x == -100 && e2.poss_x == -100 && e3.poss_x == -100 && e4.poss_x == -100 && e5.poss_x == -100) {
            cambiar_nivel(5, &e1, &e2, &e3, &e4, &e5, &boss, p, b);
            current_level = 5;
            
        }
    	mover_personaje(p, &e1, &e2, &e3, &e4, &e5, bala_x, bala_y, &m,&boss);
		actualizar_balas(&b);
        mover_enemigo(&e1);
        mover_enemigo(&e2);
        mover_enemigo(&e3);
        mover_enemigo(&e4);
        mover_enemigo(&e5);
        mover_boss(p, boss);

  		if (boss.health <= 0 && boss.alive) 
		  	{ 
                boss.health = 0;
                boss.alive = false; // Marcar al jefe como muerto
                game_over = 1; // Cambiar el estado del juego a "terminado"
            }
            
        if (p.num_vidas <= 0)
            game_over = 1;
	 	}
	}

    limpiar();
    return 0;
}
END_OF_MAIN()


void dibujo(personaje &p, struct Enemigo &e1, struct Enemigo &e2, struct Enemigo &e3, struct Enemigo &e4, struct Enemigo &e5, int bala_x, int bala_y, struct Boss &boss, int current_level, struct Misil &m)
{
	
	m.y_velocity = 5; 
    release_screen();
    acquire_screen();
    
    draw_sprite(screen, fondo, 0, 0);
    m.x = boss.x;
	m.y = boss.y;

    
    if (p.direccion == 1)
    {
        masked_blit(megacorriendo, screen, 0, 0, p.pos_x, p.pos_y, MAXANCHO, MAXLARGO);
    }
    if (p.direccion == 2)
    {
        masked_blit(megacorriendo2, screen, 0, 0, p.pos_x, p.pos_y, MAXANCHO, MAXLARGO);
    }
    masked_blit(enemigo_bitmap, screen, 0, 0, e1.poss_y, e1.poss_x, MAXANCHO, MAXLARGO);
    masked_blit(enemigo_bitmap, screen, 0, 0, e2.poss_y, e2.poss_x, MAXANCHO, MAXLARGO);
    masked_blit(enemigo_bitmap, screen, 0, 0, e3.poss_y, e3.poss_x, MAXANCHO, MAXLARGO);
    masked_blit(enemigo_bitmap, screen, 0, 0, e4.poss_y, e4.poss_x, MAXANCHO, MAXLARGO);
    masked_blit(enemigo_bitmap, screen, 0, 0, e5.poss_y, e5.poss_x, MAXANCHO, MAXLARGO);
	if (current_level == 5 && boss.alive) 
	{
	    masked_blit(bos, screen, 0, 0, boss.y, boss.x, BOSSL, BOSSA);
	}

    dibujar_vidas();
    
    rest(10);
}


int main_menu(BITMAP* menu)
{
    bool menu_selected = false;
    int option_selected = OPTION_START; 

    while (!menu_selected)
    {
        if (key[KEY_1]) 
        {
            option_selected = OPTION_START;
            menu_selected = true;
        } 
        else if (key[KEY_2]) 
        {
            option_selected = OPTION_EXIT;
            menu_selected = true;
        } 
        else if (key[KEY_3]) 
        {
            option_selected = OPTION_RANKING;
            menu_selected = true;
        }

       
        blit(menu, screen, 0, 0, 0, 0, MAXANCHO, MAXLARGO);

        rest(100); 
    }

    return option_selected;
}



void dibujar_vidas()
{
    int x = 40; 

    for (int i = 0; i < p.num_vidas; i++)
    {
        draw_sprite(screen, vida, x, 40); 
        x += 40; 
    }
}

void mover_boss(personaje &p, Boss &boss)
{
    int velocidad_boss_x = 20; // Ajusta la velocidad de movimiento del jefe
    int velocidad_enemigo_y = 20; // Ajusta la velocidad del enemigo
    int velocidad_boss_y = 20;
    int next_pos_y;

    if (boss.direccion == 0)
    {
        next_pos_y = boss.y - velocidad_enemigo_y;

        if (next_pos_y >= 0 && mapita[boss.x / ANCHOB][(boss.y / ANCHOB)] != 'x' && mapita[(boss.x / ANCHOB) + 1][(boss.y / ANCHOB)] != ' ')
        {
            boss.y = next_pos_y;
        }
        else
        {
            boss.direccion = 1;
        }
    }
    else if (boss.direccion == 1)
    {
        next_pos_y = boss.y + velocidad_enemigo_y;

        if (next_pos_y < MAXANCHO && mapita[boss.x / ANCHOB][(boss.y / ANCHOB) + 1] != 'x' && mapita[(boss.x + ANCHOB) / ANCHOB][(boss.y + ANCHOB) / ANCHOB] != ' ')
        {
            boss.y = next_pos_y;
        }
        else
        {
            boss.direccion = 0;
        }
    }
}


void mover_enemigo(Enemigo *e)
{
    int velocidad_enemigo_y = 2; // Ajusta la velocidad del enemigo
    int velocidad_boss_y = 2;
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
    /*else if (boss->direccion == 1)
    {
    	next_pos_y = boss->x - velodiad_boss_y;
    	if ()
	}*/
}

void mover_personaje(personaje &p, Enemigo *e1, Enemigo *e2, Enemigo *e3,Enemigo *e4,Enemigo *e5, int &bala_x, int &bala_y, Misil *m,struct Boss *boss)
{
    static bool disparo_realizado = false;
    int velocidad = 5;
    const int gravedad = 1;
	//const int MAX_BALAS = 999; 
	bool todas_las_balas_salen_de_la_pantalla = true;
	bool x_presionada = false;

    // Actualizar posición vertical
    int next_y = p.pos_y + p.y_velocity;
    int next_bottom_y = next_y + PLARGO;
    int row_bottom_y = next_bottom_y / ANCHOB;
    int row_top_y = next_y / ANCHOB;

    // Movimiento eje x
    if (key[KEY_D] && p.pos_x + PANCHO < MAXANCHO)
    {
        if (mapita[p.pos_y / ANCHOB][(p.pos_x + PANCHO + velocidad) / ANCHOB] != 'x' && mapita[(p.pos_y + PLARGO - 1) / ANCHOB][(p.pos_x + PANCHO + velocidad) / ANCHOB] != 'x')
        {
            p.pos_x += velocidad;
            p.direccion = 1;
        }
    }

    if (key[KEY_A] && p.pos_x >= velocidad)
    {
        if (mapita[p.pos_y / PLARGO][(p.pos_x - velocidad) / PANCHO] != 'x' && mapita[(p.pos_y + PLARGO - 1) / PLARGO][(p.pos_x - velocidad) / PANCHO] != 'x')
        {
            p.pos_x -= velocidad;
            p.direccion = 2;
        }
    }

    // Salto
    if (key[KEY_SPACE] && !p.is_jumping && mapita[(p.pos_y + PLARGO) / ANCHOB][(p.pos_x + PANCHO / 2) / ANCHOB] == 'x')
    {
        p.is_jumping = true;
        p.y_velocity = -JUMP_FORCE;
    }

    // Aplicar gravedad
    p.y_velocity += GRAVITY;

    if (p.y_velocity > 0) // Si está cayendo
    {
        // Verificar colisión con el suelo
        if (mapita[row_bottom_y][(p.pos_x + PANCHO / 2) / ANCHOB] == 'x')
        {
            p.is_jumping = false;
            p.y_velocity = 0;
            p.pos_y = (row_bottom_y * ANCHOB) - PLARGO;
        }
        else
        {
            p.pos_y = next_y;
        }
    }
    else if (p.y_velocity < 0) // Si está saltando
    {
        // Verificar colisión con el techo
        if (mapita[row_top_y][(p.pos_x + PANCHO / 2) / ANCHOB] == 'x')
        {
            p.y_velocity = 0;
            p.pos_y = (row_top_y + 1) * ANCHOB;
        }
        else
        {
            p.pos_y = next_y;
        }
    }
    else // Si está en el suelo
    {
        if (mapita[row_bottom_y][(p.pos_x + PANCHO / 2) / ANCHOB] == 'x')
        {
            p.pos_y = (row_bottom_y * ANCHOB) - PLARGO;
        }
    }

	if (key[KEY_X] && !x_presionada) 
	{
    	x_presionada = true;
    	disparar_bala(p);
	}

	if (!key[KEY_X]) 
	{
    	x_presionada = false;
	}

    // Mover la bala si está en vuelo
    if (b.disparada || b.bala_x != -1 || b.bala_y != -1)
    {
    	b.bala_x += b.x_velocity;  
    }	
    
   // Verificar colisión con los enemigos
    if (!p.invulnerable)
    {
        if ((p.pos_x + PANCHO) >= e1->poss_y && p.pos_x <= (e1->poss_y + EANCHO) && (p.pos_y + PLARGO) >= e1->poss_x && p.pos_y <= (e1->poss_x + ELARGO))
        {
            p.jugador_herido = 1;
            p.invulnerable = true;
            p.invulnerable_timer = 0;
        }

        if ((p.pos_x + PANCHO) >= e2->poss_y && p.pos_x <= (e2->poss_y + EANCHO) && (p.pos_y + PLARGO) >= e2->poss_x && p.pos_y <= (e2->poss_x + ELARGO))
        {
            p.jugador_herido = 1;
            p.invulnerable = true;
            p.invulnerable_timer = 0;
        }

        if ((p.pos_x + PANCHO) >= e3->poss_y && p.pos_x <= (e3->poss_y + EANCHO) && (p.pos_y + PLARGO) >= e3->poss_x && p.pos_y <= (e3->poss_x + ELARGO))
        {
            p.jugador_herido = 1;
            p.invulnerable = true;
            p.invulnerable_timer = 0;
        }
        if ((p.pos_x + PANCHO) >= e4->poss_y && p.pos_x <= (e4->poss_y + EANCHO) && (p.pos_y + PLARGO) >= e4->poss_x && p.pos_y <= (e4->poss_x + ELARGO))
        {
            p.jugador_herido = 1;
            p.invulnerable = true;
            p.invulnerable_timer = 0;
        }
        if ((p.pos_x + PANCHO) >= e5->poss_y && p.pos_x <= (e5->poss_y + EANCHO) && (p.pos_y + PLARGO) >= e5->poss_x && p.pos_y <= (e5->poss_x + ELARGO))
        {
            p.jugador_herido = 1;
            p.invulnerable = true;
            p.invulnerable_timer = 0;
        }
        if ((p.pos_x + BOSSL) >= boss->y && p.pos_x <= (boss->y + BOSSL) && (p.pos_y + BOSSL) >= boss->x && p.pos_y <= (boss->x + BOSSL))
        {
        	p.jugador_herido = 1;
            p.invulnerable = true;
            p.invulnerable_timer = 0;
        }
        if (m->y >= p.pos_x && m->y <= p.pos_x + PANCHO && m->x >= p.pos_y && m->x <= p.pos_y + PLARGO)
    	{
        	if (p.invulnerable)
        	{
            	p.jugador_herido = 1;
            	p.invulnerable = true;
            	p.invulnerable_timer = 0;
        	}
        }
		
    }

    // Actualizar el temporizador de invulnerabilidad
    if (p.invulnerable)
    {
        p.invulnerable_timer++;
        if (p.invulnerable_timer >= 180)
        {
            p.invulnerable = false;
            p.invulnerable_timer = 0;
        }
    }

    // Actualizar el estado de jugador_herido
    if (p.jugador_herido)
    {
        p.num_vidas--;
        p.jugador_herido = 0; // Reiniciar la variable jugador_herido después de perder una vida
    }

    // Actualizar la invulnerabilidad del jugador
    if (p.invulnerable)
    {
        if (p.invulnerable_timer % 10 >= 5)
        {
            draw_sprite(screen, p.direccion == 1 ? megacorriendo : megacorriendo2, p.pos_x, p.pos_y);
        }
    }
    dibujar_vidas();
}

void actualizar_balas(struct Bala *b) 
{
	bool disparo_realizado = false;
    for (int i = 0; i < MAX_BALAS; ++i) 
	{
        if (balas[i].disparada) 
		{
            balas[i].x += balas[i].x_velocity;

            // Verificar colisión con los enemigos
            if (verificar_colision_con_enemigos(&balas[i])) 
			{
                balas[i].disparada = false;
            }
            // Verificar colisión de la bala con los bloques
	        if (balas[i].bala_x / ANCHOB >= 0 && balas[i].bala_x / ANCHOB < MAXMANCHO && balas[i].bala_y / ANCHOB >= 0 && balas[i].bala_y / ANCHOB < MAXALTO)
	        {
	            if (mapita[balas[i].y / ANCHOB][balas[i].x / ANCHOB] == 'x')
	            {
	                balas[i].disparada = false;
	                disparo_realizado = false;
	                balas[i].bala_x = -1;
	                balas[i].bala_y = -1;
	            }
	        }
            // Verificar si la bala salió de la pantalla
            if (balas[i].x < 0 || balas[i].x > MAXANCHO) 
			{
                balas[i].disparada = false; // Desactiva la bala si salió de la pantalla
            } 
			else if(balas[i].bala_x != -1 && balas[i].bala_y != -1)
			{
				masked_blit(bala, screen, 0, 0, balas[i].x, balas[i].y, 20, 10);
                //draw_sprite(screen, bala, balas[i].x, balas[i].y);
            }
        }
    }
}

bool verificar_colision_con_enemigos(Bala *bala) 
{
    if (bala->x >= e1.poss_y && bala->x <= e1.poss_y + EANCHO &&
        bala->y >= e1.poss_x && bala->y <= e1.poss_x + ELARGO) {
        e1.poss_x = -100;
        e1.poss_y = -100;
        return true;
    }
    if (bala->x >= e2.poss_y && bala->x <= e2.poss_y + EANCHO &&
        bala->y >= e2.poss_x && bala->y <= e2.poss_x + ELARGO) {
        e2.poss_x = -100;
        e2.poss_y = -100;
        return true;
    }
    if (bala->x >= e3.poss_y && bala->x <= e3.poss_y + EANCHO &&
        bala->y >= e3.poss_x && bala->y <= e3.poss_x + ELARGO) {
        e3.poss_x = -100;
        e3.poss_y = -100;
        return true;
    }
    if (bala->x >= e4.poss_y && bala->x <= e4.poss_y + EANCHO &&
        bala->y >= e4.poss_x && bala->y <= e4.poss_x + ELARGO) {
        e4.poss_x = -100;
        e4.poss_y = -100;
        return true;
    }
    if (bala->x >= e5.poss_y && bala->x <= e5.poss_y + EANCHO &&
        bala->y >= e5.poss_x && bala->y <= e5.poss_x + ELARGO) {
        e5.poss_x = -100;
        e5.poss_y = -100;
        return true;
    }
    if (bala->x >= boss.y && bala->x <= boss.y + BOSSL &&
        bala->y >= boss.x && bala->y <= boss.x + BOSSL) {
        boss.health -= 5;
        return true;
    }
    return false;
}

void disparar_bala(personaje &p) 
{
    // Obtener el tiempo actual en ciclos de reloj
    clock_t tiempo_actual = clock();

    // Calcular la diferencia de tiempo desde el último disparo en segundos
    double tiempo_transcurrido = static_cast<double>(tiempo_actual - ultimo_disparo) / CLOCKS_PER_SEC;

    // Si ha pasado al menos 1 segundo desde el último disparo
    if (tiempo_transcurrido >= 0.1) 
    {
        for (int i = 0; i < MAX_BALAS; ++i) 
        {
            if (!balas[i].disparada) 
            { 
                balas[i].x = p.direccion == 1 ? p.pos_x + PANCHO : p.pos_x;
                balas[i].y = p.pos_y + PLARGO / 2;
                balas[i].x_velocity = p.direccion == 1 ? 5 : -5;
                balas[i].bala_x = 1;
                balas[i].bala_y = 1;
                balas[i].disparada = true;
                ultimo_disparo = tiempo_actual; // Actualizar el tiempo del último disparo
                break; 
            }
        }
    }
}

void dibujamapa()
{
    int i, j;
    clear_to_color(fondo, 0xFFFFFF); // Limpiar el fondo antes de dibujar el mapa
    for (i = 0; i < MAXALTO; i++)
    {
        for (j = 0; j < MAXMANCHO; j++)
        {
            if (mapita[i][j] == 'x')
            {
                draw_sprite(fondo, bloque, ANCHOB * j, ANCHOB * i);
            }
            else if (mapita[i][j] == 'c')
            {
                draw_sprite(fondo, vacio, ANCHOB * j, ANCHOB * i);
            }   
        }
    }
}


void mapa(int level, Enemigo *e1, Enemigo *e2, Enemigo *e3, Enemigo *e4, Enemigo *e5,Boss *boss) 
{
    FILE *mapFile;
    int i, j, cont = 1;
    char aux;
    char filename[20];
    switch (level) {
        case 1:
            strcpy(filename, "mapa.txt");
            break;
        case 2:
            strcpy(filename, "mapa2.txt");
            break;
        case 3:
            strcpy(filename, "mapa3.txt");
            break;
        case 4:
            strcpy(filename, "mapa4.txt");
            break;
        case 5:
            strcpy(filename, "mapa5.txt");
            break;
        // Agregar más casos para cada nivel adicional si es necesario
        default:
            printf("Nivel no encontrado");
            return;
    }

    if ((mapFile = fopen(filename, "r")) == NULL) {
        printf("Error al abrir archivo");
        return;
    }

    for (i = 0; i < MAXALTO; i++) {
        for (j = 0; j < MAXMANCHO; j++) {
            fscanf(mapFile, "%c", &mapita[i][j]);
            if (mapita[i][j] == 'e') {
                if (cont == 1) {
                    e1->direccion = 1;
                    e1->poss_x = i * EANCHO;
                    e1->poss_y = j * ELARGO;
                    mapita[i][j] = ' ';
                    cont++;
                } else if (cont == 2) {
                    e2->direccion = 1;
                    e2->poss_x = i * EANCHO;
                    e2->poss_y = j * ELARGO;
                    mapita[i][j] = ' ';
                    cont++;
                } else if (cont == 3) {
                    e3->direccion = 1;
                    e3->poss_x = i * EANCHO;
                    e3->poss_y = j * ELARGO;
                    mapita[i][j] = ' ';
                    cont++;
                } else if (cont == 4) {
                    e4->direccion = 1;
                    e4->poss_x = i * EANCHO;
                    e4->poss_y = j * ELARGO;
                    mapita[i][j] = ' ';
                    cont++;
                } else if (cont == 5) {
                    e5->direccion = 1;
                    e5->poss_x = i * EANCHO;
                    e5->poss_y = j * ELARGO;
                    mapita[i][j] = ' ';
                    cont++;
                }
            }
            if (mapita[i][j] == 'b')
			{
				boss->x = i * BOSSL;
				boss->y = j * BOSSA;
				mapita[i][j] = ' ';	
			}
        }
        fscanf(mapFile, "%c", &aux);
    }
    fclose(mapFile);
}

void ingresar_nombre(char nombre[], int longitud_maxima) {
    int key;
    int indice = 0;
    bool terminado = false;

    while (!terminado) {
        key = readkey() >> 8;
        
        if (key == KEY_ENTER) {
            terminado = true;
        } else if (key == KEY_BACKSPACE) {
            if (indice > 0) {
                nombre[--indice] = '\0';
            }
        } else if (key >= ' ' && key <= '~' && indice < longitud_maxima - 1) {
            nombre[indice++] = key;
            nombre[indice] = '\0';
        }
//		textout(fondo, font, const char *s, int x, y, 0);

    }
}


void cambiar_nivel(int nuevo_nivel, Enemigo *e1, Enemigo *e2, Enemigo *e3, Enemigo *e4, Enemigo *e5,Boss *boss,personaje &p, Bala &b) 
{
    char filename[20];
    sprintf(filename, "mapa%d.txt", nuevo_nivel);
    mapa(nuevo_nivel, e1, e2, e3, e4, e5,boss);
    for (int i = 0; i < MAX_BALAS; i++){
		balas[i].disparada = false;
	    balas[i].bala_x = -1;
	    balas[i].bala_y = -1;
	}
    dibujamapa();
    bool verificar_colision_con_enemigos(Bala *bala);
	actualizar_balas(&b);

    // Restablecer posición del personaje
    p.pos_x = 40;
    p.pos_y = 620;
    p.num_vidas = 3;
    p.jugador_herido = 0;
    p.invulnerable = false;
    p.invulnerable_timer = 0;

    // Restablecer posición de los enemigos
    FILE *mapFile;
    if ((mapFile = fopen(filename, "r")) == NULL) 
	{
        printf("Error al abrir archivo");
        return;
    }

    int i, j, cont = 1; 
    char aux;
    for (i = 0; i < MAXALTO; i++) {
        for (j = 0; j < MAXMANCHO; j++) {
            fscanf(mapFile, "%c", &mapita[i][j]);
            if (mapita[i][j] == 'e') {
                if (cont == 1) {
                    e1->direccion = 1;
                    e1->poss_x = i * EANCHO;
                    e1->poss_y = j * ELARGO;
                    mapita[i][j] = ' ';
                    cont++;
                } else if (cont == 2) {
                    e2->direccion = 1;
                    e2->poss_x = i * EANCHO;
                    e2->poss_y = j * ELARGO;
                    mapita[i][j] = ' ';
                    cont++;
                } else if (cont == 3) {
                    e3->direccion = 1;
                    e3->poss_x = i * EANCHO;
                    e3->poss_y = j * ELARGO;
                    mapita[i][j] = ' ';
                    cont++;
                } else if (cont == 4) { 
                    e4->direccion = 1;
                    e4->poss_x = i * EANCHO;
                    e4->poss_y = j * ELARGO;
                    mapita[i][j] = ' ';
                    cont++;
                } else if (cont == 5) { 
                    e5->direccion = 1;
                    e5->poss_x = i * EANCHO;
                    e5->poss_y = j * ELARGO;
                    mapita[i][j] = ' ';
                    cont++;
                }
            }
            if (mapita[i][j] == 'b')
			{
				boss->x = i * BOSSL;
				boss->y = j * BOSSA;
				mapita[i][j] == ' ';	
			}
        }
        fscanf(mapFile, "%c", &aux);
    }	

    fclose(mapFile);
    boss_hit_count = 0;
}
void leeranking(struct rank ranking[], int &Nreal) //lee ranking
{
    FILE *ranki;
    Nreal = 0;

    if ((ranki = fopen("rank.txt", "rt")) == NULL)
    {
        printf("Error al abrir el archivo");
        return;
    }

    while (!feof(ranki))
    {
        fscanf(ranki, "%s %d", ranking[Nreal].nick, &ranking[Nreal].puntos);
        Nreal++;
    }
    fclose(ranki);
}

void muestrarank(struct rank ranking[], int Nreal) //muestra ranking
{
    for (int i = 0; i < Nreal; i++)
    {
        int horas = ranking[i].tiempo / 3600 ;
        int minutos = (ranking[i].tiempo % 3600) / 60;
        int segundos = ranking[i].tiempo % 60;
        printf("Jugador: %s - Tiempo: %02d:%02d:%02d\n", ranking[i].nick, horas, minutos, segundos);
    }
}

void grabaranking(struct rank ranking[], int Nreal) //guarda ranking
{
    FILE *ranki;
    if ((ranki = fopen("rank.txt", "wt")) == NULL)
    {
        printf("Error al abrir el archivo");
        return;
    }
    for (int i = 0; i < Nreal; i++)
    {
        if (i < Nreal - 1)
        {
            fprintf(ranki, "%s %d\n", ranking[i].nick, ranking[i].puntos);
        }
        else
        {
            fprintf(ranki, "%s %d", ranking[i].nick, ranking[i].puntos);
        }
    }
    fclose(ranki);
}


void limpiar()
{
    destroy_bitmap(bala);
    destroy_bitmap(megacorriendo);
    destroy_bitmap(megacorriendo2);
    destroy_bitmap(bloque);
    destroy_bitmap(vacio);
    destroy_bitmap(enemigo_bitmap);
    destroy_bitmap(vida);
}

