#include "pch.h"
#include <iostream>
#include "conio.h"
#include <vector>
#include <ctime>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


using namespace System;
using namespace std;

#define ANCHO 80
#define LARGO 40
#define Derecha 77
#define Izquierda 75
#define FILAS 30
#define COLUMNAS 60

vector<int> balasX;
vector<int> balasY;

void setxy(int x, int y) {
    if (x >= 0 && x < Console::BufferWidth && y >= 0 && y < Console::BufferHeight) {
        Console::SetCursorPosition(x, y);
    }
}

void dibujarPowerUp(int pX, int pY) {
	setxy(pX, pY);
	cout << "P"; // Representa un power-up
}
void borrarPowerUp(int pX, int pY) {
	setxy(pX, pY);
	cout << " "; // Borra el power-up
}
void moverPowerUp(int& pX, int& pY, bool& powerUpActivo) {
    if (!powerUpActivo) return; // Si no está activo, no hacer nada

    borrarPowerUp(pX, pY);
    pY++;
    if (pY >= LARGO - 1) {
        borrarPowerUp(pX, pY);
        powerUpActivo = false; // Lo desactiva si llegó al fondo
    }
    else {
        dibujarPowerUp(pX, pY);
    }
}

void dispararBalas(int x, int y) {
    PlaySound(TEXT("C:\\Users\\fight\\Downloads\\disparo.wav"), NULL, SND_FILENAME | SND_ASYNC);
	balasX.push_back(x);
	balasY.push_back(y - 1);
}
void actualizarBalas() {
	for (int i = 0; i < balasX.size(); i++) {
		setxy(balasX[i], balasY[i]);
		cout << " "; // Borra la bala anterior
		balasY[i]--; 
		setxy(balasX[i], balasY[i]);
		cout << char(254);
	}
	for (int i = 0; i < balasX.size(); i++) {
		if (balasY[i] < 2) {
			setxy(balasX[i], balasY[i]);
			cout << " "; // Borra la bala que sale de la pantalla
			balasX.erase(balasX.begin() + i);
			balasY.erase(balasY.begin() + i);
			i--;

		}
	}
}

void dibujar(int x, int y) {
    setxy(x, y);
    cout << char(197);
    setxy(x - 1, y);
    cout << char(248);
    setxy(x + 1, y);
    cout << char(248);
}

void borrar(int x, int y) {
    setxy(x, y);
    cout << " ";
    setxy(x - 1, y);
    cout << " ";
    setxy(x + 1, y);
    cout << " ";
}

void mover(int& x, int& y, int tecla) {
    switch (tecla) {
    case Derecha:
        if (x < ANCHO - 2) {
            borrar(x, y);
            x++;
            dibujar(x, y);
        }
        break;
    case Izquierda:
        if (x > 0 + 2) {
            borrar(x, y);
            x--;
            dibujar(x, y);
        }
        break;
	case ' ':
		dispararBalas(x, y); // Dispara una bala
    default:
        break;
    }
}

void dibujarLimites() {
    for (int i = 0; i < ANCHO; i++) {
        setxy(i, 0);
        cout << char(205);
        setxy(i, LARGO);
        cout << char(205);
    }
    for (int i = 0; i < LARGO; i++) {
        setxy(0, i);
        cout << char(186);
        setxy(ANCHO, i);
        cout << char(186);
    }
}

void dibujarEsquinas() {
    setxy(0, 0);
    cout << char(201);
    setxy(ANCHO, 0);
    cout << char(187);
    setxy(0, LARGO);
    cout << char(200);
    setxy(ANCHO, LARGO);
    cout << char(188);
}

void dibujarEnemigos(int x, int y) {
    setxy(x, y);
    cout << char(197);
    setxy(x - 1, y - 1);
    cout << char(191);
    setxy(x + 1, y - 1);
    cout << char(218);
}

void borrarEnemigos(int x, int y) {
    setxy(x, y);
    cout << " ";
    setxy(x - 1, y - 1);
    cout << " ";
    setxy(x + 1, y - 1);
    cout << " ";
}

void moverEnemigo(int& x, int& y, int& vidas) {
    borrarEnemigos(x, y);
    y++;
    if (y >= LARGO - 1) {
		vidas--;
        y = 2;
        x = rand() % (ANCHO - 2) + 1;
    }

    dibujarEnemigos(x, y);
	
}
void colisionJugadorEnemigo(int& x, int& y, int& ex, int& ey, int& vidas) {
    // Comparar cada parte del jugador con cada parte del enemigo
    if (
        // Jugador (x, y) colisiona con cualquier parte del enemigo
        (x == ex && y == ey) ||
        (x == ex - 1 && y == ey - 1) ||
        (x == ex + 1 && y == ey - 1) ||

        // Jugador (x - 1, y) colisiona con cualquier parte del enemigo
        (x - 1 == ex && y == ey) ||
        (x - 1 == ex - 1 && y == ey - 1) ||
        (x - 1 == ex + 1 && y == ey - 1) ||

        // Jugador (x + 1, y) colisiona con cualquier parte del enemigo
        (x + 1 == ex && y == ey) ||
        (x + 1 == ex - 1 && y == ey - 1) ||
        (x + 1 == ex + 1 && y == ey - 1)
        ) {
        system("cls");
        setxy(ANCHO / 2, LARGO / 2);
        cout << "COLISION DETECTADA" << endl;
        system("pause");
        exit(0); // o podrías reiniciar el juego, restar una vida, etc.
    }
}
void colisionBalasEnemigos(int& x, int& y, int& ex, int& ey, int& puntos) {
    for (int i = 0; i < balasX.size(); i++) {
        if (
            (balasX[i] == ex && abs(balasY[i] - ey) <= 1) ||
            (balasX[i] == ex - 1 && abs(balasY[i] - (ey - 1)) <= 1) ||
            (balasX[i] == ex + 1 && abs(balasY[i] - (ey - 1)) <= 1)
            ) {
            puntos++;
            borrarEnemigos(ex, ey); // Borra al enemigo actual
			setxy(balasX[i], balasY[i]);
			cout << " "; // Borra la bala
            balasX.erase(balasX.begin() + i); // Elimina la bala
            balasY.erase(balasY.begin() + i);
            i--;

            // Generar nueva posición para el enemigo
            ey = 2; // Reinicia la posición vertical del enemigo
            ex = rand() % (ANCHO - 2) + 1; // Genera una nueva posición horizontal
            dibujarEnemigos(ex, ey); // Redibuja al enemigo en la nueva posición
        }
    }
}

void mostrarPuntos(int puntos) {
	setxy(0, LARGO + 1);
	cout << "Puntos: " << puntos;
}
void mostrarVidas(int vidas) {
	setxy(0, LARGO + 2);
	cout << "Vidas: " << vidas;
}

int main()
{
    Random r;
    Console::CursorVisible = false;
    Console::SetWindowSize(ANCHO + 5, LARGO + 5);
    clock_t tiempoPowerUp = clock(); // Marca el tiempo inicial
    bool powerUpActivo = false;
    int x = ANCHO / 2;
    int y = 38;
    int tecla = 0;
	int ex = r.Next(3, ANCHO - 1); 
    int ey = 2;
    int puntos = 0;
    int vidas = 3;
    int contador = 0;
	int powerUpX = 10; // Posición inicial del power-up
	int powerUpY = 10; // Posición inicial del power-up
    dibujarLimites();
    dibujarEsquinas();
    dibujar(x, y);
	dibujarEnemigos(ex, ey);

    while (tecla != 27) {
		mostrarPuntos(puntos);
		mostrarVidas(vidas);
        if (_kbhit()) {
            tecla = _getch();
            mover(x, y, tecla);
        }

        colisionJugadorEnemigo(x, y, ex, ey, vidas); // Verificar colisión
        colisionBalasEnemigos(x, y, ex, ey, puntos); // Verificar colisión entre balas y enemigos
		if (vidas <= 0) {
			system("cls");
			setxy(ANCHO / 2, LARGO / 2);
			cout << "GAME OVER" << endl;
			system("pause");
			exit(0);
		}

        if (contador % 5 == 0) { // cada cierto tiempo se mueve el enemigo
            moverEnemigo(ex, ey, vidas);
        }
		if (contador % 10 == 0) { // cada cierto tiempo se mueve el power-up
			moverPowerUp(powerUpX, powerUpY, powerUpActivo); // Mueve el power-up
		}
		actualizarBalas(); // Actualizar la posición de las balas
        _sleep(15); // Pequeña pausa para evitar alto consumo de CPU
        contador++;
        if (((clock() - tiempoPowerUp) / CLOCKS_PER_SEC) >= 40 && !powerUpActivo) {
            powerUpX = rand() % (ANCHO - 2) + 1;
            powerUpY = 2;
            dibujarPowerUp(powerUpX, powerUpY);
            powerUpActivo = true;
            tiempoPowerUp = clock(); // Reinicia el temporizador
        }
		

    }


    exit(0);

    cin.get();
    cin.ignore();

    return 0;
}
