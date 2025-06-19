# 🔥 ESP32-Based Fire Fighter Robot

An autonomous and manually controllable firefighting robot built using an ESP32, flame sensors, water pump, servo, and controlled via the Dabble Bluetooth app.

<p align="center">
  <img src="images/model_ffr.jpeg" alt="Robot Model" width="400"/>
</p>


---

## 🚀 Features

- Detects fire using 3 flame sensors.
- Automatically navigates and sprays water toward flame.
- Controlled manually using Dabble GamePad (Bluetooth).
- Servo-controlled spray angle (automatic + manual).
- Water pump control.
- Motor control via PWM.

---

## 🎮 Controls (Dabble GamePad)

| Button   | Function              |
|----------|-----------------------|
| Up       | Move forward          |
| Down     | Move backward         |
| Left     | Turn left             |
| Right    | Turn right            |
| Circle   | Move servo right      |
| Square   | Move servo left       |

---

## 🔧 Hardware Required

- ESP32 board  
- 2 DC Motors + L298N driver  
- 3 Flame sensors  
- Water pump module  
- Servo motor (SG90/standard)  
- Power supply (battery)  
- Bluetooth control via Dabble app
---

## 🧪 Fire Suppression Enhancement (CO₂ Generation)

To complement the water-based fire suppression system, the robot can optionally integrate a **chemical-based CO₂ generation module**. This involves a controlled reaction between:

- **Acetic Acid (CH₃COOH)** – e.g., vinegar
- **Sodium Bicarbonate (NaHCO₃)** – i.e., baking soda

### ⚗️ Chemical Reaction:
CH₃COOH + NaHCO₃ → CH₃COONa + CO₂ ↑ + H₂O
This reaction rapidly generates **carbon dioxide (CO₂)** gas, which displaces oxygen near the flame, helping suppress fire especially in **electrical or oil-based fires** where water may not be safe.

---

## 🧠 Logic Overview

- If no input is received from the GamePad, flame sensors guide the robot toward fire.
- Water pump activates when any flame is detected.
- Servo rotates automatically toward the flame direction.
- Manual override using Circle/Square for fine servo positioning.

---

## 📷 Gallery

<p align="center">
  <img src="images/model_ffr.jpeg" alt="Robot Model" width="400"/>
</p>

---

## 🛠️ How to Upload Code

1. Connect ESP32 to your computer.
2. Open `FireFighterRobot.ino` in Arduino IDE.
3. Select correct board & port.
4. Install libraries:
   - `DabbleESP32`
   - `ESP32Servo`
5. Upload and enjoy!

---

## 📄 Academic Note

This project was developed as part of my undergraduate coursework.

---

## 🤖 Author

Developed by Dinesh Kanna P(https://github.com/dineshkanna113)

