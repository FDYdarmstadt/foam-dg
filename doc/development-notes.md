# Entwicklungs-Notizen zur OpenFOAM-BoSSS Brücke

**Erster Milestone: Laplace-Gleichung in BoSSS lösen.**



***
Datum: 06. April 2021

## Bestandsaufnahme und Erinnerung 

- aktuelle `foam-extend-4.1` version kompiliert;
  - siehe https://openfoamwiki.net/index.php/Installation/Linux/foam-extend-4.1
  - `Allwmake`, resp. `Allwmake.firstinstall`
- `foam-dg`kompiliert gegen `foam-extend-4.1`
  - sourcing `foam-extend-4.1`; alias `fe41` in `.bashrc`
  - make sure that the BoSSS libraries can be found by OpenFOAM:
    - make sure that the environment variable `$BOSSS_INSTALL` is set to the root of the BoSSS code repository
    - make sure that the directory `$BOSSS_INSTALL/public/src/L4-application/ExternalBinding.CodeGen/bin/Release/net5.0/` exists and contains the relevant .dll-files.
  - `Allwmake`
- wo liegen main-files und executables (Laplace-Gleichung)?
  - Source: `./foam-dg/src/dgLaplacianFoam/dgLaplacianFoam.C`
  - Binary: `~/foam/flori-4.1/applications/bin/linux64GccDPInt32Opt/dgLaplacianFoam`
  - Execute in `~/foam/foam-dg/run/julyReport/threeModes/laplace-linear`
    1. call `blockmeshdict`
    2. call `dgLaplacianFoam`


## Konfiguration für VScode-Entwicklung
- Code-Navigation scheint Out-Of-Box zu funktionieren
- Anleitung: https://openfoamwiki.net/index.php/HowTo_Use_OpenFOAM_with_Visual_Studio_Code
- Schwierige Einstellungen (verschoben):
  - Build und Run? wie wird das "sourcing" konfiguriert?
  - Benutzung des Debuggers?

## Konzept für foam-bosss
- Welche Klassen müssen erzeugt werden?
- Wie können zusätzliche Bibliotheken hinzgefügt werden?

## Erster Milestone: Initialize BoSSS-Grid
- Klasse `/home/flori/foam/foam-dg/src/discontinuousGalerkin/dgMesh/dgMesh.H`

***
Datum: 07apr21

## Softwarebrücke C++ zu C#
- Varianten: via `libmono` vs. `clr`-Python Modul
- Python-Variante: prinzipiell geil, die .NET-Klassenstruktur wird durch das `clr`-Modul
  fast eins-zu-eins in Python abgebildet.
  Aber: auf C++-Seite wären trotzdem wieder Wrapper-Objekte notwendig, 
  da man ansonsten über die umständliche C-API gehen müsste; Das will man nicht, ist 
  genauso scheisse wie `libmono`
- Daher: **bleibe fürs erste bei** `libmono`; Wrapper-Code geht sowieso immer nach 
  demselben Schema, man könnte also später auch auf eine andere Embedding-API umstellen.

***
Datum: 10apr21

## Zwischenstand
- `libmono`-Variante funktioniert, BoSSS-Gitter wird erzeugt
- btw. mono 6.12 scheint .NET 5 handhaben zu können;
  d.h. ich bin bzgl der .NET 5 - Umstellung mit `libmono`
  weniger eingeschränkt als mit `clr`-Python
- evt. zukünftige Alterantive: .NET (bzw. .NET core) embedding:
  https://yizhang82.dev/hosting-coreclr
  https://docs.microsoft.com/en-us/dotnet/core/tutorials/netcore-hosting

## Nächste Schritte und offene Fragen
- `cellScalarField`, `cellVectorField` und `cellScalarMatrix`, `cellVectorMatrix`,
  aber auch `dgLaplacianScheme`
- Die Frage ist: Welche BoSSS-Klassen sollen/könnenwelchen OpenFOAM-Klassen 
  zugrordnet werden?
- `cellVectorField`, `cellVectorField` könnte man dem `CoordinateMapping` zuordnen
- Detailfrage: Wo kommt in der Main-Datei das Schema `dgLaplacianScheme` ins Spiel?


## Wissen über foam-dg
- `dgScalar` (Datei `dgScalar.H`), bzw. `DGscalar` (Datei `dgScalarTemplate.H`)   halten die DG-Moden pro Zelle 
- in Klasse `dgOrder` (Datei `dgOrder.H`) wird z.B. der max. Polynomgrad hart kodiert.

***
Datum: 13apr21, 14apr21

## Zwischenstand
- Neue Klasse `DgGeometricField` als "Proxy" für die BoSSS-DG-Felder angelegt; aufgrund der hochgradig
  templatisierten Konstruktion deutlich schwerer als erwartet.
- Weiters noch IO-Probleme mit aufgrund Laufzeit-Typinformation der Basisklasse 
  (`preCellScalarField` vs. `cellScalarField`)

## Next Steps
- `dgMatrix<...>` erweitern; soll eine `BlockMsrMatrix` sowie eine RHS (`dgMatrix<...>::source()`) beheimaten (Komposition).
- Neues DG-Schema anlegen; z.B. `BoSSS_SIPLaplacianScheme`; analog zu `SIPLaplacianScheme`;
   `*LaplacianScheme.dgmLaplacian(...)` beschreibt `dgMatrix<...>`;
  Das neue Schema fordert direkt die enthaltene `BlockMsrMatrix` an, welche dann in BoSSS beschrieben werden
- Operatoren für `dgMatrix<...>` müssten überladen werden, kann aber noch warten



***
Datum: 15apr21

## Fragen
- Was ist die Beziehung zwischen `DgGeometricField` und `dgPatchField`?

***
Datum: 16apr21

## Zwischenstand
- Austausch der DG-Koordinaten zwischen `DgGeometricField` (C++)
 und BoSSS-Klasse `OpenFoamDGField` (C#);
- Nächste Schritte wie gehabt, 
  d.h. Schema anlegen, Matrix erweitern.
 
## Fragen
- Evtl. die Template-Parameter von `DgGeometricField` einschränken;
  Sollte eigentlich nur für `dgMesh` (3. Param) korrekt kompilieren,
  daher könnte man diesen Param gleich fixieren.
  Mir aber leider nicht gelungen, 
  siehe Branch `FuckDgGeometricalField`.
  Wie kann man das erreichen?


***
20apr21

## Fragen
- Verschiedene Zeilen- und Spalten für `dgMatrix`,
  z.B. für nicht-quadratische Operatoren (Divergenz, Gradient)?
- Open Issue: Hart-Kodierter DG-Grad und Dimension ist Kakke.

## Punkte/Fragen für Meeting
- Ziele definieren und Einigung bzlg. API; Nicht irgendwas produzieren was Holger nicht interessiert.
- aus meiner Sicht: Für DG-Systeme auf jeden Fall ein Block-Systeme; z.B. für Phasenfeld aber auch wichtig für Stokes-Löser.
- Ansprechpartner für FOAM-Fragen (Compiler-Fehler, API-Design, etc.)












