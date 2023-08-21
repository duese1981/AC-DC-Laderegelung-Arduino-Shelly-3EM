# AC-DC-Laderegelung-Arduino-Shelly-3EM
Laderegelung um überschüssigen PV-Strom (DC-seitig) über ein Ladegerät in einem Akku zu speichern.

Wemos D1 Mini ließt die Leistungsdaten des Smartmeters Shelly 3EM aus und steuert damit über ein digitales Poti ein Ladegerät.

Anwendung:
Wenn DC Laden nicht möglich ist.
Balkonkraftwerk oder Wechselrichter ohne DC-Speichermöglichkeit (Nachrüstung von Stromspeichern).
Flexibilität beim Aufstellungsort des Stromspeichers.

Der Nachteil der zusätzlichen Stromwandlungsverluste sollte bekannt sein. 

Komponenten:
- Shelly 3EM (im Hauptverteiler)
- Arduino D1 Mini pro (mit zusätzlicher Antenne)
- X9C103S digitales Potentimeter
- D1 Mini Relais Modul (optional)
- Ladegerät mit Poti einstellbar 58,4V 20A (für 16s LiFePo4 Akku 5kWh)

![Ladegerät_Poti](https://github.com/duese1981/AC-DC-Laderegelung-Arduino-Shelly-3EM/assets/142802395/d6074ceb-3240-4481-9419-38751acf9994)

![digital_poti_X9C103S-pin](https://github.com/duese1981/AC-DC-Laderegelung-Arduino-Shelly-3EM/assets/142802395/863944bd-d091-4ffd-a50c-a6e99d91d16f)

![IMG_0637](https://github.com/duese1981/AC-DC-Laderegelung-Arduino-Shelly-3EM/assets/142802395/8460f603-dcc0-472c-9378-143bebd9d43a)

![IMG_0636](https://github.com/duese1981/AC-DC-Laderegelung-Arduino-Shelly-3EM/assets/142802395/d9130db4-b106-40ef-81aa-2876fb5cd8e9)
