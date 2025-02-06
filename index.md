# SPE Karlsruhe - MultiMonG
{:toc}
## Vorwort
Dieses Dokument ist in mehreren Kapiteln gegliedert und soll dir das Gadget (Multi-Monitoring Gadget) näherbringen. Darum wird im ersten Kapitel das Gadget im Allgemeinen vorgestellt. Hierzu gehören die verschiedenen Funktionen und der Mikrocontroller. Im nächsten Kapitel wird auf die Module, den Verdrahtungsplan, und die Bauteile des Gadgets eingegangen. Damit im darauffolgenden Kapitel, mit der Inbetriebnahme des Gadgets angefangen werden kann. Im vorletzten Kapitel wird die Programmierung vorgestellt und schematisch an einem Beispiel erklärt. Daraufhin folgt eine Anleitung, in der beschrieben wird, wie du dein Gadget programmieren kannst. Zum Schluss folgt das letzte Kapitel, indem der Ablauf der Konstruktion und die Planung deines Gadgets vorgestellt wird.

## Einführung
Das Gadget ist eine Station mit insgesamt sieben Funktionen. Diese Funktionen werden mit einem Mikrokontroller und verschiedenen Modulen realisiert. Diese Funktionen sind:
1. Anzeigen der aktuellen Uhrzeit
2. Anzeigen der Temperatur
3. Anzeigen der Luftfeuchte
4. Anzeigen des Timers
5. Anzeigen einer eigenen Nachricht
6. Anzeigen der Luftqualität
7. Zyklisches wechseln (5 Sekunden), zwischen Funktion eins, zwei, drei und sechs
