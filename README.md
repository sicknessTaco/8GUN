# 🎮 8GUN - Rhythm Gungeon

**8GUN** es un *rhythm bullet hell* con estilo 8-bit inspirado en *Enter the Gungeon* y *Crypt of the NecroDancer*.  
Combina esquivar balas sincronizadas con la música en un sistema de progresión por zonas y canciones.

---

## 🎯 Objetivo del Juego

- **Sobrevive** a oleadas de balas sincronizadas con el ritmo de la música.
- **Completa canciones** para desbloquear nuevas zonas.
- **Avanza** por 4 zonas con 4 canciones cada una.
- **Derrota** al jefe de cada zona para avanzar a la siguiente.

---

## 🕹️ Controles

| Acción | Tecla |
|--------|-------|
| Mover arriba | **W** o **↑** |
| Mover abajo | **S** o **↓** |
| Mover izquierda | **A** o **←** |
| Mover derecha | **D** o **→** |
| Disparar | **ESPACIO** |
| Seleccionar | **ESPACIO** o **ENTER** |
| Volver | **ESC** |

---

## 🎵 Música y Ritmo

- Las balas se disparan **sincronizadas con el beat** de la canción actual.
- El jugador debe **esquivar al ritmo** para sobrevivir.
- Las canciones duran **60 segundos** cada una.

---

## 🗺️ Sistema de Zonas

| Zona | Enemigos | Descripción |
|------|----------|-------------|
| **Zona 1** | Verdes (normales) | Inicial, 4 canciones fáciles |
| **Zona 2** | Verdes + Amarillos (especiales) | Más patrones de balas |
| **Zona 3** | Amarillos + Azules (explosivos) | Mayor velocidad y caos |
| **Zona 4** | Rojos (jefes) | Desafío final |

---

## 🧩 Progresión

- Para desbloquear **Zona N**, debes completar la **Canción 3** de la **Zona N-1**.
- Para desbloquear el **Jefe de Zona**, debes completar la **Canción 3** de la misma zona.
- Cada canción completada **desbloquea la siguiente**.

---

## 🧰 Tecnología

- **Motor**: C++ con SDL2
- **Gráficos**: 1280x720 (720p) con estilo 8-bit
- **Audio**: SDL_mixer
- **Texto**: SDL_ttf con fuente pixel art
- **Estilo**: 8-bit, pixel perfect

---

## 📁 Estructura de Carpetas

```
assets/
├── fonts/
│   └── pkmn.ttf
├── music/
│   ├── 1/
│   │   ├── 1.mp3
│   │   ├── 2.mp3
│   │   ├── 3.mp3
│   │   └── 4.mp3
│   ├── 2/
│   │   └── ...
│   └── mainmenu/
│       └── 1.mp3
```

---

## 🚀 Cómo Compilar

### Requisitos:
- Visual Studio 2019 o superior
- SDL2, SDL_mixer, SDL_ttf

### Pasos:
1. Clona el repositorio
2. Copia las librerías SDL a `lib/` y `include/`
3. Compila en Visual Studio
4. Copia `SDL2.dll`, `SDL2_mixer.dll`, `SDL2_ttf.dll` a la carpeta de salida
5. Asegúrate de tener `assets/` en la carpeta de ejecución

---

## 🧑‍💻 Desarrollado por

**SicknessTaco**  
Proyecto personal para aprender C++ y desarrollo de juegos

---

## 🎮 Estado Actual

- ✅ Menú de selección de zonas y canciones
- ✅ Sistema de desbloqueo progresivo
- ✅ Jugador con movimiento WASD
- ✅ Disparo con ESPACIO
- ⏳ Enemigos con patrones rítmicos
- ⏳ 4 zonas con 4 canciones cada una
- ⏳ Sonidos de disparo y explosión
- ⏳ Más patrones de balas
- ⏳ Mejores efectos visuales
- ⏳ Sprites funcionales
- ⏳ Musica sin copyright (mi error jaja)

---

## 📝 Notas

- El juego está en desarrollo activo
- Inspirado en *Enter the Gungeon*, *Crypt of the NecroDancer*, *Touhou*
- Hecho con amor por un entusiasta de los juegos retro

---

## 🎉 ¡Gracias por jugar!
