import java.util.Scanner;
import java.util.LinkedList;
import java.util.Queue;

class Estudiante {
    String cedula, apellidos, nombres, carrera;
    float notaFinal;
    int nivel;

    public Estudiante(String cedula, String apellidos, String nombres, float notaFinal, String carrera, int nivel) {
        this.cedula = cedula;
        this.apellidos = apellidos;
        this.nombres = nombres;
        this.notaFinal = notaFinal;
        this.carrera = carrera;
        this.nivel = nivel;
    }
}

class Nodo {
    Estudiante datos;
    Nodo izq, der;

    public Nodo(Estudiante datos) {
        this.datos = datos;
        this.izq = this.der = null;
    }
}

class ArbolEstudiantes {
    private Nodo raiz;

    public ArbolEstudiantes() { this.raiz = null; }

    public void insertarEstudiante(Estudiante est) {
        raiz = insertarRecursivo(raiz, est);
    }

    private Nodo insertarRecursivo(Nodo raiz, Estudiante est) {
        if (raiz == null) return new Nodo(est);
        if (est.cedula.compareTo(raiz.datos.cedula) < 0)
            raiz.izq = insertarRecursivo(raiz.izq, est);
        else if (est.cedula.compareTo(raiz.datos.cedula) > 0)
            raiz.der = insertarRecursivo(raiz.der, est);
        else
            System.out.println("[!] Error: El estudiante con esa cédula ya existe.");
        return raiz;
    }

    public void buscarEstudiante(String cedula) {
        Nodo res = buscarRecursivo(raiz, cedula);
        if (res != null) {
            System.out.println("Encontrado: " + res.datos.apellidos + " " + res.datos.nombres);
            System.out.println("Carrera: " + res.datos.carrera + " | Nivel: " + res.datos.nivel);
            System.out.println("Nota Final: " + res.datos.notaFinal);
        } else {
            System.out.println("[-] Estudiante no encontrado.");
        }
    }

    private Nodo buscarRecursivo(Nodo raiz, String cedula) {
        if (raiz == null || raiz.datos.cedula.equals(cedula)) return raiz;
        if (raiz.datos.cedula.compareTo(cedula) > 0) return buscarRecursivo(raiz.izq, cedula);
        return buscarRecursivo(raiz.der, cedula);
    }

    public void eliminarEstudiante(String cedula) {
        if(buscarRecursivo(raiz, cedula) != null) {
            raiz = eliminarRecursivo(raiz, cedula);
            System.out.println("[+] Eliminado correctamente.");
        } else {
            System.out.println("[-] No existe el estudiante.");
        }
    }

    private Nodo eliminarRecursivo(Nodo raiz, String cedula) {
        if (raiz == null) return raiz;
        if (cedula.compareTo(raiz.datos.cedula) < 0) raiz.izq = eliminarRecursivo(raiz.izq, cedula);
        else if (cedula.compareTo(raiz.datos.cedula) > 0) raiz.der = eliminarRecursivo(raiz.der, cedula);
        else {
            if (raiz.izq == null) return raiz.der;
            else if (raiz.der == null) return raiz.izq;

            raiz.datos = valorMinimo(raiz.der);
            raiz.der = eliminarRecursivo(raiz.der, raiz.datos.cedula);
        }
        return raiz;
    }

    private Estudiante valorMinimo(Nodo raiz) {
        Estudiante minV = raiz.datos;
        while (raiz.izq != null) {
            minV = raiz.izq.datos;
            raiz = raiz.izq;
        }
        return minV;
    }

    public void recorridoInorden() { if(raiz==null) System.out.println("Árbol vacío."); else inorden(raiz); }
    private void inorden(Nodo raiz) {
        if (raiz != null) {
            inorden(raiz.izq);
            System.out.println(raiz.datos.cedula + " - " + raiz.datos.apellidos + " | Nota: " + raiz.datos.notaFinal);
            inorden(raiz.der);
        }
    }

    public void recorridoPreorden() { if(raiz==null) System.out.println("Árbol vacío."); else preorden(raiz); }
    private void preorden(Nodo raiz) {
        if (raiz != null) {
            System.out.println(raiz.datos.cedula + " - " + raiz.datos.apellidos + " | Nota: " + raiz.datos.notaFinal);
            preorden(raiz.izq);
            preorden(raiz.der);
        }
    }

    public void recorridoPostorden() { if(raiz==null) System.out.println("Árbol vacío."); else postorden(raiz); }
    private void postorden(Nodo raiz) {
        if (raiz != null) {
            postorden(raiz.izq);
            postorden(raiz.der);
            System.out.println(raiz.datos.cedula + " - " + raiz.datos.apellidos + " | Nota: " + raiz.datos.notaFinal);
        }
    }

    public void recorridoPorNiveles() {
        if (raiz == null) { System.out.println("Árbol vacío."); return; }
        Queue<Nodo> cola = new LinkedList<>();
        cola.add(raiz);
        while (!cola.isEmpty()) {
            Nodo actual = cola.poll();
            System.out.print(actual.datos.cedula + " (" + actual.datos.apellidos + ") -> ");
            if (actual.izq != null) cola.add(actual.izq);
            if (actual.der != null) cola.add(actual.der);
        }
        System.out.println("FIN");
    }

    public void contarNodos() { System.out.println("Total de estudiantes: " + contar(raiz)); }
    private int contar(Nodo raiz) {
        return (raiz == null) ? 0 : 1 + contar(raiz.izq) + contar(raiz.der);
    }

    public void calcularAltura() { System.out.println("Altura del árbol: " + altura(raiz)); }
    private int altura(Nodo raiz) {
        return (raiz == null) ? 0 : 1 + Math.max(altura(raiz.izq), altura(raiz.der));
    }

    private Nodo maxNodo, minNodo;

    public void buscarNotaMayor() {
        maxNodo = null;
        mayorRecursivo(raiz);
        if (maxNodo != null) System.out.println("Mayor nota: " + maxNodo.datos.notaFinal + " (" + maxNodo.datos.apellidos + ")");
        else System.out.println("Árbol vacío.");
    }
    private void mayorRecursivo(Nodo nodo) {
        if (nodo == null) return;
        if (maxNodo == null || nodo.datos.notaFinal > maxNodo.datos.notaFinal) maxNodo = nodo;
        mayorRecursivo(nodo.izq);
        mayorRecursivo(nodo.der);
    }

    public void buscarNotaMenor() {
        minNodo = null;
        menorRecursivo(raiz);
        if (minNodo != null) System.out.println("Menor nota: " + minNodo.datos.notaFinal + " (" + minNodo.datos.apellidos + ")");
        else System.out.println("Árbol vacío.");
    }
    private void menorRecursivo(Nodo nodo) {
        if (nodo == null) return;
        if (minNodo == null || nodo.datos.notaFinal < minNodo.datos.notaFinal) minNodo = nodo;
        menorRecursivo(nodo.izq);
        menorRecursivo(nodo.der);
    }

    public void mostrarAprobados() {
        if(raiz==null) System.out.println("Árbol vacío.");
        else { System.out.println("--- APROBADOS ---"); estadoRecursivo(raiz, true); }
    }
    public void mostrarReprobados() {
        if(raiz==null) System.out.println("Árbol vacío.");
        else { System.out.println("--- REPROBADOS ---"); estadoRecursivo(raiz, false); }
    }
    private void estadoRecursivo(Nodo nodo, boolean aprobados) {
        if (nodo != null) {
            estadoRecursivo(nodo.izq, aprobados);
            boolean paso = nodo.datos.notaFinal >= 7.0f;
            if (aprobados == paso) {
                System.out.println(nodo.datos.cedula + " - " + nodo.datos.apellidos + " | Nota: " + nodo.datos.notaFinal);
            }
            estadoRecursivo(nodo.der, aprobados);
        }
    }
}

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        ArbolEstudiantes arbol = new ArbolEstudiantes();
        int opcion = 0;

        do {
            System.out.println("\n========================================");
            System.out.println("       SISTEMA ACADÉMICO - UTA");
            System.out.println("========================================");
            System.out.println("1. Insertar estudiante\n2. Buscar estudiante\n3. Eliminar estudiante");
            System.out.println("4. Recorrido Inorden\n5. Recorrido Preorden\n6. Recorrido Postorden\n7. BFS (Por niveles)");
            System.out.println("8. Contar estudiantes\n9. Altura del árbol\n10. Mayor nota\n11. Menor nota");
            System.out.println("12. Estudiantes aprobados\n13. Estudiantes reprobados\n14. Salir");
            System.out.println("========================================");
            System.out.print("Seleccione una opción: ");

            try {
                opcion = Integer.parseInt(scanner.nextLine());
            } catch (NumberFormatException e) {
                System.out.println("\n[!] Error: Debe ingresar un número entero válido.");
                continue; // Salta el resto del bucle y vuelve a mostrar el menú
            }

            switch (opcion) {
                case 1:
                    System.out.println("\n--- 1. INSERTAR ESTUDIANTE ---");
                    System.out.print("Cédula: "); String ced = scanner.nextLine();
                    System.out.print("Apellidos: "); String ape = scanner.nextLine();
                    System.out.print("Nombres: "); String nom = scanner.nextLine();

                    float nota = 0;
                    boolean notaValida = false;
                    while (!notaValida) {
                        System.out.print("Nota Final (ej. 8.5): ");
                        try {
                            nota = Float.parseFloat(scanner.nextLine());
                            notaValida = true;
                        } catch (NumberFormatException e) {
                            System.out.println("[!] Error: Ingrese un formato numérico válido para la nota.");
                        }
                    }

                    System.out.print("Carrera: "); String car = scanner.nextLine();

                    int niv = 0;
                    boolean nivelValido = false;
                    while (!nivelValido) {
                        System.out.print("Nivel (solo número entero, ej. 3): ");
                        try {
                            niv = Integer.parseInt(scanner.nextLine());
                            nivelValido = true;
                        } catch (NumberFormatException e) {
                            System.out.println("[!] Error: El nivel debe ser un número entero. No use letras.");
                        }
                    }

                    arbol.insertarEstudiante(new Estudiante(ced, ape, nom, nota, car, niv));
                    System.out.println("[+] Estudiante registrado con éxito.");
                    break;
                case 2:
                    System.out.println("\n--- 2. BUSCAR ESTUDIANTE ---");
                    System.out.print("Cédula a buscar: ");
                    arbol.buscarEstudiante(scanner.nextLine());
                    break;
                case 3:
                    System.out.println("\n--- 3. ELIMINAR ESTUDIANTE ---");
                    System.out.print("Cédula a eliminar: ");
                    arbol.eliminarEstudiante(scanner.nextLine());
                    break;
                case 4: System.out.println("\n--- 4. RECORRIDO INORDEN ---"); arbol.recorridoInorden(); break;
                case 5: System.out.println("\n--- 5. RECORRIDO PREORDEN ---"); arbol.recorridoPreorden(); break;
                case 6: System.out.println("\n--- 6. RECORRIDO POSTORDEN ---"); arbol.recorridoPostorden(); break;
                case 7: System.out.println("\n--- 7. RECORRIDO POR NIVELES (BFS) ---"); arbol.recorridoPorNiveles(); break;
                case 8: System.out.println("\n--- 8. CONTAR ESTUDIANTES ---"); arbol.contarNodos(); break;
                case 9: System.out.println("\n--- 9. ALTURA DEL ÁRBOL ---"); arbol.calcularAltura(); break;
                case 10: System.out.println("\n--- 10. MAYOR NOTA ---"); arbol.buscarNotaMayor(); break;
                case 11: System.out.println("\n--- 11. MENOR NOTA ---"); arbol.buscarNotaMenor(); break;
                case 12: System.out.println("\n--- 12. ESTUDIANTES APROBADOS ---"); arbol.mostrarAprobados(); break;
                case 13: System.out.println("\n--- 13. ESTUDIANTES REPROBADOS ---"); arbol.mostrarReprobados(); break;
                case 14: System.out.println("\nSaliendo del sistema..."); break;
                default: System.out.println("\n[!] Opción no válida. Intente de nuevo."); break;
            }
        } while (opcion != 14);

        scanner.close();
    }
}