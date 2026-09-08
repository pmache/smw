# Super Mario War — SMW 2.0 modernization

Modernizowana linia źródeł klasycznej gry **Super Mario War**, z naciskiem na uruchamianie starego kodu na współczesnych systemach i stopniowe przejście z legacy SDL/build tooling.

## Co to jest za projekt

To nie jest gra napisana od zera w tym repo. Bazą jest istniejący projekt Super Mario War innych autorów; prace w tej linii dotyczą przede wszystkim utrzymania, portowania i modernizacji technicznej. Historyczne README/credits/thanks pozostają w repo jako część provenance.

## Do czego służy

Celem jest zachowanie gry w działającej formie oraz stworzenie współczesnego baseline'u build/runtime bez utraty podstawowego gameplayu i zawartości legacy.

## Stan projektu

**Status: Legacy modernization / P2**

Repo zawiera pełne stare źródła, historyczną dokumentację, CMake/Makefile oraz dokumenty sesji SMW 2.0. Nie należy oceniać postępu po samym „kompiluje się”: najważniejsza jest zgodność gameplay/assets/input/audio z referencyjną wersją.

## Provenance

Oryginalni autorzy i historia projektu są opisani w legacy dokumentach, m.in. `README.txt`, `README.html`, `THANKS.txt` i `WHATSNEW.txt`. Modernizacja nie powinna usuwać tych informacji ani przedstawiać całego kodu jako nowego autorstwa.

## Milestone'y i bramki implementacyjne

### M0 — Modern build baseline
**Stan:** PARTIAL

**Gate SMW00**
- clean configure/build działa na wspieranym systemie,
- zależności są udokumentowane,
- executable startuje,
- asset lookup nie zależy od katalogu dewelopera.

**PASS:** clean checkout → build → menu/game launch.  
**FAIL:** build działa tylko dzięki lokalnym legacy bibliotekom lub absolutnym ścieżkom.

### M1 — SDL modernization
**Stan:** IN PROGRESS

**Gate SMW01**
- legacy SDL API w aktywnej ścieżce jest zastąpione lub izolowane,
- video/input/audio mają jeden wspierany backend,
- resize/fullscreen nie crashuje,
- controller/keyboard mapping jest deterministyczny.

**PASS:** smoke matrix video/input/audio przechodzi na współczesnym runtime.  
**FAIL:** podstawowy gameplay nadal zależy od nieobsługiwanej biblioteki.

### M2 — Gameplay compatibility
**Stan:** TODO

**Gate SMW02**
- start meczu działa,
- ruch/skok/kolizje zachowują referencyjne zachowanie,
- win/death/respawn flow działa,
- co najmniej reprezentatywny zestaw map i trybów ładuje się poprawnie.

**PASS:** zdefiniowany gameplay regression checklist przechodzi bez krytycznej różnicy.  
**FAIL:** port buduje się, ale zmienia lub łamie podstawową grę.

### M3 — Content compatibility
**Stan:** TODO

**Gate SMW03**
- legacy maps/assets/config są odnajdywane,
- brak assetu daje kontrolowany komunikat,
- formaty użytkownika nie są bez potrzeby łamane,
- ścieżki są portable.

**PASS:** referencyjny content pack ładuje się bez ręcznego przenoszenia plików.  
**FAIL:** modernizacja wymaga konwersji danych bez migration path.

### M4 — SMW 2.0 baseline
**Stan:** TODO

**Gate SMW10**
- SMW00–SMW03 = PASS,
- build/test/package są powtarzalne,
- provenance/licensing pozostają widoczne,
- known regressions są jawnie opisane,
- release artifact uruchamia się na czystym wspieranym systemie.

**PASS:** SMW 2.0 jest używalną modernizacją istniejącej gry, a nie tylko portem kompilatora.  
**FAIL:** użytkownik musi ręcznie rekonstruować legacy environment.

## Dokumentacja historyczna

Nie usuwać bez osobnej decyzji:
- `README.txt`
- `README.html`
- `SMW 1.8 Readme.htm`
- `THANKS.txt`
- `WHATSNEW.txt`

Są one częścią historii i informacji o autorstwie projektu.
