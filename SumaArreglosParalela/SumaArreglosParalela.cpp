#include <iostream>
#include <omp.h>

#define N 1000
#define CHUNK 10
#define MOSTRAR 10

void imprimeArreglo(const float* d, const char* nombre);

int main() {
    std::cout << "Sumando Arreglos en Paralelo con OpenMP\n\n";

    float A[N], B[N], Rseq[N], Rpar[N];

    // 1) Llenar arreglos (puedes usar aleatorios si deseas)
    for (int i = 0; i < N; i++) {
        A[i] = i * 10.0f;
        B[i] = (i + 3) * 3.7f;
    }

    // 2) SUMA SECUENCIAL
    double t0 = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        Rseq[i] = A[i] + B[i];
    }
    double t1 = omp_get_wtime();

    // 3) SUMA PARALELA (OpenMP)
    double t2 = omp_get_wtime();

#pragma omp parallel
    {
        int tid = omp_get_thread_num();

        // Mensaje de inicio por hilo (evita mezcla con critical)
#pragma omp critical
        std::cout << "El thread " << tid << " esta en marcha\n";

        // For paralelo con schedule static, CHUNK
#pragma omp for schedule(static, CHUNK)
        for (int i = 0; i < N; i++) {
            Rpar[i] = A[i] + B[i];
        }

        // Mensaje de fin por hilo
#pragma omp critical
        std::cout << "El thread " << tid << " ha terminado\n";
    }

    double t3 = omp_get_wtime();

    // 4) Validación: comprobar si Rseq y Rpar son iguales
    bool ok = true;
    for (int i = 0; i < N; i++) {
        if (Rseq[i] != Rpar[i]) {
            ok = false;
            break;
        }
    }

    // 5) Imprimir primeros valores para comprobar
    std::cout << "\n";
    imprimeArreglo(A, "A");
    imprimeArreglo(B, "B");
    imprimeArreglo(Rpar, "R (Paralelo)");

    // 6) Mostrar tiempos
    std::cout << "\nValidacion: " << (ok ? "OK " : "ERROR ") << "\n";
    std::cout << "Tiempo secuencial (s): " << (t1 - t0) << "\n";
    std::cout << "Tiempo paralelo   (s): " << (t3 - t2) << "\n";

    return 0;
}

void imprimeArreglo(const float* d, const char* nombre) {
    std::cout << "Primeros " << MOSTRAR << " valores del arreglo " << nombre << ": ";
    for (int i = 0; i < MOSTRAR; i++) {
        std::cout << d[i] << (i < MOSTRAR - 1 ? " - " : "");
    }
    std::cout << "\n";
}