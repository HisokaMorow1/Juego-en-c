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
    int pos_x = 300;
    int pos_y = 540;
    int direccion = 1;
    int bala_x = -1; 
    int bala_y = -1;
    int jugador_herido = 0;
    int num_vidas = 3;
    bool invulnerable = false;
    int invulnerable_timer = 0;

    // Variables para el salto
    bool en_el_suelo = true;
    bool saltando = false;
    int y_velocidad = 0;
} p;

struct Enemigo
{
    int poss_x, poss_y;
    int direccion;
} e1, e2, e3;

BITMAP *fondo;
BITMAP *bala;
BITMAP *megacorriendo;
BITMAP *megacorriendo2;
BITMAP *bloque;
BITMAP *vacio;
BITMAP *enemigo_bitmap;
BITMAP *vida;

void dibujo();
void mapa(int level);
void mover_personaje();
void dibujamapa();
void limpiar();
void mover_enemigo(Enemigo *e);
char mapita[MAXANCHO][MAXLARGO];
void dibujar_vidas();

int main()
{
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, MAXANCHO, MAXLARGO, 0, 0);
    fondo = create_bitmap(MAXANCHO, MAXLARGO);
    clear_to_color(fondo, 0xFFFFFF);
    megacorriendo2 = load_bitmap("megacorriendo2.bmp",NULL);
    bala = load_bitmap("bala.bmp", NULL);
    megacorriendo = load_bitmap("megacorriendo.bmp", NULL);
    fondo = load_bitmap("fondo.bmp", NULL);
    bloque = load_bitmap("bloque3.bmp", NULL);
    vacio = load_bitmap("f.bmp", NULL);
    enemigo_bitmap = load_bitmap("enemigo.bmp", NULL);
    vida = load_bitmap("vida.bmp", NULL);

    mapa(1);
    dibujamapa();
    
    while (!key[KEY_ESC])
    {
        dibujo();
        mover_personaje();
        mover_enemigo(&e1);
        mover_enemigo(&e2);
        mover_enemigo(&e3);
    }

    limpiar();
    return 0;
}
END_OF_MAIN()

void dibujo()
{
    release_screen();
    acquire_screen();
    draw_sprite(screen, fondo, 0, 0);
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
    if (p.bala_x != -1 && p.bala_y != -1)
    {
        masked_blit(bala, screen, 0, 0, p.bala_x, p.bala_y, 20, 10);
    }
    dibujar_vidas();
    rest(10);
}

void dibujar_vidas()
{
    int x = 40; // Coordenada x inicial para dibujar los corazones de vida

    for (int i = 0; i < p.num_vidas; i++)
    {
        draw_sprite(screen, vida, x, 40); // Dibuja un corazón en la posición (x, 10)
        x += 10; // Aumenta la coordenada x para dibujar el siguiente corazón
    }
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

        if (next_pos_y < MAXANCHO && mapita[e->poss_x / ANCHOB][(e->poss_y / ANCHOB) + 1] != 'x' && mapita[(e->poss_x + ANCHOB)/ ANCHOB][(e->poss_y + ANCHOB) / ANCHOB] != ' ')
        {
            e->poss_y = next_pos_y;
        }
        else
        {
            e->direccion = 0;
        }
    }
}

void mover_personaje()
{
    static bool disparo_realizado = false;
    int velocidad = 5;
    const int gravedad = 1;

    // Calcular la columna y fila actual del personaje
    int columna_actual = p.pos_x / ANCHOB;
    int fila_actual = p.pos_y / ANCHOB;
    int fila_siguiente = (p.pos_y + PLARGO) / ANCHOB;

    // Verificar si el personaje está en el suelo
    if (mapita[fila_siguiente][columna_actual] == 'x' || mapita[fila_siguiente][(p.pos_x + PLARGO) / ANCHOB] == 'x')
    {
        p.en_el_suelo = true;
        p.saltando = false;
        p.y_velocidad = 0;
    }
    else
    {
        p.en_el_suelo = false;
    }

    // Salto del personaje
    if (key[KEY_SPACE] && p.en_el_suelo)
    {
        p.saltando = true;
        p.y_velocidad = -15;
    }

    // Aplicar gravedad si no está en el suelo
    if (!p.en_el_suelo)
    {
        p.y_velocidad += gravedad;
        p.pos_y += p.y_velocidad;
    }

    // Movimiento lateral del personaje
    if (key[KEY_RIGHT])
    {
        p.direccion = 1;
        int next_pos_x = p.pos_x + velocidad;

        if (next_pos_x + PANCHO < MAXLARGO && mapita[p.pos_y / ANCHOB][(next_pos_x + PANCHO) / ANCHOB] != 'x')
        {
            p.pos_x = next_pos_x;
        }
    }

    if (key[KEY_LEFT])
    {
        p.direccion = 2;
        int next_pos_x = p.pos_x - velocidad;

        if (next_pos_x >= 0 && mapita[p.pos_y / ANCHOB][(next_pos_x) / ANCHOB] != 'x')
        {
            p.pos_x = next_pos_x;
        }
    }

    // Disparo del personaje
    if (key[KEY_LCONTROL] && !disparo_realizado)
    {
        p.bala_x = p.pos_x + PANCHO;
        p.bala_y = p.pos_y + PLARGO / 2;
        disparo_realizado = true;
    }
    else if (!key[KEY_LCONTROL])
    {
        disparo_realizado = false;
    }
}

void mapa(int level)
{
    FILE *file;
    char filename[100];
    sprintf(filename, "mapa%d.txt", level);
    file = fopen(filename, "r");
    int i = 0, j = 0;
    while (!feof(file))
    {
        char c = fgetc(file);
        if (c != '\n')
        {
            mapita[i][j] = c;
            j++;
        }
        else
        {
            i++;
            j = 0;
        }
    }
    fclose(file);
}

void dibujamapa()
{
    for (int i = 0; i < MAXALTO; i++)
    {
        for (int j = 0; j < MAXMANCHO; j++)
        {
            if (mapita[i][j] == 'x')
            {
                masked_blit(bloque, fondo, 0, 0, j * ANCHOB, i * ANCHOB, ANCHOB, ANCHOB);
            }
            if (mapita[i][j] == ' ')
            {
                masked_blit(vacio, fondo, 0, 0, j * ANCHOB, i * ANCHOB, ANCHOB, ANCHOB);
            }
            if (mapita[i][j] == 'E')
            {
                if (e1.poss_x == 0)
                {
                    e1.poss_x = i * EANCHO;
                    e1.poss_y = j * ELARGO;
                }
                else if (e2.poss_x == 0)
                {
                    e2.poss_x = i * EANCHO;
                    e2.poss_y = j * ELARGO;
                }
                else if (e3.poss_x == 0)
                {
                    e3.poss_x = i * EANCHO;
                    e3.poss_y = j * ELARGO;
                }
            }
        }
    }
}

void limpiar()
{
    destroy_bitmap(fondo);
    destroy_bitmap(megacorriendo);
    destroy_bitmap(bala);
    destroy_bitmap(megacorriendo2);
    destroy_bitmap(bloque);
    destroy_bitmap(vacio);
    destroy_bitmap(enemigo_bitmap);
    destroy_bitmap(vida);
}

