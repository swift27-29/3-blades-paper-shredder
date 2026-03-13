# 3-blades-paper-shredder

This project implemented a paper shredder system utilizing three pairs of blades, with a custom-designed printed circuit board (PCB) for control and operation, demonstrating my ability in Embedded Systems. 

## 🚀 Features
- Three pairs of high-efficiency blades
- PCB-based control and automation
- Safety voltage execution integration
- Compact motor driver circuitry

## ⚡ System Overview
1. **Motor Control:** The PCB drives the shredder motor.
2. **Blade Mechanism:** Three pairs of blades operate in synchronized motion.
3. **Safety Circuit:** Automatically stops on jam detection.

| Component | Description |
|------------|-------------|
| Motor | 12V DC Motor |
| Blades | 3 pairs, stainless steel |
| PCB | Custom designed using EasyEDA & Proteus |

## 💡 PCB Schematic Diagram
![Paper Shredder PCB](images/schematic.bmp)

## 📐 PCB Layout and Prototyping

<table>
<tr>
<td><img src="images/PCB-Layout.png" width="200"/></td>
<td><img src="images/Top View.png" width="200"/></td>
<td><img src="images/Design-Trial.jpg" width="210"/></td>
<td><img src="images/Design-Trial1.jpg" width="210"/></td>
</tr>
</table>

## 💻⚙️ PCB Combinational Circuit Simulation

<table>
  <tr>
    <td><img src="images/PCB-Logisim-Simulation-C.jpg" width="200"/></td>
    <td><img src="images/PCB-Logisim-Simulation-U.jpg" width="200"/></td>
    <td><img src="images/PCB-Logisim-Simulation-S.jpg" width="200"/></td>
    <td><img src="images/Pcb-Trial1-Circuit-Simulation.jpg" width="200"/></td>
  </tr>
</table>

## 🧾 User Manual 
1. ** Plug-in Device: ** Switch on the external power button to supply power to the prototype. 
2. ** System Initialization Check: ** Confirm that the seven-segment display and other system indicators are working properly. The display should show "0" upon successful initialization.
3. ** Select a Cutting Mode: ** Once the system has initialized, select the preferred cutting mode by pressing the
corresponding switch.

![Status](https://img.shields.io/badge/status-complete-brightgreen)
