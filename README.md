# Práctica 5 - Parte 1: Simulación Física sin Interfaz Gráfica

---

## Objetivo
Simular el movimiento y las colisiones de múltiples partículas dentro de una caja rectangular, aplicando las **leyes de la física** (movimiento y conservación del momento) mediante **Programación Orientada a Objetos (POO)** en **C++ / Qt Creator**.

---

## Descripción general
El programa crea un entorno cerrado (la “caja”) donde varias partículas se mueven libremente, rebotan contra las paredes y pueden interactuar entre sí o con obstáculos.  
Toda la simulación se ejecuta en **tiempo discreto (Δt)** y los resultados se guardan en un archivo de texto (`resultados.txt`) para su posterior graficación.

---

## ⚙️ Características principales
- Simulación en **2D** de varias partículas.  
- Rebotes **perfectamente elásticos** con las paredes.  
- Posibilidad de agregar obstáculos y colisiones inelásticas (opcional).  
- Registro de las posiciones y colisiones en un archivo de salida.  
- Implementación basada en **POO**:
  - `Particula` → maneja posición, velocidad y masa.  
  - `Simulador` → gestiona el sistema, el bucle temporal y las colisiones.

---

## 🧱 Estructura básica del proyecto
```
│  
├── main.cpp  
├── particula.h / particula.cpp  
├── simulador.h / simulador.cpp  
└── resultados.txt
```
