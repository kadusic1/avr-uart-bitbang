# avr-uart-bitbang

Softverska (bit-banged) implementacija UART protokola na mikrokontroleru
ATmega328P, pisana u AVR asemblerskom jeziku. Program radi kao echo, odnosno svaki primljeni bajt odmah šalje nazad, bez upotrebe ugrađenog hardverskog
UART modula.

## Struktura projekta

- `src/` - izvorni kod programa
  - `main.S` - glavni fajl, konfiguracija pinova i echo petlja
  - `uart.inc` - potprogrami za slanje i prijem podataka
  - `delay.inc` - potprogram za precizno kašnjenje
  - `pins.inc` - definicije korištenih pinova
- `paper/` - LaTeX izvorni fajlovi seminarskog rada koji objašnjava teoriju,
  proračun tajminga i implementaciju
- `Makefile` - automatizacija asembliranja, programiranja i izrade rada

## Preduslovi

- alat `avra` za asembliranje
- alat `avrdude` za programiranje mikrokontrolera
- alat `tio` za serijsku komunikaciju
- razvojna ploča Arduino Uno R3 (ili druga ploča sa ATmega328P)

## Razvojno okruženje

Kompletan razvoj, kompajliranje i testiranje izvršeni su na operativnom
sistemu Ubuntu 24.04 LTS (Noble Numbat). Korištene su sljedeće verzije
alata:

- `avra` 1.4.2
- `avrdude` 7.1
- `tio` v2.7
- `pdflatex` (pdfTeX) 3.141592653-2.6-1.40.25, TeX Live 2023

Projekat nije testiran na Windowsu. Alati `avra`, `avrdude` i `pdflatex`
imaju Windows verzije, ali `tio`, korišten u `make monitor` cilju, radi
samo na Linuxu, macOS-u i BSD-u, pa bi na Windowsu bilo potrebno
zamijeniti ga nekim drugim programom za serijsku komunikaciju.

## Pokretanje

Asembliranje programa:

```
make build
```

Programiranje mikrokontrolera (podrazumijevani port je `/dev/ttyACM0`,
može se promijeniti preko `PORT` promjenljive):

```
make flash
```

Uspostavljanje serijske veze radi testiranja:

```
make monitor
```

Brisanje generisanih fajlova:

```
make clean
```

Izrada PDF verzije seminarskog rada iz `paper/` direktorija:

```
make paper
```

