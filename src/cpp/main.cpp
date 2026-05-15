#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#include <limits> // Necesario para limpiar el buffer correctamente

using namespace std;

struct Estudiante {
    string cedula;
    string apellidos;
    string nombres;
    float notaFinal;
    string carrera;
    int nivel;
};

struct Nodo {
    Estudiante datos;
    Nodo* izq;
    Nodo* der;
};

class ArbolEstudiantes {
private:
    Nodo* raiz;

    Nodo* crearNodo(Estudiante est) {
        Nodo* nuevo = new Nodo();
        nuevo->datos = est;
        nuevo->izq = NULL;
        nuevo->der = NULL;
        return nuevo;
    }

    Nodo* insertar(Nodo* nodo, Estudiante est) {
        if (nodo == NULL) return crearNodo(est);
        if (est.cedula < nodo->datos.cedula)
            nodo->izq = insertar(nodo->izq, est);
        else if (est.cedula > nodo->datos.cedula)
            nodo->der = insertar(nodo->der, est);
        else
            cout << "\n[!] Error: El estudiante con esta cédula ya existe.\n";
        return nodo;
    }

    Nodo* buscar(Nodo* nodo, string cedula) {
        if (nodo == NULL || nodo->datos.cedula == cedula) return nodo;
        if (nodo->datos.cedula < cedula) return buscar(nodo->der, cedula);
        return buscar(nodo->izq, cedula);
    }

    Nodo* encontrarMinimo(Nodo* nodo) {
        Nodo* actual = nodo;
        while (actual && actual->izq != NULL) actual = actual->izq;
        return actual;
    }

    Nodo* eliminar(Nodo* nodo, string cedula) {
        if (nodo == NULL) return nodo;
        if (cedula < nodo->datos.cedula)
            nodo->izq = eliminar(nodo->izq, cedula);
        else if (cedula > nodo->datos.cedula)
            nodo->der = eliminar(nodo->der, cedula);
        else {
            if (nodo->izq == NULL) {
                Nodo* temp = nodo->der;
                delete nodo;
                return temp;
            } else if (nodo->der == NULL) {
                Nodo* temp = nodo->izq;
                delete nodo;
                return temp;
            }
            Nodo* temp = encontrarMinimo(nodo->der);
            nodo->datos = temp->datos;
            nodo->der = eliminar(nodo->der, temp->datos.cedula);
        }
        return nodo;
    }

    void inorden(Nodo* nodo) {
        if (nodo != NULL) {
            inorden(nodo->izq);
            cout << nodo->datos.cedula << " - " << nodo->datos.apellidos << " " << nodo->datos.nombres << " | Nota: " << nodo->datos.notaFinal << "\n";
            inorden(nodo->der);
        }
    }

    void preorden(Nodo* nodo) {
        if (nodo != NULL) {
            cout << nodo->datos.cedula << " - " << nodo->datos.apellidos << " " << nodo->datos.nombres << " | Nota: " << nodo->datos.notaFinal << "\n";
            preorden(nodo->izq);
            preorden(nodo->der);
        }
    }

    void postorden(Nodo* nodo) {
        if (nodo != NULL) {
            postorden(nodo->izq);
            postorden(nodo->der);
            cout << nodo->datos.cedula << " - " << nodo->datos.apellidos << " " << nodo->datos.nombres << " | Nota: " << nodo->datos.notaFinal << "\n";
        }
    }

    int contar(Nodo* nodo) {
        if (nodo == NULL) return 0;
        return 1 + contar(nodo->izq) + contar(nodo->der);
    }

    int altura(Nodo* nodo) {
        if (nodo == NULL) return 0;
        return 1 + max(altura(nodo->izq), altura(nodo->der));
    }

    void notaMayor(Nodo* nodo, Nodo*& maxNodo) {
        if (nodo == NULL) return;
        if (maxNodo == NULL || nodo->datos.notaFinal > maxNodo->datos.notaFinal) maxNodo = nodo;
        notaMayor(nodo->izq, maxNodo);
        notaMayor(nodo->der, maxNodo);
    }

    void notaMenor(Nodo* nodo, Nodo*& minNodo) {
        if (nodo == NULL) return;
        if (minNodo == NULL || nodo->datos.notaFinal < minNodo->datos.notaFinal) minNodo = nodo;
        notaMenor(nodo->izq, minNodo);
        notaMenor(nodo->der, minNodo);
    }

    void mostrarEstado(Nodo* nodo, bool aprobados) {
        if (nodo != NULL) {
            mostrarEstado(nodo->izq, aprobados);
            bool esAprobado = nodo->datos.notaFinal >= 7.0; 
            if ((aprobados && esAprobado) || (!aprobados && !esAprobado)) {
                cout << nodo->datos.cedula << " - " << nodo->datos.apellidos << " " << nodo->datos.nombres << " | Nota: " << nodo->datos.notaFinal << "\n";
            }
            mostrarEstado(nodo->der, aprobados);
        }
    }

public:
    ArbolEstudiantes() { raiz = NULL; }

    void insertarEstudiante(Estudiante est) { 
        raiz = insertar(raiz, est); 
        cout << "[+] Estudiante insertado/procesado correctamente.\n";
    }
    
    void buscarEstudiante(string cedula) {
        Nodo* res = buscar(raiz, cedula);
        if (res) cout << "Encontrado: " << res->datos.apellidos << " " << res->datos.nombres << "\nCarrera: " << res->datos.carrera << "\nNivel: " << res->datos.nivel << "\nNota: " << res->datos.notaFinal << "\n";
        else cout << "[-] Estudiante no encontrado.\n";
    }

    void eliminarEstudiante(string cedula) {
        if (buscar(raiz, cedula)) {
            raiz = eliminar(raiz, cedula);
            cout << "[+] Estudiante eliminado correctamente.\n";
        } else {
            cout << "[-] Estudiante no existe.\n";
        }
    }

    void recorridoInorden() { if(raiz==NULL) cout<<"Árbol vacío.\n"; else inorden(raiz); }
    void recorridoPreorden() { if(raiz==NULL) cout<<"Árbol vacío.\n"; else preorden(raiz); }
    void recorridoPostorden() { if(raiz==NULL) cout<<"Árbol vacío.\n"; else postorden(raiz); }

    void recorridoPorNiveles() {
        if (raiz == NULL) { cout << "Árbol vacío.\n"; return; }
        queue<Nodo*> q;
        q.push(raiz);
        while (!q.empty()) {
            Nodo* actual = q.front();
            cout << actual->datos.cedula << " (" << actual->datos.apellidos << ") -> ";
            q.pop();
            if (actual->izq != NULL) q.push(actual->izq);
            if (actual->der != NULL) q.push(actual->der);
        }
        cout << "FIN\n";
    }

    void contarNodos() { cout << "Total de estudiantes en el sistema: " << contar(raiz) << "\n"; }
    void calcularAltura() { cout << "Altura máxima del árbol: " << altura(raiz) << "\n"; }

    void buscarNotaMayor() {
        Nodo* maxNodo = NULL;
        notaMayor(raiz, maxNodo);
        if (maxNodo) cout << "Mayor nota: " << maxNodo->datos.notaFinal << " (" << maxNodo->datos.apellidos << " " << maxNodo->datos.nombres << ")\n";
        else cout << "El árbol está vacío.\n";
    }

    void buscarNotaMenor() {
        Nodo* minNodo = NULL;
        notaMenor(raiz, minNodo);
        if (minNodo) cout << "Menor nota: " << minNodo->datos.notaFinal << " (" << minNodo->datos.apellidos << " " << minNodo->datos.nombres << ")\n";
        else cout << "El árbol está vacío.\n";
    }

    void mostrarAprobados() { if(raiz==NULL) cout<<"Árbol vacío.\n"; else mostrarEstado(raiz, true); }
    void mostrarReprobados() { if(raiz==NULL) cout<<"Árbol vacío.\n"; else mostrarEstado(raiz, false); }
};

// Función auxiliar para leer enteros de forma segura
int leerEntero(string mensaje) {
    int valor;
    cout << mensaje;
    while (!(cin >> valor)) {
        cout << "[!] Entrada inválida. Por favor, ingrese solo números enteros.\n" << mensaje;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return valor;
}

// Función auxiliar para leer flotantes de forma segura
float leerFlotante(string mensaje) {
    float valor;
    cout << mensaje;
    while (!(cin >> valor)) {
        cout << "[!] Entrada inválida. Por favor, ingrese un número (ej. 8.5).\n" << mensaje;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return valor;
}

int main() {
    ArbolEstudiantes arbol;
    int opcion;

    do {
        cout << "\n========================================\n";
        cout << "       SISTEMA ACADEMICO - UTA\n";
        cout << "========================================\n";
        cout << "1. Insertar estudiante\n";
        cout << "2. Buscar estudiante por cedula\n";
        cout << "3. Eliminar estudiante\n";
        cout << "4. Recorrido Inorden\n";
        cout << "5. Recorrido Preorden\n";
        cout << "6. Recorrido Postorden\n";
        cout << "7. Recorrido por niveles BFS\n";
        cout << "8. Contar estudiantes\n";
        cout << "9. Calcular altura del arbol\n";
        cout << "10. Mostrar estudiante con mayor nota\n";
        cout << "11. Mostrar estudiante con menor nota\n";
        cout << "12. Mostrar estudiantes aprobados\n";
        cout << "13. Mostrar estudiantes reprobados\n";
        cout << "14. Salir\n";
        cout << "========================================\n";
        
        opcion = leerEntero("Seleccione una opcion: ");

        switch(opcion) {
            case 1: {
                cout << "\n--- 1. INSERTAR ESTUDIANTE ---\n";
                Estudiante est;
                cout << "Cedula: "; cin >> est.cedula;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Apellidos: "; getline(cin, est.apellidos);
                cout << "Nombres: "; getline(cin, est.nombres);
                est.notaFinal = leerFlotante("Nota Final (ej. 8.5): ");
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Carrera: "; getline(cin, est.carrera);
                est.nivel = leerEntero("Nivel (solo numero, ej. 3): ");
                arbol.insertarEstudiante(est);
                break;
            }
            case 2: {
                cout << "\n--- 2. BUSCAR ESTUDIANTE ---\n";
                string ced; cout << "Ingrese la cedula a buscar: "; cin >> ced;
                arbol.buscarEstudiante(ced);
                break;
            }
            case 3: {
                cout << "\n--- 3. ELIMINAR ESTUDIANTE ---\n";
                string ced; cout << "Ingrese la cedula a eliminar: "; cin >> ced;
                arbol.eliminarEstudiante(ced);
                break;
            }
            case 4: cout << "\n--- 4. RECORRIDO INORDEN ---\n"; arbol.recorridoInorden(); break;
            case 5: cout << "\n--- 5. RECORRIDO PREORDEN ---\n"; arbol.recorridoPreorden(); break;
            case 6: cout << "\n--- 6. RECORRIDO POSTORDEN ---\n"; arbol.recorridoPostorden(); break;
            case 7: cout << "\n--- 7. RECORRIDO POR NIVELES (BFS) ---\n"; arbol.recorridoPorNiveles(); break;
            case 8: cout << "\n--- 8. CONTAR ESTUDIANTES ---\n"; arbol.contarNodos(); break;
            case 9: cout << "\n--- 9. CALCULAR ALTURA ---\n"; arbol.calcularAltura(); break;
            case 10: cout << "\n--- 10. ESTUDIANTE CON MAYOR NOTA ---\n"; arbol.buscarNotaMayor(); break;
            case 11: cout << "\n--- 11. ESTUDIANTE CON MENOR NOTA ---\n"; arbol.buscarNotaMenor(); break;
            case 12: cout << "\n--- 12. ESTUDIANTES APROBADOS ---\n"; arbol.mostrarAprobados(); break;
            case 13: cout << "\n--- 13. ESTUDIANTES REPROBADOS ---\n"; arbol.mostrarReprobados(); break;
            case 14: cout << "\nSaliendo del sistema...\n"; break;
            default: cout << "\n[!] Opcion no valida. Intente de nuevo.\n"; break;
        }

    } while (opcion != 14);

    return 0;
}