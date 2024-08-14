//TA - Proyecto final Mauricio López y Josue Flores Grupo 7
#include <iostream>
#include <algorithm>
#include <conio.h> 
#include <windows.h> 
#include <vector>
#include <fstream>
#include <cstdlib> 

using namespace std;

struct Jugador {
    string nombre;
    int puntaje;
};

void guardarJugadores(const vector<Jugador>& jugadores);
void cargarJugadores(vector<Jugador>& jugadores);
void mostrarPuntajes(const vector<Jugador>& jugadores);

void mostrarMenu();
void jugar(vector<Jugador>& jugadores);
void mostrarCreditos();
void mostrarInstrucciones();
void validarOpcion(char& opcion);

bool gameOver;
const int width = 40; 
const int height = 20; 
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirecton dir;

void Setup();
void Draw();
void Input();
void Logic();

int main() {
    vector<Jugador> jugadores;
    cargarJugadores(jugadores);
    char opcion;

    do {
        mostrarMenu();
        cin >> opcion;
        validarOpcion(opcion);
        switch(opcion) {
            case '1':
                jugar(jugadores);
                guardarJugadores(jugadores);
                break;
            case '2':
                mostrarPuntajes(jugadores);
                break;
            case '3':
                cout << "Saliendo del juego...\n";
                break;
            case '4':
                mostrarCreditos();
                break;
            case '5':
                mostrarInstrucciones();
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo.\n";
        }
    } while(opcion != '3');

    return 0;
}

void mostrarMenu() {
    cout << "\n----------------NO APTO PARA EPILEPTICOS----------------\n";
    cout << "\n--- Menu ---\n";
    cout << "1. Jugar\n";
    cout << "2. Puntajes\n";
    cout << "3. Salir\n";
    cout << "4. Creditos\n";
    cout << "5. Instrucciones\n";
    cout << "Seleccione una opcion: ";
}

void jugar(vector<Jugador>& jugadores) {
    cout << "Iniciando el juego de Snake...\n";
    Jugador nuevoJugador;
    cout << "Ingrese su nombre: ";
    cin.ignore();
    getline(cin, nuevoJugador.nombre);

    
    Setup();
    while (dir == STOP) {
        Draw();
        Input();
        Sleep(100); 
    }
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100); 
    }

    nuevoJugador.puntaje = score;
    jugadores.push_back(nuevoJugador);
    cout << "Juego terminado. Puntaje: " << nuevoJugador.puntaje << "\n";
}

void mostrarPuntajes(const vector<Jugador>& jugadores) {
    cout << "\n--- Puntajes ---\n";
    vector<Jugador> sortedJugadores = jugadores;
    sort(sortedJugadores.begin(), sortedJugadores.end(), [](const Jugador& a, const Jugador& b) {
        return a.puntaje > b.puntaje;
    });
    int mostrar = min(3, (int)sortedJugadores.size());
    for (int i = 0; i < mostrar; ++i) {
        cout << "Jugador: " << sortedJugadores[i].nombre << " - Puntaje: " << sortedJugadores[i].puntaje << "\n";
    }
}

void mostrarCreditos() {
    cout << "\n--- Creditos ---\n";
    cout << "Desarrollado por el equipo 7\n";
    cout << "Integrantes: \n";
    cout << "1. Josue Flores\n";
    cout << "2. Mauricio Lopez\n";
}

void mostrarInstrucciones(){
    cout << "\nLa serpiente es un 'O'. Debes moverte para tocar la comida 'F'.\n";
    cout << "Cuando toques una 'F' se genera un 'o' detras de 'O' y ganas puntos.\n";
    cout << "Evita tocar los '#' de los bordes y tu propio cuerpo o el juego termina.\n";
    cout << "Controles:\n";
    cout << "Arriba: W\n";
    cout << "Abajo: S\n";
    cout << "Izquierda: A\n";
    cout << "Derecha: D\n";
}
void validarOpcion(char& opcion) {
    while(opcion < '1' || opcion > '5') {
        cout << "Opcion no valida. Intente de nuevo: ";
        cin >> opcion;
    }
}

void guardarJugadores(const vector<Jugador>& jugadores) {
    ofstream archivo("jugadores.txt");
    if (archivo.is_open()) {
        for (const auto& jugador : jugadores) {
            archivo << jugador.nombre << '\n' << jugador.puntaje << '\n';
        }
        archivo.close();
        cout << "Jugadores guardados correctamente.\n"; 
    } else {
        cout << "Error al abrir el archivo para guardar.\n"; 
    }
}

void cargarJugadores(vector<Jugador>& jugadores) {
    ifstream archivo("jugadores.txt");
    if (archivo.is_open()) {
        Jugador jugador;
        while (getline(archivo, jugador.nombre)) {
            archivo >> jugador.puntaje;
            archivo.ignore(); 
            jugadores.push_back(jugador);
        }
        archivo.close();
        cout << "Jugadores cargados correctamente.\n"; 
    } else {
        cout << "Error al abrir el archivo para cargar.\n"; 
    }
}

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

void Draw() {
    system("cls");
    for (int i = 0; i < width+2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#"; 
            if (i == y && j == x)
                cout << "O"; 
            else if (i == fruitY && j == fruitX)
                cout << "F"; // Fruta
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        print = true;
                    }
                }
                if (!print) cout << " ";
            }

            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width+2; i++)
        cout << "#";
    cout << endl;
    cout << "Score:" << score << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    if (x >= width || x < 0 || y >= height || y < 0) {
        gameOver = true;
    }

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
}
}