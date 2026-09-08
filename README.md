# Super Mario War — SMW 2.0 modernization

> **Status: Legacy modernization / Active Development / CODE REVIEWED / P2**

Modernizowana linia źródeł klasycznej gry **Super Mario War**, z naciskiem na uruchamianie starego kodu na współczesnych systemach i stopniowe przejście z legacy SDL/build tooling.

## Co to jest za projekt

To nie jest gra napisana od zera w tym repo. Bazą jest istniejący projekt Super Mario War innych autorów; aktywna linia zawiera jednak realne prace modernizacyjne: CMake, SDL2 migration i poprawki runtime.

## Do czego służy

Celem jest zachowanie gry w działającej formie oraz stworzenie współczesnego baseline'u build/runtime bez utraty podstawowego gameplayu i zawartości legacy.

## Stan projektu

**Status: Legacy modernization / Active Development / CODE REVIEWED / P2**

`SMW2.0_SESSION.md` dokumentuje wcześniejszy zweryfikowany build `smw`, `leveledit` i `worldedit` po migracji SDL2. Ten historyczny wynik nie jest traktowany jako PASS aktualnego HEAD po kolejnych zmianach.

Source review aktywnej ścieżki objął build/runtime modernization, asset/file-list handling, audio crash notes oraz legacy binary I/O. Znaleziony został istotny problem parsera `_src/FileIO.cpp`: scalar readers ignorowały wynik `fread()`, a `ReadString()` ufał długości z pliku. Truncated/corrupt save/config mógł więc użyć niezainicjalizowanych wartości, zażądać ogromnej alokacji lub wejść w invalid index. Reader został zahardeningowany fail-safe bez zmiany formatu plików.

Pozostałe ryzyka:
- formaty save/config są legacy binary i nie mają jeszcze pełnej walidacji strukturalnej,
- duży stary codebase nadal zawiera wiele implicit index/range assumptions,
- historyczne platform projects i Makefile współistnieją z CMake,
- nie ma aktualnego CI/status evidence dla bieżącego HEAD,
- gameplay/content compatibility nie jest automatycznie testowane.

## Provenance

Oryginalni autorzy i historia projektu są opisani w legacy dokumentach, m.in. `README.txt`, `README.html`, `THANKS.txt` i `WHATSNEW.txt`. Modernizacja nie powinna usuwać tych informacji ani przedstawiać całego kodu jako nowego autorstwa.

## Milestone'y i bramki implementacyjne

### M0 — Modern build baseline
**Stan:** VERIFICATION REQUIRED

**Gate SMW00**
- clean configure/build działa na wspieranym systemie,
- `smw`, `leveledit`, `worldedit` budują się z jednego CMake source of truth,
- zależności SDL2 są udokumentowane,
- executable startuje,
- asset lookup nie zależy od katalogu dewelopera.

**PASS:** aktualny SHA: clean checkout → build → menu/game launch.  
**FAIL:** opieramy się wyłącznie na starym session note albo lokalnych legacy bibliotekach.

### M1 — SDL modernization
**Stan:** FUNCTIONAL / VERIFY CURRENT HEAD

Migracja SDL 1.2 → SDL2 została wykonana w aktywnej linii, ale wymaga regresji na aktualnym HEAD.

**Gate SMW01**
- aktywna ścieżka używa jednego wspieranego SDL2 backendu,
- video/input/audio mają smoke tests,
- resize/fullscreen nie crashuje,
- controller/keyboard mapping jest deterministyczny.

**PASS:** smoke matrix video/input/audio przechodzi na współczesnym runtime.  
**FAIL:** podstawowy gameplay nadal zależy od nieobsługiwanej biblioteki lub regresji portu.

### M2 — Legacy data safety
**Stan:** PARTIAL / READER HARDENED

**Gate SMW02**
- truncated save/config nie używa niezainicjalizowanych wartości,
- string lengths są bounded,
- invalid enum/index values są sprawdzane przed użyciem,
- corrupt file daje kontrolowany failure/default zamiast crasha,
- referencyjne stare pliki nadal się wczytują.

**PASS:** malformed/truncated fixture suite nie crashuje i legacy fixtures zachowują kompatybilność.  
**FAIL:** spreparowany lub uszkodzony plik może prowadzić do OOB/resource exhaustion.

### M3 — Gameplay compatibility
**Stan:** TODO

**Gate SMW03**
- start meczu działa,
- ruch/skok/kolizje zachowują referencyjne zachowanie,
- win/death/respawn flow działa,
- reprezentatywny zestaw map i trybów ładuje się poprawnie.

**PASS:** zdefiniowany gameplay regression checklist przechodzi bez krytycznej różnicy.  
**FAIL:** port buduje się, ale zmienia lub łamie podstawową grę.

### M4 — Content compatibility
**Stan:** PARTIAL

**Gate SMW04**
- legacy maps/assets/config są odnajdywane,
- brak assetu daje kontrolowany komunikat,
- formaty użytkownika nie są bez potrzeby łamane,
- ścieżki są portable i case differences są obsłużone zgodnie z decyzją projektu.

**PASS:** referencyjny content pack ładuje się bez ręcznego przenoszenia plików.  
**FAIL:** modernizacja wymaga nieudokumentowanej konwersji lub lokalnych ścieżek.

### M5 — SMW 2.0 baseline
**Stan:** TODO

**Gate SMW10**
- SMW00–SMW04 = PASS,
- build/test/package są powtarzalne,
- provenance/licensing pozostają widoczne,
- known regressions są jawnie opisane,
- release artifact uruchamia się na czystym wspieranym systemie.

**PASS:** SMW 2.0 jest używalną modernizacją istniejącej gry, a nie tylko portem kompilatora.  
**FAIL:** użytkownik musi ręcznie rekonstruować legacy environment.

## Priorytet implementacyjny

1. uruchomić CI dla aktualnego CMake/SDL2 HEAD,
2. dodać malformed/truncated fixtures dla FileIO/save/config,
3. zbudować gameplay/content smoke matrix,
4. dopiero potem usuwać kolejne historyczne platform/build paths.

## Dokumentacja historyczna

Nie usuwać bez osobnej decyzji:
- `README.txt`
- `README.html`
- `SMW 1.8 Readme.htm`
- `THANKS.txt`
- `WHATSNEW.txt`
