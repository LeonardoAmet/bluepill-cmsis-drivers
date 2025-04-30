# Bluepill CMSIS Drivers

Repositorio base para proyectos bare-metal con STM32F103C8T6 (Bluepill), usando CMSIS.  
Incluye ejemplos funcionales, drivers educativos y estructura modular pensada para aprendizaje y TPs.

---

## ✅ Estado actual

- 🧠 Clock configurado a 72 MHz (HSE + PLL) desde `SystemInit`
- 📱 USART1 funcionando (PA9/PA10) con driver propio y eco a 9600 baudios
- ⏱️ Timer 2 con interrupciones generando 1 Hz (LED en PC13)
- 🧰 Makefiles simples y configurables
- 📁 Estructura clara: `labs/`, `common/`, `dev/`
- 📦 Submódulos: `CMSIS_5` y `cmsis-device-f1` integrados en `dev/`

---

## 🧱 Estructura

```
bluepill-cmsis-drivers/
├── dev/                        ← Submódulos: CMSIS y Device STM32F1
│   ├── CMSIS_5/
│   └── cmsis-device-f1/
├── common/                     ← Código compartido (startup, system, drivers)
│   ├── drivers/
│   ├── startup.s
│   ├── linker.ld
│   └── system_stm32f1xx.c
├── labs/                       ← Ejercicios y pruebas
│   ├── 01_blink_timer_irq/
│   └── 02_uart_echo/
└── README.md
```

---

## 🛠️ Requisitos

- Toolchain: `arm-none-eabi-gcc`, `make`
- Flasheo: `openocd`
- Opcional: `gdb-multiarch` para debug
- Si usás **WSL2**: `usbipd` y hacer el *attach* del ST-Link

---

### 🪟 Si usás WSL2: habilitar ST-Link

Si estás usando **WSL2**, los dispositivos USB no se comparten automáticamente. Para usar el ST-Link, seguí estos pasos en **PowerShell (como administrador):**

1. Listá los dispositivos USB disponibles:

   ```bash
   usbipd list
   ```

2. Identificá el `busid` de tu ST-Link, por ejemplo `2-5`, y hacé:

   ```bash
   usbipd attach -a -b 2-5 -w
   ```

> ✅ Gracias al parámetro `-a`, si desconectás y volvés a conectar el ST-Link, **se volverá a asociar automáticamente** a tu distro WSL. No necesitás repetir el comando.

Luego, dentro de WSL podés flashear normalmente:

```bash
make flash
```

---

## 🚀 Uso básico

### 1. Clonar con submódulos

```bash
git clone --recursive https://github.com/LeonardoAmet/bluepill-cmsis-drivers.git
```

> Si ya lo clonaste:
> ```bash
> git submodule update --init --recursive
> ```

---

### 2. Compilar y flashear un ejemplo

```bash
cd labs/01_blink_timer_irq
make
make flash
```

---

## 💡 Tips

- UART default: PA9 = TX, PA10 = RX (USART1)
- LED usuario: PC13
- Podés extender con FreeRTOS, ADC, SPI, etc.
- Compatible con proyectos TP UADE

---

## 📚 Créditos y fuentes

- [CMSIS_5 (ARM)](https://github.com/ARM-software/CMSIS_5)
- [cmsis_device_f1 (ST)](https://github.com/STMicroelectronics/cmsis_device_f1)

